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


// scale scales figure, keep it from 0 to 2; shift not more than image
// size; can shift to negative direction a bit
static void
draw_mandelbrot (Image image, u32 hex_color, r32 scale, s32 shift_x, s32 shift_y, u32 max_steps=100)
{
  V3 pixel = to_color (hex_color);

  s32 image_w = image.w;
  s32 image_h = image.h;

  for (s32 y = -shift_y; y < image_h - shift_y; y++)
    {
      for (s32 x = -shift_x; x < image_w - shift_x; x++)
        {
          r64 cor_x = 0.0;
          r64 cor_y = 0.0;
          r64 x_c = (r64) x / (image_w * scale);
          r64 y_c = (r64) y / (image_h * scale);

          for (u32 steps = 0; ; ++steps) {
            r64 x_test = cor_x;
            r64 y_test = cor_y;
            cor_x = x_test*x_test - y_test*y_test + x_c;
            cor_y = 2 * x_test * y_test + y_c;

            if (cor_x > 2 || cor_x < -2|| cor_y > 2 || cor_y < -2) break;

            if ((steps >= 1 && cor_x - x_test == 0.0 && cor_y - y_test == 0.0) || steps > max_steps)
              {
                image.pixels[(y + shift_y) * image.w + x + shift_x] = pixel;
                break;
              }
          }
        }
    }

}


static void
draw_mandelbrot_convergence (Image image, u32 hex_color1, u32 hex_color2, r32 scale, s32 shift_x, s32 shift_y, u32 max_steps=100)
{
    V3 c1 = to_color (hex_color1);
    V3 c2 = to_color (hex_color2);
    u8 r1 = c1.r;
    u8 g1 = c1.g;
    u8 b1 = c1.b;
    u8 r2 = c2.r;
    u8 g2 = c2.g;
    u8 b2 = c2.b;
    
    s32 image_w = image.w;
    s32 image_h = image.h;
    
    for (s32 y = 0; y < image_h; y++)
    {
        V3 pixel = {};
        for (s32 x = 0; x < image_w; x++)
        {
            r64 cor_x = 0.0;
            r64 cor_y = 0.0;
            r64 x_c = (r64) (x-shift_x) / (image_w * scale);
            r64 y_c = (r64) (y-shift_y) / (image_h * scale);
            
            for (u32 steps = 0; ; ++steps) {
                r64 x_test = cor_x;
                r64 y_test = cor_y;
                cor_x = x_test*x_test - y_test*y_test + x_c;
                cor_y = 2 * x_test * y_test + y_c;
                
                if (cor_x*cor_x+cor_y*cor_y > 4)
                {
                    
                    if (steps<=25)
                    {pixel.r = steps*10.2;
                        pixel.g = steps*10.2;
                        pixel.b = 255;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>25 && steps<=50)
                    {pixel.r = 255;
                        pixel.g = 255 - (steps-25)*3.8;
                        pixel.b = 255 - (steps-25)*10.2;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>50 && steps<=75)
                    {pixel.r = 255 - (steps-50)*10.2;
                        pixel.g = 160 - (steps-50)*3.8;
                        pixel.b = (steps-50)*10.2;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>75 && steps<=100)
                    {pixel.r = (steps-75)*10.2;
                        pixel.g = (steps-75)*10.2;
                        pixel.b = 255;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>100 && steps<=125)
                    {pixel.r = 255;
                        pixel.g = 255 - (steps-100)*3.8;
                        pixel.b = 255 - (steps-100)*10.2;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>125 && steps <=150)
                    {pixel.r = 255 - (steps-125)*3.8;
                        pixel.g = 160 - (steps-125)*5.1;
                        pixel.b = (steps-125)*10.2;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>150 && steps <=175)
                    {pixel.r = (steps-150)*10.2;
                        pixel.g = (steps-150)*10.2;
                        pixel.b = 255;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else if (steps>175 && steps <=200)
                    {pixel.r = 255;
                        pixel.g = 255 - (steps-175)*3.8;
                        pixel.b = 255 - (steps-175)*10.2;
                        image.pixels[y * image.w + x] = pixel;
                        break;}
                    else {
                        pixel.r = 255 - (steps-200)*10.2;
                        pixel.g = 160 - (steps-200)*3.8;
                        pixel.b = (steps-200)*10.2;
                        image.pixels[y * image.w + x] = pixel;
                        break;
                    }
                    
                    
                }
                if ((steps >= 1 && cor_x - x_test == 0.0 && cor_y - y_test == 0.0) || steps > max_steps)
                {
                    pixel.r = 0;
                    pixel.g = 0;
                    pixel.b = 0;
                    image.pixels[y * image.w + x] = pixel;
                    break;
                }
            }
        }
    }
    
}

