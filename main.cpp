#include "cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <iostream>

const int WINDOW_SCALE = 20;

SDL_Window *initWindow() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Window *window = SDL_CreateWindow("CHIP8 Emulator",  // window title
                                        64 * WINDOW_SCALE, // width, in pixels
                                        32 * WINDOW_SCALE, // height, in pixels
                                        SDL_WINDOW_OPENGL  // flags - see below
  );

  // Check that the window was successfully created
  if (window == NULL) {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                 SDL_GetError());
    return NULL;
  }
  return window;
}

int main(int argc, char *argv[]) {
  cpu cpu;
  bool running = true;

  SDL_Window *window = initWindow();
  if (window == NULL) {
    // Handle the error, already logged by createWindow
    SDL_Quit();
    return 1;
  }
  // SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  cpu.init();
  // TODO:: change to argv[1] after
  if (!cpu.loadRom("./tests/3-corax+.ch8")) {
    std::cout << "Error: Loading Rom" << std::endl;
    running = false;
  }

  /// if (argc != 2) {/   std::cout << "Enter the filename to open" <<
  /// std::endl;
  //   SDL_Quit();
  // }

  // TODO: Correct emulation loop

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case (SDL_EVENT_QUIT):
        running = false;
        break;
      case (SDL_EVENT_KEY_DOWN):
        switch (event.key.scancode) {
          {
          case (SDL_SCANCODE_1):
            cpu.executeCycle();
            if (cpu.draw) {
              cpu.drawGraphics();
              cpu.draw = false;
            }
            break;
          case (SDL_SCANCODE_2):
            std::cout << "2";
            break;
          case (SDL_SCANCODE_3):
            std::cout << "3";
            break;
          case (SDL_SCANCODE_4):
            break;
          case (SDL_SCANCODE_Q):
            break;
          case (SDL_SCANCODE_W):
            break;
          case (SDL_SCANCODE_E):
            break;
          case (SDL_SCANCODE_R):
            break;
          case (SDL_SCANCODE_A):
            break;
          case (SDL_SCANCODE_S):
            break;
          case (SDL_SCANCODE_D):
            break;
          case (SDL_SCANCODE_F):
            break;
          case (SDL_SCANCODE_Z):
            break;
          case (SDL_SCANCODE_X):
            break;
          case (SDL_SCANCODE_C):
            break;
          case (SDL_SCANCODE_V):
            break;
          default:
            break;
          }
        }
      }
    }
    cpu.executeCycle();

    if (cpu.draw) {
      cpu.drawGraphics();
      cpu.draw = false;
    }
  }
  // close window
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
