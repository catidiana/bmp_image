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

#define MAIN_WINDOW_INIT_WIDTH  900
#define MAIN_WINDOW_INIT_HEIGHT 600
#define M_PI 3.14159265358979323846

#include <SDL.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

typedef float    r32;
typedef double   r64;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef int16_t s32;
typedef int16_t s16;


struct V2 {
  r32 x, y;
};

union V3 {
  struct {u8 r, g, b;};
  struct {u8 x, y, z;};
};


struct Image {
  V3 *pixels;
  u32 w, h;
  u32 x, y;
  GLuint texture;
};


static V3
to_color (u32 hex_color)
{
  u8 r = (hex_color & 0xff0000) >> 16;
  u8 g = (hex_color & 0x00ff00) >> 8;
  u8 b = (hex_color & 0x0000ff);
  return {r, g, b};
}

#include "gradients.cpp"
#include "mandelbrot.cpp"
#include "figures.cpp"
#include "lines.cpp"


static void
set_window_transform (int window_w, int window_h)
{
  r32 w = 2.0f / window_w;
  r32 h = 2.0f / window_h;
  r32 x = 0;
  r32 y = 0;

  r32 transform[] = {
    w, 0, 0, 0,
    0, h, 0, 0,
    0, 0, 1, 0,
    x, y, 0, 1,
  };

  glLoadMatrixf (transform);
  glViewport (0, 0, window_w, window_h);
}


static Image
new_image (u32 w, u32 h, int x, int y)
{
  Image image = {};
  image.x = x;
  image.y = y;
  image.w = w;
  image.h = h;
  image.pixels = (V3 *) malloc (sizeof (V3) * image.w * image.h);

  glGenTextures (1, &image.texture);
  glBindTexture (GL_TEXTURE_2D, image.texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return image;
}


static void
update_image_texture (Image image)
{
  glBindTexture (GL_TEXTURE_2D, image.texture);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB8,
                image.w, image.h, 0,
                GL_RGB, GL_UNSIGNED_BYTE,
                image.pixels);
}


static void
show_image (Image image)
{
  int w = image.w / 2;
  int h = image.h / 2;

  int x0 = image.x - w;
  int x1 = image.x + w;
  int y0 = image.y - h;
  int y1 = image.y + h;

  glBindTexture (GL_TEXTURE_2D, image.texture);

  glBegin (GL_TRIANGLE_STRIP);

  glTexCoord2f (0, 0);
  glVertex2f (x0, y0);
  glTexCoord2f (1, 0);
  glVertex2f (x1, y0);
  glTexCoord2f (0, 1);
  glVertex2f (x0, y1);
  glTexCoord2f (1, 1);
  glVertex2f (x1, y1);

  glEnd ();
}


int
main (int argc, char **argv)
{
  SDL_Init (SDL_INIT_VIDEO);
  SDL_Window *main_window = SDL_CreateWindow ("Graphics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAIN_WINDOW_INIT_WIDTH, MAIN_WINDOW_INIT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  assert (main_window);
  SDL_GLContext glcontext = SDL_GL_CreateContext (main_window);
  assert (glcontext);

  set_window_transform (MAIN_WINDOW_INIT_WIDTH, MAIN_WINDOW_INIT_HEIGHT);

  size_t points_max = 1024;
  size_t points_count = 0;
  V2    *points = (V2 *) malloc (points_max * sizeof (*points));

  u32   images_count = 6;
  Image images[6] = {
    new_image (300, 300, -300,  150),
    new_image (300, 300,    0,  150),
    new_image (300, 300,  300,  150),
    new_image (300, 300, -300, -150),
    new_image (300, 300,    0, -150),
    new_image (300, 300,  300, -150),
  };

  glEnable (GL_TEXTURE_2D);
  glClearColor (0.20, 0.25, 0.30, 1.0);

  int window_w = MAIN_WINDOW_INIT_WIDTH;
  int window_h = MAIN_WINDOW_INIT_WIDTH;
  int mouse_screen_x = 0;
  int mouse_screen_y = 0;
  int mouse_pressed = 0;
  V2  mouse_pos = {};

  for (int keep_running = 1; keep_running; )
    {
      for (SDL_Event event; SDL_PollEvent (&event);)
        {
          switch (event.type)
            {
            case SDL_MOUSEMOTION:
              {
                mouse_screen_x = event.motion.x;
                mouse_screen_y = event.motion.y;
                mouse_pos.x =            (mouse_screen_x - window_w / 2);
                mouse_pos.y = window_h - (mouse_screen_y + window_h / 2);
              } break;
            case SDL_MOUSEBUTTONDOWN:
              {
                if (event.button.button == SDL_BUTTON_LEFT) mouse_pressed = 1;
              } break;
            case SDL_MOUSEBUTTONUP:
              {
                if (event.button.button == SDL_BUTTON_LEFT) mouse_pressed = 0;
              } break;
            case SDL_WINDOWEVENT:
              {
                switch (event.window.event)
                  {
                  case SDL_WINDOWEVENT_SIZE_CHANGED:
                    window_w = event.window.data1;
                    window_h = event.window.data2;
                    set_window_transform (window_w, window_h);
                    break;
                  }
              } break;
            case SDL_QUIT:
              {
                keep_running = 0;
                break;
              }
            }
        }

      if (mouse_pressed)
        {
          mouse_pressed = 0;
          V2 point;
          point.x = mouse_pos.x - images[2].x + images[2].w / 2;
          point.y = mouse_pos.y - images[2].y + images[2].h / 2;
          points[points_count++] = point;
        }

      glClear (GL_COLOR_BUFFER_BIT);

      static int x = 0;
      draw_gradient_tiles    (images[0], -mouse_screen_x, mouse_screen_y);
      draw_rectangle (images[0], 50, 50 + x % 100, x % 100, 40 + x % 100, 0xff0000 - x);
      draw_vertical_line (images[0], x%200, 0xff8f43, 10);
      draw_vertical_line (images[0], 30+x%170, 0x43bcff, 10);
      radial_gradient_fill   (images[1], 0x000000 + x, 0x00FFFF);
      draw_circle (images[1], 20 + x % 100, 200, 200, 0xff0000 - x);

      diagonal_gradient_fill    (images[2], 0x0000FF + x, 0x00FFFF - x);
      draw_equilateral_triangle (images[2], 70, 20 + x % 100, 50 + x % 100, 0xff0000 - x);

      for (size_t i = 0; i < points_count; ++i)
        {
          V2 point = points[i];
          draw_rectangle (images[2], 3, 3, point.x, point.y, 0xffffff);
        }

      for (size_t i = 1; i < points_count; ++i)
        {
          V2 p0 = points[i-1];
          V2 p1 = points[i];
          linear_bezier_curve (images[2], p0.x, p0.y, p1.x, p1.y, 0xffffff);
        }

      for (size_t i = 2; i < points_count; i+=2)
        {
          V2 p0 = points[i-2];
          V2 p1 = points[i-1];
          V2 p2 = points[i];
          quadratic_bezier_curve (images[2], p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, 0xffffff);
        }

      for (size_t i = 3; i < points_count; i+=3)
        {
          V2 p0 = points[i-3];
          V2 p1 = points[i-2];
          V2 p2 = points[i-1];
          V2 p3 = points[i];
          cubic_bezier_curve (images[2], p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0xffffff);
        }

      draw_gradient_tiles    (images[3], -x,  x);
      draw_ellipse (images[3], 10 + x % 100, 50+2 * x % 100, 150, 100, 0xff0000 - x);
      draw_angle_line (images[3], x, 0xff8f43, 150, 150, 15);

      uniform_fill    (images[4], 0x771111);
      /* draw_mandelbrot_convergence (images[4], 0xffaa00, 0x1730e5, 200,
                                         -20000 + mouse_screen_x - window_w / 2,
                                         -31000 - mouse_screen_y + window_h / 2,
                                         x / 2 % 200);*/
draw_julia_convergence (images[4], 0x0000ff, 0xffa000, 0.7885*cos(x*M_PI/180), 0.7885*sin(x*M_PI/180), 0.3, 150, 150);


      draw_gradient_tiles (images[5], -x, -x);
      quadratic_bezier_curve (images[5], x % 100, x % 100, 20 + (x % 100) *2, 30 + x % 100, 300 - x % 100, 300 - (x % 100) * 2, 0xff8f43);
      cubic_bezier_curve (images[5], 2 * (x % 100), 300 - x % 100, 30 + x % 100, 250 - 2 * (x % 100), 100 + (x % 100) /2 , 150 - (x % 100),  300, 20, 0x43bcff);

      ++x;

      for (u32 i = 0; i < images_count; ++i)
        {
          update_image_texture (images[i]);
          show_image           (images[i]);
        }

      // glBegin (GL_TRIANGLES);
      // glVertex2f (mouse_pos.x     , mouse_pos.y);
      // glVertex2f (mouse_pos.x + 10, mouse_pos.y);
      // glVertex2f (mouse_pos.x + 10, mouse_pos.y - 10);
      // glEnd ();

      SDL_GL_SwapWindow (main_window);
    }

  return 0;
}
