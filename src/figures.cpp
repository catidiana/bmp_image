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





// corner_x corner_y - coordinates of bottom left corner
static void
draw_square (Image image, u32 square_w, u32 corner_x = 0, u32 corner_y = 0, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);
    
    for (u32 y = 0; y < image.h; y++)
    {
        for (u32 x = 0; x < image.w; x++)
        {
            if ((x>=corner_x && x<=corner_x+square_w) && (y>=corner_y && y<=corner_y+square_w))
                image.pixels[y * image.w + x] = color;
        }
    }
}


// TODO(Martin): Should be draw_rect or draw_rectangle.
// TODO(Martin): Position coordinates should be before dimensions.
// TODO(Martin): Rectangle of w/h of 1 should be one pixel wide/tall, not 2.
// TODO(Martin): Maybe position s32, not u32, so we can draw partial rectangles too.
// TODO(Martin): Maybe the rectangle should be drawn centered to x and y, or there should be another function for that.
// TODO(Martin): The function should be made much more optimized, as in, it can do it's work in much less cycles; right now it's pretty slow.
static void
draw_rectangular (Image image, u32 square_w, u32 square_h, u32 corner_x = 0, u32 corner_y = 0, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);
    
    for (u32 y = 0; y < image.h; y++)
    {
        for (u32 x = 0; x < image.w; x++)
        {
            if ((x>=corner_x && x<=corner_x+square_w) && (y>=corner_y && y<=corner_y+square_h))
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
