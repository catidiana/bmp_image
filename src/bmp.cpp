/* BMP Drawing program
 *
 * Copyright (C) 2019 Martin & Diana
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#import "gradients.cpp"

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
  u32 width  = 800;
  u32 height = 500;
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

    FILE *image = fopen ("image.bmp", "wb");
    fwrite (&file_header,  1, sizeof (BMPHeader), image);
    fwrite (&image_header, 1, sizeof (BMPImageHeader), image);

    radial_gradient_fill (pixels, width, height, 0x800000, 0x000080);

    fwrite (pixels, sizeof (Pixel), pixels_count, image);
    fclose(image);
    free (pixels);

  return 0;
}
