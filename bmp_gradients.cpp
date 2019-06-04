#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef int16_t s16;


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
//put HEX color code like 0x000000
void uniform_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color= 0xFFFFFF);
//shift - number from 0 to 1 that shift gragient, making part of the file one color, squeezing gradient
void linear_vertical_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000, float shift = 0);
void linear_horizontal_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000, float shift = 0);
void diagonal_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000);
void radial_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000);



int
main()
{
    u32 width  = 800;
    u32 height = 500;
    u32 pixels_count = width * height;
    u32 image_size = pixels_count * 3;
    Pixel *pixels = (Pixel *) malloc (sizeof (Pixel) * pixels_count);
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


    FILE *image = fopen ("image.bmp", "wb");
    fwrite (&file_header,  1, sizeof (BMPHeader), image);
    fwrite (&image_header, 1, sizeof (BMPImageHeader), image);
    radial_gradient_fill (pixels, width, height, 0x800000, 0x000080);
    fwrite (pixels, sizeof (Pixel), pixels_count, image);
    fclose(image);
    free (pixels);

    return 0;
}


void uniform_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_h; x++)
        {
            pixels[y * image_w + x] = color;
        }
    }
}

void linear_vertical_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1, u32 hex_color2, float shift)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);

    for (u32 y = 0; y < image_h; y++)
    {
        Pixel pixel = {};
        if (y<image_h*shift)
        {
            pixel.r = r1;
            pixel.g = g1;
            pixel.b = b1;
        }
        else {
            pixel.r = r1 - r1*(y-image_h*shift) / (image_h - image_h*shift) + r2*(y-image_h*shift) / (image_h - image_h*shift);
            pixel.g = g1 - g1*(y-image_h*shift) / (image_h - image_h*shift) + g2*(y-image_h*shift) / (image_h - image_h*shift);
            pixel.b = b1 - b1*(y-image_h*shift) / (image_h - image_h*shift) + b2*(y-image_h*shift) / (image_h - image_h*shift);
        }
        for (u32 x = 0; x < image_w; x++)
        {
            pixels[y * image_w + x] = pixel;
        }
    }
}

void linear_horizontal_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1, u32 hex_color2, float shift)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);

    for (u32 x = 0; x < image_w; x++)
    {

        Pixel pixel = {};
        if (x<image_w*shift)
        {
            pixel.r = r1;
            pixel.g = g1;
            pixel.b = b1;
        }
        else {
            pixel.r = r1 - r1*(x-image_w*shift) / (image_w - image_w*shift) + r2*(x-image_w*shift) / (image_w - image_w*shift);
            pixel.g = g1 - g1*(x-image_w*shift) / (image_w - image_w*shift) + g2*(x-image_w*shift) / (image_w - image_w*shift);
            pixel.b = b1 - b1*(x-image_w*shift) / (image_w - image_w*shift) + b2*(x-image_w*shift) / (image_w - image_w*shift);
        }
        for (u32 y = 0; y < image_h; y++)
        {
            pixels[y * image_w + x] = pixel;
        }
    }
}

void diagonal_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1, u32 hex_color2)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);

    for (u32 y = 0; y < image_h; y++)
    {
        Pixel pixel = {};
        for (u32 x = 0; x < image_w; x++)
        {
            pixel.r = r1 +r2*(x*image_w+y*image_h)/(image_w*image_w+image_h*image_h)-r1*(x*image_w+y*image_h)/(image_w*image_w+image_h*image_h);
            pixel.g = g1 +g2*(x*image_w+y*image_h)/(image_w*image_w+image_h*image_h)-g1*(x*image_w+y*image_h)/(image_w*image_w+image_h*image_h);
            pixel.b = b1 +b2*(x*image_w+y*image_h)/(image_w*image_w+image_h*image_h)-b1*(x*image_w+y*image_h)/(image_w*image_w+image_h*image_h);

            pixels[y * image_w + x] = pixel;
        }
    }
}

void radial_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1, u32 hex_color2)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);

    for (u32 y = 0; y < image_h; y++)
    {
        Pixel pixel = {};
        for (u32 x = 0; x < image_w; x++)
        {
         if(image_h<=image_w)
         {
            double fraction = ((x-image_w/2.0)*(x-image_w/2.0)+(y-image_h/2.0)*(y-image_h/2.0))/(image_h*image_h/4.0);
            pixel.r = sqrt(r1*r1+r2*r2*fraction-r1*r1*fraction);
            pixel.g = sqrt(g1*g1+g2*g2*fraction-g1*g1*fraction);
            pixel.b = sqrt(b1*b1+b2*b2*fraction-b1*b1*fraction);
            pixels[y * image_w + x] = pixel;
         }
         else {
             double fraction = ((x-image_w/2.0)*(x-image_w/2.0)+(y-image_h/2.0)*(y-image_h/2.0))/(image_w*image_w/4.0);
             pixel.r = sqrt(r1*r1+r2*r2*fraction-r1*r1*fraction);
             pixel.g = sqrt(g1*g1+g2*g2*fraction-g1*g1*fraction);
             pixel.b = sqrt(b1*b1+b2*b2*fraction-b1*b1*fraction);
             pixels[y * image_w + x] = pixel;
         }
        }
    }
}



