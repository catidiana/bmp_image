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


#pragma pack(push, 1)
union V3 {
  struct {u8 r, g, b;};
  struct {u8 x, y, z;};
};
#pragma pack(pop)


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
  int mouse_x = 0;
  int mouse_y = 0;

  for (int keep_running = 1; keep_running; )
    {
      for (SDL_Event event; SDL_PollEvent (&event);)
        {
          switch (event.type)
            {
            case SDL_MOUSEMOTION:
              {
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
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

      glClear (GL_COLOR_BUFFER_BIT);

      static int x = 0;
      draw_gradient_tiles    (images[0], -mouse_x, mouse_y);
      radial_gradient_fill   (images[1], 0x000000 + x, 0x00FFFF);
      diagonal_gradient_fill (images[2], 0x0000FF + x, 0x00FFFF - x);
      draw_gradient_tiles    (images[3], -x,  x);

      uniform_fill    (images[4], 0x771111);
      draw_mandelbrot_convergence (images[4], 0xffaa00, 0x1730e5, 0.38,
                                   240 + mouse_x - window_w / 2,
                                   150 - mouse_y + window_h / 2,
                                   x / 2 % 20);

      draw_gradient_tiles (images[5], -x, -x);

      ++x;

      for (u32 i = 0; i < images_count; ++i)
        {
          update_image_texture (images[i]);
          show_image           (images[i]);
        }

      SDL_GL_SwapWindow (main_window);
    }

  return 0;
}
