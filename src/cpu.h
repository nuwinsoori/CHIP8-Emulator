#pragma once
// unsigned short 2bytes
// unsigned char 1byte
#include <cstdint>

class cpu {
private:
  uint16_t opcode;
  uint8_t memory[4096];
  uint16_t I;          // Index Register
  uint16_t pc;         // Program Counter
  uint8_t delay_timer; // used for events (counts down at 60Hz)
  uint8_t sound_timer; // plays sound when >0 (counts down at 60Hz)
  uint16_t stack[16];
  uint16_t sp; // Stack Pointer

public:
  uint8_t gfx[64 * 32]; // Pixel State (graphics)
  uint8_t key[16];      // State of keys 0-F
  uint8_t prevKeys[16]; // State of keys in previous frame
  bool running;
  void init();
  bool loadRom(const char *romName);
  void executeCycle();
  bool breakIPF;
  bool draw;
  void keyDown(int pressedKey);
  void keyUp(int pressedKey);
  bool timers();
  uint8_t V[16]; // Registers V0-VE
};
