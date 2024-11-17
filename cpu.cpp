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
  case (0x0000): {
    switch (opcode & 0x000F) {
    case (0x0): // 00E0: Clear Screen
      std::memset(gfx, 0, 64 * 32);
      draw = true;
      break;
    case (0xE): // 00EE: return from subroutine
      sp--;
      pc = stack[sp];
      break;
    }
    break;
  }
  case (0x1000): // 1NNN: jump to NNN
  {
    pc = OP_NNN;
    break;
  }
  case (0x2000): // 2NNN: jump to NNN, push PC to stack
  {
    stack[sp] = pc;
    sp++;
    pc = OP_NNN;
    break;
  }
  case (0x3000): // 3XNN: skip instruction if VX == NN
  {
    if (V[OP_X] == OP_NN) {
      pc += 2;
    }
    break;
  }

  case (0x4000): // 4XNN: skip instruction if VX != NN
  {
    if (V[OP_X] != OP_NN) {
      pc += 2;
    }
    break;
  }

  case (0x5000): // 5XY0: skip instruction if VX == VY
  {
    if (V[OP_X] == V[OP_Y]) {
      pc += 2;
    }
  }
  case (0x6000): // 6XNN: set V[X] to NN;
  {
    V[OP_X] = OP_NN;
    break;
  }
  case (0x7000): // 7XNN: V[X] += NNk
  {
    V[OP_X] += OP_NN;
    break;
  }

  case (0x8000): // Logic functions
  {
    switch (opcode & 0x000F) {
    case (0x0): // 8XY0: VX = VY
    {
      V[OP_X] = V[OP_Y];
      break;
    }
    case (0x1): // 8XY1: VX |= VY
    {
      V[OP_X] |= V[OP_Y];
      break;
    }
    case (0x2): // 8XY2: VX &= VY
    {
      V[OP_X] &= V[OP_Y];
      break;
    }
    case (0x3): // 8XY3: VX ^= VY
    {
      V[OP_X] ^= V[OP_Y];
      break;
    }

    case (0x4): // 8XY4: VX += VY (carry flag VF)
    {
      V[0xF] = 0;
      V[OP_X] += V[OP_Y];
      if (V[OP_X] > 255) {
        V[0xF] = 1;
      }
      break;
    }

    case (0x5): // 8XY5: VX -= VY
    {
      V[0xF] = (V[OP_X] > V[OP_Y]) ? 1 : 0;
      V[OP_X] -= V[OP_Y];
      break;
    }
    case (0x7): // 8XY7: VX = VY - VX
    {
      V[0xF] = (V[OP_Y] > V[OP_X]) ? 1 : 0;
      V[OP_X] = V[OP_Y] - V[OP_X];
      break;
    }

    case (0x6): // AMBIGUOUS! 8XY6: VX = VY, VX >> 1, VF = bitshifted num
    {
      V[OP_X] = V[OP_Y];
      V[0xF] = (V[OP_X] & 0b1);
      V[OP_X] >>= 1;
      break;
    }
    case (0xE): // AMBIGUOUS! 8XYE: VX = VY, VX << 1, VF = bitshifted num
    {
      V[OP_X] = V[OP_Y];
      V[0xF] = (((V[OP_X] & 0x80) >> 7) & 0b1);
      V[OP_X] <<= 1;
      break;
    }
    }
    break;
  }

  case (0xA000): // ANNN: I = NNN
  {
    I = OP_NNN;
    break;
  }

  case (0xD000): // DXYN: Display  V[X] = xpos, V[Y] = ypos, N = height
  {
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

  case (0xF000): {
    switch (opcode & 0x00FF) {
    case (0x33): { // FX33: Binary-coded decimal conversion
      int hundreds = 0, tens = 0, ones = 0;
      int number = V[OP_X];
      hundreds = number / 100;
      tens = (number / 10) % 10;
      ones = number % 10;
      memory[I] = hundreds;
      memory[I + 1] = tens;
      memory[I + 2] = ones;
      break;
    }
    case (0x55): { // FX55: store memory from V0 to VX
      for (int i = 0; i <= OP_X; i++) {
        memory[I + i] = V[i];
      }
      break;
    }
    case (0x65): { // FX65: store memory from V0 to VX
      for (int i = 0; i <= OP_X; i++) {
        V[i] = memory[I + i];
      }
      break;
    }
    case (0x1E): {
      I += V[OP_X];
      break;
    }
    }
    break;
  }
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
