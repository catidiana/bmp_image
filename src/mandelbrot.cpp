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

typedef int32_t  s32;

//scale scales figure, keep it from 0 to 2; shift not more than image size; can shift to negative direction a bit
void draw_Mandelbrot (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color = 0xFFFFFF, float scale = 1.0, s32 shift_x = 0, s32 shift_y = 0);
void draw_Mandelbrot_convergence (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000, float scale = 1.0, s32 shift_x = 0, s32 shift_y = 0, u32 max_step = 100);

void draw_Mandelbrot (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color, float scale, s32 shift_x, s32 shift_y)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};
    s32 image_wid = image_w;
    s32 image_hei = image_h;


    for (s32 y = -shift_y; y < image_hei-shift_y; y++)
    {
        for (s32 x = -shift_x; x < image_wid-shift_x; x++)
        {
            double cor_x = 0.0;
            double cor_y = 0.0;
            double x_c = (double)x/(image_wid*scale);
            double y_c = (double)y/(image_hei*scale);
            u32 steps = 0;
            
            while (1) {
                double x_test = cor_x;
                double y_test = cor_y;
                cor_x = x_test*x_test - y_test*y_test + x_c;
                cor_y = 2*x_test*y_test + y_c;
                if (cor_x>2 || cor_x<-2|| cor_y>2 || cor_y<-2)
                {break;}
                
                if ((steps>=1 && cor_x-x_test==0.0 && cor_y-y_test==0.0) || steps>100)
                {
                    pixels[(y+shift_y) * image_wid + x+shift_x] = color;
                    break;
                }
                steps++;
            }
        }
    }

}

void draw_Mandelbrot_convergence (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1,  u32 hex_color2, float scale, s32 shift_x, s32 shift_y, u32 max_step)
{
    u8 r1 = (hex_color1 & 0xff0000) >> 16;
    u8 g1 = (hex_color1 & 0x00ff00) >> 8;
    u8 b1 = (hex_color1 & 0x0000ff);
    u8 r2 = (hex_color2 & 0xff0000) >> 16;
    u8 g2 = (hex_color2 & 0x00ff00) >> 8;
    u8 b2 = (hex_color2 & 0x0000ff);
    
    s32 image_wid = image_w;
    s32 image_hei = image_h;
    
    
    for (s32 y = -shift_y; y < image_hei-shift_y; y++)
    {
        Pixel pixel = {};
        for (s32 x = -shift_x; x < image_wid-shift_x; x++)
        {
            double cor_x = 0.0;
            double cor_y = 0.0;
            double x_c = (double)x/(image_wid*scale);
            double y_c = (double)y/(image_hei*scale);
            u32 steps = 0;
            
            while (1) {
                double x_test = cor_x;
                double y_test = cor_y;
                cor_x = x_test*x_test - y_test*y_test + x_c;
                cor_y = 2*x_test*y_test + y_c;
                if (cor_x > image_w/scale || cor_x < -image_w/scale|| cor_y > image_h/scale || cor_y < -image_h/scale)
                {   pixel.r = r1+max_step*(r2-r1)/(steps+2);
                    pixel.g = g1+max_step*(g2-g1)/(steps+2);
                    pixel.b = b1+max_step*(b2-b1)/(steps+2);
                    pixels[(y+shift_y) * image_wid + x+shift_x] = pixel;
                    break;}
                
                if ((steps>=1 && cor_x-x_test==0.0 && cor_y-y_test==0.0) || steps>max_step)
                {
                    pixel.r = r1;
                    pixel.g = g1;
                    pixel.b = b1;
                    pixels[(y+shift_y) * image_wid + x+shift_x] = pixel;
                    break;
                }
                steps++;
            }
        }
    }
    
}
