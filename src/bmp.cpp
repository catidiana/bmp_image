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

#define MAIN_WINDOW_INIT_WIDTH  400
#define MAIN_WINDOW_INIT_HEIGHT 400

#include <SDL.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


typedef float    r32;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef int16_t s16;

#pragma pack(push, 1)
union V3 {
  struct {u8 r, g, b;};
  struct {u8 x, y, z;};
};
#pragma pack(pop)


#include "gradients.cpp"


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


static void
update_texture (GLuint texture, u32 image_w, u32 image_h, V3 *image)
{
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB8,
                image_w, image_h, 0,
                GL_RGB, GL_UNSIGNED_BYTE,
                image);
}


static void
show_image (GLuint texture, u32 image_w, u32 image_h)
{
  int w = image_w / 2;
  int h = image_h / 2;

  glColor3f (1, 1, 1);

  glBegin (GL_TRIANGLE_STRIP);
  glTexCoord2f (1, 1);
  glVertex2f ( w, -h);
  glTexCoord2f (0, 1);
  glVertex2f (-w, -h);
  glTexCoord2f (1, 0);
  glVertex2f ( w,  h);
  glTexCoord2f (0, 0);
  glVertex2f (-w,  h);
  glEnd ();
}


int
main()
{
  SDL_Init (SDL_INIT_VIDEO);
  SDL_Window *main_window = SDL_CreateWindow ("Graphics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAIN_WINDOW_INIT_WIDTH, MAIN_WINDOW_INIT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  assert (main_window);
  SDL_GLContext glcontext = SDL_GL_CreateContext (main_window);
  assert (glcontext);

  u32 image_w = 400;
  u32 image_h = 400;
  V3 *image = (V3 *) malloc (sizeof (V3) * image_w * image_h);

  GLuint texture;
  glGenTextures (1, &texture);
  glBindTexture (GL_TEXTURE_2D, texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glEnable (GL_TEXTURE_2D);
  glClearColor (0.20, 0.25, 0.30, 1.0);

  for (int keep_running = 1; keep_running; )
    {
      for (SDL_Event event; SDL_PollEvent (&event);)
        {
          switch (event.type)
            {
            case SDL_WINDOWEVENT:
              {
                switch (event.window.event)
                  {
                  case SDL_WINDOWEVENT_SIZE_CHANGED:
                    int window_w = event.window.data1;
                    int window_h = event.window.data2;
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

      // static u32 x_offset = 0;
      // static u32 y_offset = 0;
      // V3 pixel = {};
      // for (u32 y = 0; y < image_h; ++y)
      //   {
      //     pixel.b = y + y_offset;
      //     for (u32 x = 0; x < image_w; ++x)
      //       {
      //         pixel.g = x + x_offset;
      //         image[image_w * y + x] = pixel;
      //       }
      //   }
      // ++x_offset;
      // ++y_offset;

      radial_gradient_fill (image, image_w, image_h, 0x000000, 0x00FFFF);

      update_texture (texture, image_w, image_h, image);
      show_image     (texture, image_w, image_h);

      SDL_GL_SwapWindow (main_window);
    }

  return 0;
}
