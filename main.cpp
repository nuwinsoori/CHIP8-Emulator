#include "cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <iostream>

const int WINDOW_SCALE = 20;
const int IPF = 15;

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
  srand(time(0));
  cpu cpu;
  bool running = true;

  SDL_Window *window = initWindow();
  if (window == NULL) {
    // Handle the error, already logged by createWindow
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  SDL_Texture *texture = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
      64 * WINDOW_SCALE, 32 * WINDOW_SCALE);

  SDL_SetRenderTarget(renderer, texture);

  cpu.init();
  // TODO:: change to argv[1] after
  if (!cpu.loadRom("./tests/5-quirks.ch8")) {
    std::cout << "Error: Loading Rom" << std::endl;
    running = false;
  }

  /// if (argc != 2) {/   std::cout << "Enter the filename to open" <<
  /// std::endl;
  //   SDL_Quit();
  // }

  // TODO: Correct emulation loop

  while (running) {
    // Reset to black
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case (SDL_EVENT_QUIT):
        running = false;
        break;
      case (SDL_EVENT_KEY_DOWN): {
        for (int i = 0; i < 16; i++) {
        }
        switch (event.key.scancode) {
          {
          case (SDL_SCANCODE_1):
            cpu.keyDown(0x1);
            break;
          case (SDL_SCANCODE_2):
            cpu.keyDown(0x2);
            break;
          case (SDL_SCANCODE_3):
            cpu.keyDown(0x3);
            break;
          case (SDL_SCANCODE_4):
            cpu.keyDown(0xC);
            break;
          case (SDL_SCANCODE_Q):
            cpu.keyDown(0x4);
            break;
          case (SDL_SCANCODE_W):
            cpu.keyDown(0x5);
            break;
          case (SDL_SCANCODE_E):
            cpu.keyDown(0x6);
            break;
          case (SDL_SCANCODE_R):
            cpu.keyDown(0xD);
            break;
          case (SDL_SCANCODE_A):
            cpu.keyDown(0x7);
            break;
          case (SDL_SCANCODE_S):
            cpu.keyDown(0x8);
            break;
          case (SDL_SCANCODE_D):
            cpu.keyDown(0x9);
            break;
          case (SDL_SCANCODE_F):
            cpu.keyDown(0xE);
            break;
          case (SDL_SCANCODE_Z):
            cpu.keyDown(0xA);
            break;
          case (SDL_SCANCODE_X):
            cpu.keyDown(0x0);
            break;
          case (SDL_SCANCODE_C):
            cpu.keyDown(0xB);
            break;
          case (SDL_SCANCODE_V):
            cpu.keyDown(0xF);
            break;
          case (SDL_SCANCODE_N): {
            cpu.executeCycle();
            if (cpu.draw) {
              cpu.drawGraphics();
              cpu.draw = false;
            }
            break;
          }
          default:
            break;
          }
        }
        break;
      }
      case (SDL_EVENT_KEY_UP): {
        switch (event.key.scancode) {
        case (SDL_SCANCODE_1):
          cpu.keyUp(0x1);
          break;
        case (SDL_SCANCODE_2):
          cpu.keyUp(0x2);
          break;
        case (SDL_SCANCODE_3):
          cpu.keyUp(0x3);
          break;
        case (SDL_SCANCODE_4):
          cpu.keyUp(0xC);
          break;
        case (SDL_SCANCODE_Q):
          cpu.keyUp(0x4);
          break;
        case (SDL_SCANCODE_W):
          cpu.keyUp(0x5);
          break;
        case (SDL_SCANCODE_E):
          cpu.keyUp(0x6);
          break;
        case (SDL_SCANCODE_R):
          cpu.keyUp(0xD);
          break;
        case (SDL_SCANCODE_A):
          cpu.keyUp(0x7);
          break;
        case (SDL_SCANCODE_S):
          cpu.keyUp(0x8);
          break;
        case (SDL_SCANCODE_D):
          cpu.keyUp(0x9);
          break;
        case (SDL_SCANCODE_F):
          cpu.keyUp(0xE);
          break;
        case (SDL_SCANCODE_Z):
          cpu.keyUp(0xA);
          break;
        case (SDL_SCANCODE_X):
          cpu.keyUp(0x0);
          break;
        case (SDL_SCANCODE_C):
          cpu.keyUp(0xB);
          break;
        case (SDL_SCANCODE_V):
          cpu.keyUp(0xF);
          break;
        case (SDL_SCANCODE_P): {
          std::cout << std::hex << cpu.V[0] << std::endl;
        }
        default:
          break;
        }
        break;
      }
      }
    }

    if (argc == 1) {
      for (int i = 0; i < IPF; i++) {
        cpu.executeCycle();

        if (cpu.draw) {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          SDL_RenderClear(renderer);
          for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 64; x++) {
              if (cpu.gfx[x + (y * 64)]) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_FRect rect = {static_cast<float>(x * WINDOW_SCALE),
                                  static_cast<float>(y * WINDOW_SCALE),
                                  WINDOW_SCALE, WINDOW_SCALE};
                SDL_RenderFillRect(renderer, &rect);
              }
            }
          }
          SDL_SetRenderTarget(renderer, NULL);
          SDL_RenderPresent(renderer);
          cpu.draw = false;
          break;
        }
      }
    }

    cpu.timers();
    SDL_Delay(16);
  }

  // close window
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
