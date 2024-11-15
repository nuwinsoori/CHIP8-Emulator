#include "cpu.h"
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>

#define OP_X (opcode & 0x0F00)
#define OP_Y (opcode & 0x00F0)
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

  // DEBUG
  for (int i = 0x200; i < 0x200 + 23; i++) {
    std::cout << std::hex << memory[i] << " ";
  }

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
  opcode = memory[pc];
  opcode <<= 8;
  opcode |= memory[pc + 1];
  pc += 2;

  // decode & execute
  switch (opcode & 0xF000) {
  case (0x0):
    switch (opcode & 0x000F) {
    case (0x0): // 00E0: Clear Screen
      memset(gfx, 0, 64 * 32);
      break;
    }
  case (0x1): // 1NNN: jump to NNN
    pc = OP_NNN;
    break;
  case (0x6): // 6XNNN: set V[X] to NN;
    V[OP_X] = OP_NNN;
    break;
  case (0x7): // 7XNN: V[X] += NN
    V[OP_X] += OP_NN;
    break;
  case (0xA): // ANNN: I = NNN
    I = OP_NNN;
    break;
  case (0xD): // DXYN: Display
    break;
  }
}

void cpu::drawGraphics() {
  system("clear");
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
