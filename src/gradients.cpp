/* Graphics drawing program
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


static void
uniform_fill (Image image, u32 hex_color)
{
  V3 color = to_color (hex_color);

  for (u32 y = 0; y < image.h; y++)
    {
      for (u32 x = 0; x < image.w; x++)
        {
          image.pixels[y * image.w + x] = color;
        }
    }
}

//shift - number from 0 to 1 that shift gradient, making part of the file one color, squeezing gradient
void linear_vertical_gradient_fill (V3 *pixels, u32 image_w, u32 image_h, u32 hex_color1, u32 hex_color2, float shift)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);

    for (u32 y = 0; y < image_h; y++)
    {
        V3 pixel = {};
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

void linear_horizontal_gradient_fill (V3 *pixels, u32 image_w, u32 image_h, u32 hex_color1, u32 hex_color2, float shift)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);

    for (u32 x = 0; x < image_w; x++)
    {

        V3 pixel = {};
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


static void
diagonal_gradient_fill (Image image, u32 hex_color1, u32 hex_color2)
{
  u8 r1 = (hex_color1 & 0xff0000) >> 16;
  u8 g1 = (hex_color1 & 0x00ff00) >> 8;
  u8 b1 = (hex_color1 & 0x0000ff);
  u8 r2 = (hex_color2 & 0xff0000) >> 16;
  u8 g2 = (hex_color2 & 0x00ff00) >> 8;
  u8 b2 = (hex_color2 & 0x0000ff);

  for (u32 y = 0; y < image.h; y++)
    {
      V3 pixel = {};
      for (u32 x = 0; x < image.w; x++)
        {
          pixel.r = r1 +r2*(x*image.w+y*image.h)/(image.w*image.w+image.h*image.h)-r1*(x*image.w+y*image.h)/(image.w*image.w+image.h*image.h);
          pixel.g = g1 +g2*(x*image.w+y*image.h)/(image.w*image.w+image.h*image.h)-g1*(x*image.w+y*image.h)/(image.w*image.w+image.h*image.h);
          pixel.b = b1 +b2*(x*image.w+y*image.h)/(image.w*image.w+image.h*image.h)-b1*(x*image.w+y*image.h)/(image.w*image.w+image.h*image.h);

          image.pixels[y * image.w + x] = pixel;
        }
    }
}


static void
radial_gradient_fill (Image image, u32 hex_color1, u32 hex_color2)
{
  uniform_fill (image, hex_color2);
  V3 c1 = to_color (hex_color1);
  V3 c2 = to_color (hex_color2);
  u8 r1 = c1.r;
  u8 g1 = c1.g;
  u8 b1 = c1.b;
  u8 r2 = c2.r;
  u8 g2 = c2.g;
  u8 b2 = c2.b;
  u32 radius = image.w;
  if (image.w > image.h) radius = image.h;

  for (u32 y = 0; y < image.h; y++)
    {
      V3 pixel = {};
      for (u32 x = 0; x < image.w; x++)
        {
          if((2*x-radius)*(2*x-radius)+(2*y-radius)*(2*y-radius)      <=radius*radius)
            {
              double fraction = ((x-image.w/2.0)*(x-image.w/2.0)+(y-image.h/2.0)*(y-image.h/2.0))/(radius*radius/4.0);
              pixel.r = sqrt(r1*r1+r2*r2*fraction-r1*r1*fraction);
              pixel.g = sqrt(g1*g1+g2*g2*fraction-g1*g1*fraction);
              pixel.b = sqrt(b1*b1+b2*b2*fraction-b1*b1*fraction);
              image.pixels[y * image.w + x] = pixel;
            }
        }
    }
}



static void
draw_gradient_tiles (Image image, int x_offset, int y_offset)
{
  V3 pixel = {};
  for (u32 y = 0; y < image.h; ++y)
    {
      pixel.b = y + y_offset;
      for (u32 x = 0; x < image.w; ++x)
        {
          pixel.g = x + x_offset;
          image.pixels[image.w * y + x] = pixel;
        }
    }
}
