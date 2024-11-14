#include "cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdio.h>
#include <stdlib.h>

const int WINDOW_SCALE = 20;

unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initCpu(cpu cpu) {
  // Stack pointer to start of program
  cpu.sp = 0x200;

  // Load font into memory
  for (int i = 0; i < 80; i++) {
    cpu.memory[i] = chip8_fontset[i];
  }
  cpu.delay_timer = 0;
  cpu.sound_timer = 0;
}

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

  /* if (argc != 2) { */
  /*   printf("Enter the filename to open"); */
  /*   SDL_Quit(); */
  /* } */

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
