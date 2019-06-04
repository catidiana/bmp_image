#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#pragma pack(push, 1)
struct Pixel {
  u8 b, g, r;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BMPHeader {
  char magic_number[2];
  u32  file_size;
  u16  reserved1;
  u16  reserved2;
  u32  offset_bits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BMPImageHeader {
  u32 header_size;
  u32 width;
  u32 height;
  u16 planes;
  u16 bit_count;
  u32 compression;
  u32 image_size;
  u32 ppm_x;
  u32 ppm_y;
  u32 clr_used;
  u32 clr_important;
};
#pragma pack(pop)


int
main()
{
  u32 width  = 400;
  u32 height = 400;
  u32 pixels_count = width * height;
  u32 image_size = pixels_count * 3;

  BMPHeader file_header = {{'B', 'M'}};
  file_header.file_size = sizeof (BMPHeader) + sizeof (BMPImageHeader) + image_size;

  BMPImageHeader image_header = {};
  image_header.header_size = 40;
  image_header.width      = width;
  image_header.height     = height;
  image_header.planes     = 1;
  image_header.bit_count  = 24;
  image_header.image_size = file_header.file_size;  // Shoudn't this be image_size?
  u32 dpi = 96;
  u32 ppm = dpi * 39.375;
  image_header.ppm_x      = ppm;
  image_header.ppm_y      = ppm;

  Pixel *pixels = (Pixel *) malloc (sizeof (Pixel) * pixels_count);

  for (u32 y = 0; y < height; y++)
    {
      for (u32 x = 0; x < width; x++)
        {
          Pixel pixel = {};
          pixel.r = 0;
          pixel.g = 256 * y / height;
          pixel.b = 256 * x / width;

          pixels[y * width + x] = pixel;
        }
    }

  FILE *image = fopen ("image.bmp", "wb");
  fwrite (&file_header,  1, sizeof (BMPHeader), image);
  fwrite (&image_header, 1, sizeof (BMPImageHeader), image);
  fwrite (pixels, sizeof (Pixel), pixels_count, image);
  fclose(image);

  return 0;
}
