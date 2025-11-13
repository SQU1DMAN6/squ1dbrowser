use crate::dom::Document;
use crate::layout::LayoutEngine;

pub struct PageRenderer;

pub struct RenderOutput {
    pub pixels: Vec<u8>,
    pub width: u32,
    pub height: u32,
}

impl PageRenderer {
    pub fn render(
        doc: &Document,
        width: u32,
        height: u32,
    ) -> RenderOutput {
        // Step 1: Layout
        let layout = LayoutEngine::layout(doc, width as f32, height as f32);

        // Step 2: Paint
        // Use a simple in-memory RGBA buffer so we don't depend on the `image` crate.
        let mut img = SimpleImage::new(width, height);
        img.fill(255, 255, 255, 255);

        // Paint layout boxes
        Self::paint_tree(&layout.root, &mut img);

        let pixels = img.into_raw();

        RenderOutput { pixels, width, height }
    }

    fn paint_tree(layout: &crate::layout::LayoutBox, img: &mut SimpleImage) {
        // Draw box borders
        if layout.height > 0.0 && layout.width > 0.0 {
            let tag = layout
                .element
                .as_ref()
                .map(|e| e.tag.as_str())
                .unwrap_or("");

            let (r, g, b) = match tag {
                "h1" | "h2" | "h3" => (50, 50, 200),
                "p" => (0, 0, 0),
                "div" => (200, 200, 200),
                _ => (100, 100, 100),
            };

            // Draw text or background
            if let Some(text) = &layout.text {
                Self::draw_text(img, layout.x as u32, layout.y as u32, text, r, g, b);
            }
        }

        // Recursively paint children
        for child in &layout.children {
            Self::paint_tree(child, img);
        }
    }

    fn draw_text(img: &mut SimpleImage, x: u32, y: u32, text: &str, r: u8, g: u8, b: u8) {
        // Use bitmap font for text rendering
        let mut px = x as i32;
        let py = y as i32;
        
        for c in text.chars() {
            // Stop if we've gone off the right edge
            if px + 4 >= img.width as i32 {
                break;
            }
            crate::bitmap_font::draw_char(&mut img.pixels, img.width, img.height, px, py, c, r, g, b);
            px += 4; // 3 pixels for char + 1 pixel spacing
        }
    }
}

impl RenderOutput {
    /// Save the rendered image as a BMP file (no external crates).
    pub fn to_bmp(&self, path: &str) -> Result<(), Box<dyn std::error::Error>> {
        // We'll write a 24-bit BMP (BGR) ignoring alpha.
        let w = self.width as u32;
        let h = self.height as u32;
        let row_pad = (4 - (w * 3) % 4) % 4;
        let mut file = std::fs::File::create(path)?;

        let pixel_data_size = (w * 3 + row_pad) * h;
        let file_size = 14 + 40 + pixel_data_size;

        // BITMAPFILEHEADER
        file.write_all(&[
            b'B', b'M',
            (file_size & 0xFF) as u8,
            ((file_size >> 8) & 0xFF) as u8,
            ((file_size >> 16) & 0xFF) as u8,
            ((file_size >> 24) & 0xFF) as u8,
            0, 0, 0, 0, // reserved
            54, 0, 0, 0, // pixel data offset
        ])?;

        // BITMAPINFOHEADER
        file.write_all(&[
            40, 0, 0, 0,
            (w & 0xFF) as u8,
            ((w >> 8) & 0xFF) as u8,
            ((w >> 16) & 0xFF) as u8,
            ((w >> 24) & 0xFF) as u8,
            (h & 0xFF) as u8,
            ((h >> 8) & 0xFF) as u8,
            ((h >> 16) & 0xFF) as u8,
            ((h >> 24) & 0xFF) as u8,
            1, 0, // planes
            24, 0, // bits per pixel
            0, 0, 0, 0, // compression
            (pixel_data_size & 0xFF) as u8,
            ((pixel_data_size >> 8) & 0xFF) as u8,
            ((pixel_data_size >> 16) & 0xFF) as u8,
            ((pixel_data_size >> 24) & 0xFF) as u8,
            0, 0, 0, 0, // ppm X
            0, 0, 0, 0, // ppm Y
            0, 0, 0, 0, // colors used
            0, 0, 0, 0, // important colors
        ])?;

        // Pixel data: BMP stores rows bottom-up
        for row in (0..h).rev() {
            for col in 0..w {
                let i = ((row * w + col) * 4) as usize;
                let r = self.pixels[i];
                let g = self.pixels[i + 1];
                let b = self.pixels[i + 2];
                file.write_all(&[b, g, r])?;
            }
            // padding
            for _ in 0..row_pad {
                file.write_all(&[0u8])?;
            }
        }

        Ok(())
    }
}

// Simple RGBA image helper without external crates
use std::io::Write;

pub struct SimpleImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u8>, // RGBA
}

impl SimpleImage {
    pub fn new(width: u32, height: u32) -> Self {
        Self { width, height, pixels: vec![0; (width * height * 4) as usize] }
    }

    pub fn fill(&mut self, r: u8, g: u8, b: u8, a: u8) {
        for i in 0..(self.pixels.len() / 4) {
            let base = i * 4;
            self.pixels[base] = r;
            self.pixels[base + 1] = g;
            self.pixels[base + 2] = b;
            self.pixels[base + 3] = a;
        }
    }

    pub fn put_pixel(&mut self, x: u32, y: u32, r: u8, g: u8, b: u8, a: u8) {
        if x >= self.width || y >= self.height { return; }
        let idx = ((y * self.width + x) * 4) as usize;
        self.pixels[idx] = r;
        self.pixels[idx + 1] = g;
        self.pixels[idx + 2] = b;
        self.pixels[idx + 3] = a;
    }

    pub fn into_raw(self) -> Vec<u8> { self.pixels }
}
