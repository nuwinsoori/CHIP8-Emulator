#pragma once
// unsigned short 2bytes
// unsigned char 1byte
class cpu {
private:
  unsigned short opcode;
  unsigned memory[4096];
  unsigned char V[16];        // Registers V0-VE
  unsigned short I;           // Index Register
  unsigned short pc;          // Program Counter
  unsigned char gfx[64 * 32]; // Pixel State (graphics)

  unsigned char delay_timer; // used for events (counts down at 60Hz)
  unsigned char sound_timer; // plays sound when >0 (counts down at 60Hz)

  unsigned short stack[16];
  unsigned short sp;     // Stack Pointer
  unsigned char key[16]; // State of keys 0-F

  char rom;

public:
  void init();
};
