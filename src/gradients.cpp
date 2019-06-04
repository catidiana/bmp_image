//fill functions

//put HEX color code like 0x000000
void uniform_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color= 0xFFFFFF);
//shift - number from 0 to 1 that shift gradient, making part of the file one color, squeezing gradient
void linear_vertical_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000, float shift = 0);
void linear_horizontal_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000, float shift = 0);
void diagonal_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000);
void radial_gradient_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color1 = 0xFFFFFF, u32 hex_color2 = 0x000000);


void uniform_fill (Pixel *pixels, u32 image_w, u32 image_h, u32 hex_color)
{
    u8 r = (hex_color & 0xff0000) >> 16;
    u8 g = (hex_color & 0x00ff00) >> 8;
    u8 b = (hex_color & 0x0000ff);

    Pixel color = {b, g, r};

    for (u32 y = 0; y < image_h; y++)
    {
        for (u32 x = 0; x < image_w; x++)
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
