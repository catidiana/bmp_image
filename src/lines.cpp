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
draw_vertical_line (Image image, u32 line_position_x, u32 hex_color = 0x000000, u32 line_width = 1)
{
    V3 color = to_color (hex_color);

    u32 line_start;
    if (line_position_x<line_width/2) line_start=0;
    else line_start = line_position_x-line_width/2;
    if (line_start>image.w) return;
    u32 line_end = line_position_x+line_width/2+line_width%2;
    if (line_end>image.w) line_end = image.w;

    for (u32 y = 0; y < image.h; y++)
    {
        for (u32 x = line_start; x < line_end; x++)
        {

            image.pixels[y * image.w + x] = color;
        }
    }
}


static void
draw_horizontal_line (Image image, u32 line_position_y, u32 hex_color = 0x000000, u32 line_width = 1)
{
    V3 color = to_color (hex_color);

    u32 line_start;
    if (line_position_y<line_width/2) line_start=0;
    else line_start = line_position_y-line_width/2;
    if (line_start>image.h) return;
    u32 line_end = line_position_y+line_width/2+line_width%2;
    if (line_end>image.h) line_end = image.h;

    for (u32 x = 0; x < image.w; x++)
    {
        for (u32 y = line_start; y < line_end; y++)
        {

            image.pixels[y * image.w + x] = color;
        }
    }
}


//put angle in degrees; corner_x corner_y - coordinates of the dot that should belong to line
static void
draw_angle_line (Image image, u32 angle = 45, u32 hex_color = 0x000000, u32 corner_x = 0, u32 corner_y = 0, u32 line_width = 1)
{
    V3 color = to_color (hex_color);

    angle = angle%180;
    if (angle==0)
    {
        draw_horizontal_line (image, corner_y, hex_color, line_width);
        return;
    }
    if (angle==90)
    {
        draw_vertical_line (image, corner_x, hex_color, line_width);
        return;
    }

    double alfa = tan(angle*M_PI/180.0);


    for (u32 x = 0; x < image.w; x++)
    {
        for (u32 y = 0; y < image.h; y++)
        {
            if(y >= corner_y + alfa*x - alfa*corner_x - line_width/2 && y<corner_y + alfa*x - alfa*corner_x + line_width/2 +line_width%2)

                image.pixels[y * image.w + x] = color;
        }
    }
}


//build smooth path based on segment P0(x,y) - P1(x,y)
static void
linear_bezier_curve (Image image, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);

    double step_t=1.0/sqrt((P0_x-P1_x)*(P0_x-P1_x)+(P0_y-P1_y)*(P0_y-P1_y));
    double t = 0;

    while (t<=1) {
        u32 x = (1.0-t)*P0_x+t*P1_x;
        u32 y = (1.0-t)*P0_y+t*P1_y;
        if ((x>=0 && x < image.w) && (y>=0 && y < image.h))
        {
            image.pixels[y * image.w + x] = color;
        }
        t=t+step_t;
    }

}


//build smooth path based on segments P0(x,y) - P1(x,y) - P2(x,y)
static void
quadratic_bezier_curve (Image image, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 P2_x, u32 P2_y, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);

    double segment1_lenth = sqrt((P0_x-P1_x)*(P0_x-P1_x)+(P0_y-P1_y)*(P0_y-P1_y));
    double segment2_lenth = sqrt((P2_x-P1_x)*(P2_x-P1_x)+(P2_y-P1_y)*(P2_y-P1_y));
    double step_t=1.0/(segment1_lenth+segment2_lenth);
    double t = 0;

    while (t<=1) {
        u32 x = (1.0-t)*(1.0-t)*P0_x+2*t*(1.0-t)*P1_x+t*t*P2_x;
        u32 y = (1.0-t)*(1.0-t)*P0_y+2*t*(1.0-t)*P1_y+t*t*P2_y;
        if ((x>=0 && x < image.w) && (y>=0 && y < image.h))
        {
            image.pixels[y * image.w + x] = color;
        }
        t=t+step_t;
    }

}


//build smooth path based on segments P0(x,y) - P1(x,y) - P2(x,y) - P3 (x,y)
static void
cubic_bezier_curve (Image image, u32 P0_x, u32 P0_y, u32 P1_x, u32 P1_y, u32 P2_x, u32 P2_y, u32 P3_x, u32 P3_y, u32 hex_color = 0x000000)
{
    V3 color = to_color (hex_color);

    double segment1_lenth = sqrt((P0_x-P1_x)*(P0_x-P1_x)+(P0_y-P1_y)*(P0_y-P1_y));
    double segment2_lenth = sqrt((P2_x-P1_x)*(P2_x-P1_x)+(P2_y-P1_y)*(P2_y-P1_y));
    double segment3_lenth = sqrt((P2_x-P3_x)*(P2_x-P3_x)+(P2_y-P3_y)*(P2_y-P3_y));
    double step_t=1.0/(segment1_lenth+segment2_lenth+segment3_lenth);
    double t = 0;

    while (t<=1) {
        u32 x = (1.0-t)*(1.0-t)*(1.0-t)*P0_x+3*t*(1.0-t)*(1.0-t)*P1_x+3*t*t*(1.0-t)*P2_x+t*t*t*P3_x;
        u32 y = (1.0-t)*(1.0-t)*(1.0-t)*P0_y+3*t*(1.0-t)*(1.0-t)*P1_y+3*t*t*(1.0-t)*P2_y+t*t*t*P3_y;
        if ((x>=0 && x < image.w) && (y>=0 && y < image.h))
        {
            image.pixels[y * image.w + x] = color;
        }
        t=t+step_t;
    }

}




