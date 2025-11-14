use squ1d_renderer::{
    html_parser::HtmlParser, renderer::PageRenderer,
};
use std::env;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();

    // Parse command-line arguments
    // Usage: renderer [html_or_url] [width] [height] [output_file]
    // Example: renderer "<html>...</html>" 800 600 /tmp/render.bmp
    // Or with default test HTML if no args
    let (input, width, height, output_file) = if args.len() >= 4 {
        let w: u32 = args[2].parse().unwrap_or(800);
        let h: u32 = args[3].parse().unwrap_or(600);
        let out = if args.len() >= 5 { args[4].clone() } else { "/tmp/squ1d_render.bmp".to_string() };
        (args[1].clone(), w, h, out)
    } else {
        // Default test HTML
        let test_html = r#"
    <!DOCTYPE html>
    <html>
    <head>
        <title>SQU1D Browser</title>
    </head>
    <body>
        <h1>Welcome to SQU1D Browser</h1>
        <p>This is a test rendering.</p>
        <p>The renderer is working!</p>
    </body>
    </html>
        "#.to_string();
        (test_html, 800, 600, "/tmp/squ1d_render.bmp".to_string())
    };

    // If input starts with '@' treat the rest as a filename and read the HTML from that file
    let html_source = if input.starts_with('@') {
        let path = &input[1..];
        std::fs::read_to_string(path)?
    } else {
        input.clone()
    };

    eprintln!("Parsing HTML... preview: {}", &html_source.chars().take(64).collect::<String>());
    let doc = HtmlParser::parse(&html_source)?;

    eprintln!("Rendering page at {}x{}...", width, height);
    let output = PageRenderer::render(&doc, width, height);

    eprintln!("Rendered {}x{} image with {} bytes", output.width, output.height, output.pixels.len());

    // Save BMP to specified location
    if let Err(e) = output.to_bmp(&output_file) {
        eprintln!("Failed to write BMP: {}", e);
    } else {
        eprintln!("Wrote {}", output_file);
    }

    Ok(())
}
