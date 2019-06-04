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


// functions to draw figures

// corner_x corner_y - coordinates of bottom left corner
void draw_square (Pixel *pixels, u32 image_w, u32 image_h, u32 square_w, u32 corner_x = 0, u32 corner_y = 0, u32 hex_color = 0x000000);
void draw_rectangular (Pixel *pixels, u32 image_w, u32 image_h, u32 square_w, u32 square_h, u32 corner_x = 0, u32 corner_y = 0, u32 hex_color = 0x000000);
void draw_circle (Pixel *pixels, u32 image_w, u32 image_h, u32 radius, u32 center_x, u32 center_y, u32 hex_color = 0x000000);
void draw_ellipse (Pixel *pixels, u32 image_w, u32 image_h, u32 ellipse_w, u32 ellipse_h, u32 center_x, u32 center_y, u32 hex_color = 0x000000);
void draw_equilateral_triangle (Pixel *pixels, u32 image_w, u32 image_h, u32 tringle_side, u32 corner_x = 0, u32 corner_y = 0, u32 hex_color = 0x000000);


void draw_square (Pixel *pixels, u32 image_w, u32 image_h, u32 square_w, u32 corner_x, u32 corner_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_w; x++)
        {
            if ((x>=corner_x && x<=corner_x+square_w) && (y>=corner_y && y<=corner_y+square_w))
                pixels[y * image_w + x] = color;
        }
    }
}

void draw_rectangular (Pixel *pixels, u32 image_w, u32 image_h, u32 square_w, u32 square_h, u32 corner_x, u32 corner_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_w; x++)
        {
            if ((x>=corner_x && x<=corner_x+square_w) && (y>=corner_y && y<=corner_y+square_h))
                pixels[y * image_w + x] = color;
        }
    }
}

void draw_circle (Pixel *pixels, u32 image_w, u32 image_h, u32 radius, u32 center_x, u32 center_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_w; x++)
        {
            if ((x-center_x)*(x-center_x)+(y-center_y)*(y-center_y)<=radius*radius)
                pixels[y * image_w + x] = color;
        }
    }
}

void draw_ellipse (Pixel *pixels, u32 image_w, u32 image_h, u32 ellipse_w, u32 ellipse_h, u32 center_x, u32 center_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_w; x++)
        {
            double x_fraction = 4.0*((x-center_x)*(x-center_x))/(ellipse_w*ellipse_w);
            double y_fraction = 4.0*((y-center_y)*(y-center_y))/(ellipse_h*ellipse_h);
            if (x_fraction+y_fraction<=1.0)
                pixels[y * image_w + x] = color;
        }
    }
}

void draw_equilateral_triangle (Pixel *pixels, u32 image_w, u32 image_h, u32 tringle_side, u32 corner_x, u32 corner_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_w; x++)
        {
            double alfa = sqrt(3);
            if ((x>=corner_x && x<=corner_x+tringle_side) && (y > corner_y && y < corner_y+alfa*(x-corner_x) && y < corner_y+alfa*(corner_x+tringle_side-x)))
            {
                pixels[y * image_w + x] = color;
            }


        }
    }
}
