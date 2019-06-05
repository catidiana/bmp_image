
typedef int32_t  s32;

//scale scales figure, keep it from 0 to 2; shift not more than image size; can shift to negative direction a bit
void draw_Mandelbrot (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color = 0xFFFFFF, float scale = 1.0, s32 shift_x = 0, s32 shift_y = 0);

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
            float cor_x = 0.0;
            float cor_y = 0.0;
            float x_c = (float)x/(image_wid*scale);
            float y_c = (float)y/(image_hei*scale);
            u32 steps = 0;

            while (1) {
                float x_test = cor_x;
                float y_test = cor_y;
                cor_x = x_test*x_test - y_test*y_test + x_c;
                cor_y = 2*x_test*y_test + y_c;
                if (cor_x>2 || cor_x<-2|| cor_y>2 || cor_y<-2)
                {break;}
                steps++;
                if (steps>20)
                {
                    pixels[(y+shift_y) * image_wid + x+shift_x] = color;
                    break;
                }
            }
        }
    }

}
