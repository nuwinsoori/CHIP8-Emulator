#include "cpu.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>

#define OP_X ((opcode & 0x0F00) >> 8)
#define OP_Y ((opcode & 0x00F0) >> 4)
#define OP_N (opcode & 0x000F)
#define OP_NN (opcode & 0x00FF)
#define OP_NNN (opcode & 0x0FFF)

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

void cpu::init() {
  pc = 0x200;                        // Program Counter to start of program
  opcode = 0x00;                     // Reset current opcode
  I = 0;                             // Reset index register
  sp = 0;                            // Reset stack pointer
  memset(memory, 0, sizeof(memory)); // Clear memory
  memset(stack, 0, sizeof(stack));   // Clear stack
  memset(gfx, 0, sizeof(gfx));       // Reset display

  // Load font into memory
  for (int i = 0; i < 80; i++) {
    memory[i] = chip8_fontset[i];
  }

  draw = false;
}

bool cpu::loadRom(const char *romName) {
  int count = 0;
  FILE *rom = fopen(romName, "rb");
  if (!rom) {
    return false;
  }
  while (!feof(rom)) {
    fread(&memory[0x200 + count], 1, 1, rom);
    count++;
  }
  fclose(rom);

  // // DEBUG
  // for (int i = 0x200; i < 0x200 + 23; i += 2) {
  //   std::cout << std::hex << memory[i] << std::hex << memory[i + 1] << " ";
  // }
  // std::cout << std::endl;

  return true;
}

/*
00E0 (clear screen)
1NNN (jump)
6XNN (set register VX)
7XNN (add value to register VX)
ANNN (set index register I)
DXYN (display/draw)
*/

void cpu::executeCycle() {
  // fetch
  opcode = memory[pc] << 8 | memory[pc + 1];
  pc += 2;

  // decode & execute
  switch (opcode & 0xF000) {
  case (0x0000):
    switch (opcode & 0x000F) {
    case (0x0): // 00E0: Clear Screen
      // std::cout << "00e0 " << std::endl;
      memset(gfx, 0, 64 * 32);
      draw = true; // TODO::
      break;
    }
    break;
  case (0x1000): // 1NNN: jump to NNN
    // std::cout << "1nnn " << std::endl;
    pc = OP_NNN;
    break;
  case (0x6000): // 6XNN: set V[X] to NN;
    // std::cout << "6xnn " << std::endl;
    V[OP_X] = OP_NN;
    break;
  case (0x7000): // 7XNN: V[X] += NN
    // std::cout << "7xnn " << std::endl;
    V[OP_X] += OP_NN;
    break;
  case (0xA000): // ANNN: I = NNN
    // std::cout << "annn " << std::endl;
    I = OP_NNN;
    break;
  case (0xD000): // DXYN: Display
    // std::cout << "dxyn " << std::endl;
    unsigned short x = V[OP_X] % 64;
    unsigned short y = V[OP_Y] % 32;
    unsigned short n = OP_N;
    unsigned short pixel = 0;
    V[0xF] = 0;
    for (int height = 0; height < n; height++) {
      pixel = memory[I + height];
      for (int bit = 0; bit < 8; bit++) {
        if ((pixel & (0x80 >> bit)) != 0) {
          if (gfx[((x + bit) % 64 + ((y + height) % 32) * 64)] == 1) {
            V[0xF] = 1;
          }
          gfx[x + bit + ((y + height) * 64)] ^= 1;
        }
      }
    }

    draw = true;
    break;
  }
}

void cpu::drawGraphics() {
  std::cout << "\033[2J\033[H" << std::endl;
  int count = 0;
  for (int j = 0; j < 32; j++) {
    for (int i = 0; i < 64; i++) {
      if (gfx[count]) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
      count++;
    }
    std::cout << std::endl;
  }
  draw = false;
}
