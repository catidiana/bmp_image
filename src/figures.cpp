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





static void
draw_square (Image image, s32 center_x, s32 center_y, u32 square_w, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);

    s32 start_x = center_x - square_w/2;
    s32 finish_x = center_x + square_w/2 + square_w%2;
    if (start_x < 0) start_x = 0;
    if (start_x > image.w) return;
    if (finish_x < 0) return;
    if (finish_x > image.w) finish_x = image.w;

    s32 start_y = center_y - square_w/2;
    s32 finish_y = center_y + square_w/2 + square_w%2;
    if (start_y < 0) start_y = 0;
    if (start_y > image.h) return;
    if (finish_y < 0) return;
    if (finish_y > image.h) finish_y = image.h;
    
    for (u32 y = start_y; y < finish_y; y++)
    {
        for (u32 x = start_x; x < finish_x; x++)
        {
            image.pixels[y * image.w + x] = color;
        }
    }
}


static void
draw_rectangle (Image image, s32 center_x, s32 center_y, u32 rect_w, u32 rect_h, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);
    
    s32 start_x = center_x - rect_w/2;
    s32 finish_x = center_x + rect_w/2 + rect_w%2;
    if (start_x < 0) start_x = 0;
    if (start_x > image.w) return;
    if (finish_x < 0) return;
    if (finish_x > image.w) finish_x = image.w;

    s32 start_y = center_y - rect_h/2;
    s32 finish_y = center_y + rect_h/2 + rect_h%2;
    if (start_y < 0) start_y = 0;
    if (start_y > image.h) return;
    if (finish_y < 0) return;
    if (finish_y > image.h) finish_y = image.h;

    for (u32 y = start_y; y < finish_y; y++)
    {
        for (u32 x = start_x; x < finish_x; x++)
        {
            image.pixels[y * image.w + x] = color;
        }
    }
}



static void
draw_circle (Image image, u32 radius, u32 center_x, u32 center_y, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);
    
    for (u32 y = 0; y < image.h; y++)
    {
        for (u32 x = 0; x < image.w; x++)
        {
            if ((x-center_x)*(x-center_x)+(y-center_y)*(y-center_y)<=radius*radius)
                image.pixels[y * image.w + x] = color;
        }
    }
}


static void
draw_ellipse (Image image, u32 ellipse_w, u32 ellipse_h, u32 center_x, u32 center_y, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);    
    
    for (u32 y = 0; y < image.h; y++)
    {
        for (u32 x = 0; x < image.w; x++)
        {
            r64 x_fraction = 4.0*((x-center_x)*(x-center_x))/(ellipse_w*ellipse_w);
            r64 y_fraction = 4.0*((y-center_y)*(y-center_y))/(ellipse_h*ellipse_h);
            if (x_fraction+y_fraction<=1.0)
                image.pixels[y * image.w + x] = color;
        }
    }
}


static void
draw_equilateral_triangle (Image image, u32 tringle_side, u32 corner_x = 0, u32 corner_y = 0, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color); 
    
    for (u32 y = 0; y < image.h; y++)
    {
        for (u32 x = 0; x < image.w; x++)
        {
            r64 alfa = sqrt(3);
            if ((x>=corner_x && x<=corner_x+tringle_side) && (y > corner_y && y < corner_y+alfa*(x-corner_x) && y < corner_y+alfa*(corner_x+tringle_side-x)))
            {
                image.pixels[y * image.w + x] = color;
            }
        }
    }
}
