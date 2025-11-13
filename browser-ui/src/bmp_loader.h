#pragma once

#include <vector>
#include <cstdint>
#include <string>

// Simple BMP loader - reads 24-bit or 32-bit BMP files into RGBA format
class BMPLoader {
public:
    struct Image {
        uint32_t width = 0;
        uint32_t height = 0;
        std::vector<uint8_t> pixels; // RGBA format
    };

    static Image load(const std::string& path) {
        Image img;
        std::FILE* file = std::fopen(path.c_str(), "rb");
        if (!file) {
            return img; // Return empty on error
        }

        // BMP file header (14 bytes)
        uint8_t header[14];
        if (std::fread(header, 1, 14, file) != 14) {
            std::fclose(file);
            return img;
        }

        // Check signature "BM"
        if (header[0] != 'B' || header[1] != 'M') {
            std::fclose(file);
            return img;
        }

        // DIB header (at least 40 bytes for BITMAPINFOHEADER)
        uint8_t dib_header[40];
        if (std::fread(dib_header, 1, 40, file) != 40) {
            std::fclose(file);
            return img;
        }

        // Parse DIB header
        img.width = read_u32_le(&dib_header[4]);
        img.height = read_u32_le(&dib_header[8]);
        uint16_t bits_per_pixel = read_u16_le(&dib_header[14]);

        // Seek to pixel data
        uint32_t pixel_offset = read_u32_le(&header[10]);
        std::fseek(file, pixel_offset, SEEK_SET);

        // Calculate row padding (BMP rows are 4-byte aligned)
        int bytes_per_pixel = bits_per_pixel / 8;
        int row_size = ((img.width * bits_per_pixel + 31) / 32) * 4;
        int pixel_data_size = row_size * img.height;

        // Allocate output buffer (RGBA)
        img.pixels.resize(img.width * img.height * 4);

        // Read and convert pixel data (BMP is bottom-up, BGR format)
        std::vector<uint8_t> bmp_data(pixel_data_size);
        if (std::fread(bmp_data.data(), 1, pixel_data_size, file) != pixel_data_size) {
            std::fclose(file);
            img.pixels.clear();
            return img;
        }

        // Convert BMP (BGR/BGRA, bottom-up) to RGBA (top-down)
        for (uint32_t y = 0; y < img.height; ++y) {
            uint32_t bmp_y = img.height - 1 - y; // Flip vertically
            uint8_t* bmp_row = &bmp_data[bmp_y * row_size];
            uint8_t* rgba_row = &img.pixels[y * img.width * 4];

            for (uint32_t x = 0; x < img.width; ++x) {
                uint8_t b = bmp_row[x * bytes_per_pixel];
                uint8_t g = bmp_row[x * bytes_per_pixel + 1];
                uint8_t r = bmp_row[x * bytes_per_pixel + 2];
                uint8_t a = (bits_per_pixel == 32) ? bmp_row[x * bytes_per_pixel + 3] : 255;

                rgba_row[x * 4 + 0] = r;
                rgba_row[x * 4 + 1] = g;
                rgba_row[x * 4 + 2] = b;
                rgba_row[x * 4 + 3] = a;
            }
        }

        std::fclose(file);
        return img;
    }

private:
    static uint16_t read_u16_le(const uint8_t* p) {
        return p[0] | (p[1] << 8);
    }

    static uint32_t read_u32_le(const uint8_t* p) {
        return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
    }
};
