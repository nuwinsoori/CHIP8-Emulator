#include "cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
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
  cpu.init();
  bool running = true;

  SDL_Window *window = initWindow();
  if (window == NULL) {
    // Handle the error, already logged by createWindow
    SDL_Quit();
    return 1;
  }

  // if (argc != 2) {
  //   std::cout << "Enter the filename to open" << std::endl;
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
      }
    }
  }

  // close window
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
