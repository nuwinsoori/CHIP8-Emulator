#pragma once
// unsigned short 2bytes
// unsigned char 1byte
class cpu {
private:
  unsigned short opcode;
  unsigned char memory[4096];
  unsigned short I;          // Index Register
  unsigned short pc;         // Program Counter
  unsigned char delay_timer; // used for events (counts down at 60Hz)
  unsigned char sound_timer; // plays sound when >0 (counts down at 60Hz)
  unsigned short stack[16];
  unsigned short sp; // Stack Pointer

public:
  unsigned char gfx[64 * 32]; // Pixel State (graphics)
  unsigned char key[16];      // State of keys 0-F
  unsigned char prevKeys[16]; // State of keys in previous frame
  bool running;
  void init();
  bool loadRom(const char *romName);
  void executeCycle();
  bool breakIPF;
  bool draw;
  void keyDown(int pressedKey);
  void keyUp(int pressedKey);
  bool timers();
  unsigned char V[16]; // Registers V0-VE
};
