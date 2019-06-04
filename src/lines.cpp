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

//functions to draw lines

void draw_vertical_line (Pixel *pixels, u32 image_w, u32 image_h, u32 line_position_x, u32 hex_color = 0x000000, u32 line_width = 1);
void draw_horizontal_line (Pixel *pixels, u32 image_w, u32 image_h, u32 line_position_y, u32 hex_color = 0x000000, u32 line_width = 1);
//put angle in degrees; corner_x corner_y - coordinates of the dot that should belong to line
void draw_angle_line (Pixel *pixels, u32 image_w, u32 image_h, u32 angle = 45, u32 hex_color = 0x000000, u32 corner_x = 0, u32 corner_y = 0, u32 line_width = 1);
//build smooth path based on segment P0(x,y) - P1(x,y)
void linear_bezier_curve (Pixel *pixels, u32 image_w, u32 image_h, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 hex_color = 0x000000);
//build smooth path based on segments P0(x,y) - P1(x,y) - P2(x,y)
void quadratic_bezier_curve (Pixel *pixels, u32 image_w, u32 image_h, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 P2_x, u32 P2_y, u32 hex_color = 0x000000);
//build smooth path based on segments P0(x,y) - P1(x,y) - P2(x,y) - P3 (x,y)
void cubic_bezier_curve (Pixel *pixels, u32 image_w, u32 image_h, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 P2_x, u32 P2_y, u32 P3_x, u32 P3_y, u32 hex_color = 0x000000);

void draw_vertical_line (Pixel *pixels, u32 image_w, u32 image_h, u32 line_position_x, u32 hex_color, u32 line_width)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);
    u32 line_start;
    if (line_position_x<line_width/2) line_start=0;
    else line_start = line_position_x-line_width/2;
    if (line_start>image_w) return;
    u32 line_end = line_position_x+line_width/2+line_width%2;
    if (line_end>image_w) line_end = image_w;

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = line_start; x < line_end; x++)
        {

            pixels[y * image_w + x] = color;
        }
    }
}

void draw_horizontal_line (Pixel *pixels, u32 image_w, u32 image_h, u32 line_position_y, u32 hex_color, u32 line_width)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);
    u32 line_start;
    if (line_position_y<line_width/2) line_start=0;
    else line_start = line_position_y-line_width/2;
    if (line_start>image_h) return;
    u32 line_end = line_position_y+line_width/2+line_width%2;
    if (line_end>image_h) line_end = image_h;

    Pixel color = {b, g, r};

    for (u32 x = 0; x < image_w; x++)
    {
        for (u32 y = line_start; y < line_end; y++)
        {

            pixels[y * image_w + x] = color;
        }
    }
}

void draw_angle_line (Pixel *pixels, u32 image_w, u32 image_h, u32 angle, u32 hex_color, u32 corner_x, u32 corner_y, u32 line_width)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);
    angle = angle%180;
    if (angle==0)
    {
        draw_horizontal_line (pixels, image_w, image_h, corner_y, hex_color, line_width);
        return;
    }
    if (angle==90)
    {
        draw_vertical_line (pixels, image_w, image_h, corner_x, hex_color, line_width);
        return;
    }

    double alfa = tan(angle*M_PI/180.0);

    Pixel color = {b, g, r};

    for (u32 x = 0; x < image_w; x++)
    {
        for (u32 y = 0; y < image_h; y++)
        {
            if(y >= corner_y + alfa*x - alfa*corner_x - line_width/2 && y<corner_y + alfa*x - alfa*corner_x + line_width/2 +line_width%2)

                pixels[y * image_w + x] = color;
        }
    }
}

void linear_bezier_curve (Pixel *pixels, u32 image_w, u32 image_h, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y,  u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};
    double step_t=1.0/sqrt((P0_x-P1_x)*(P0_x-P1_x)+(P0_y-P1_y)*(P0_y-P1_y));
    double t = 0;

    while (t<=1) {
        u32 x = (1.0-t)*P0_x+t*P1_x;
        u32 y = (1.0-t)*P0_y+t*P1_y;
        if (x < image_w && y < image_h)
        {
            pixels[y * image_w + x] = color;
        }
        t=t+step_t;
    }

}

void quadratic_bezier_curve (Pixel *pixels, u32 image_w, u32 image_h, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 P2_x, u32 P2_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};
    double segment1_lenth = sqrt((P0_x-P1_x)*(P0_x-P1_x)+(P0_y-P1_y)*(P0_y-P1_y));
    double segment2_lenth = sqrt((P2_x-P1_x)*(P2_x-P1_x)+(P2_y-P1_y)*(P2_y-P1_y));
    double step_t=1.0/(segment1_lenth+segment2_lenth);
    double t = 0;

    while (t<=1) {
        u32 x = (1.0-t)*(1.0-t)*P0_x+2*t*(1.0-t)*P1_x+t*t*P2_x;
        u32 y = (1.0-t)*(1.0-t)*P0_y+2*t*(1.0-t)*P1_y+t*t*P2_y;
        if (x < image_w && y < image_h)
        {
            pixels[y * image_w + x] = color;
        }
        t=t+step_t;
    }

}

void cubic_bezier_curve (Pixel *pixels, u32 image_w, u32 image_h, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 P2_x, u32 P2_y, u32 P3_x, u32 P3_y, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};
    double segment1_lenth = sqrt((P0_x-P1_x)*(P0_x-P1_x)+(P0_y-P1_y)*(P0_y-P1_y));
    double segment2_lenth = sqrt((P2_x-P1_x)*(P2_x-P1_x)+(P2_y-P1_y)*(P2_y-P1_y));
    double segment3_lenth = sqrt((P2_x-P3_x)*(P2_x-P3_x)+(P2_y-P3_y)*(P2_y-P3_y));
    double step_t=1.0/(segment1_lenth+segment2_lenth+segment3_lenth);
    double t = 0;

    while (t<=1) {
        u32 x = (1.0-t)*(1.0-t)*(1.0-t)*P0_x+3*t*(1.0-t)*(1.0-t)*P1_x+3*t*t*(1.0-t)*P2_x+t*t*t*P3_x;
        u32 y = (1.0-t)*(1.0-t)*(1.0-t)*P0_y+3*t*(1.0-t)*(1.0-t)*P1_y+3*t*t*(1.0-t)*P2_y+t*t*t*P3_y;
        if (x < image_w && y < image_h)
        {
            pixels[y * image_w + x] = color;
        }
        t=t+step_t;
    }

}




