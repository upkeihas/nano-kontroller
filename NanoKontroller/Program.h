/**
 * This file is part of NanoKontroller.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
 *
 * Program manager for running led effect functions and a few simple example
 * programs.
 */

#ifndef PROGRAM_H
#define PROGRAM_H
#define PI 3.14159265359
#define TWO_PI 6.2831853072
#include "LED.h" // LEDs manipulation interface
#include <avr/pgmspace.h>

/**
 * Program container class.
 *
 * Keeps track of program execution state and such. Much important.
 * @param name String, Program name.
 * @param callback Function pointer, Actual program function reference.
 * @param speed uint16_t, Effect delay in millis, shorter is faster.
 * @param color optional, RGB, Effect color setting.
 *
 **/

class Program {
public:
  using Callback = void (*)();
  using CallbackWargs = void (*)(RGB);

  String Name;
  unsigned long beginMillis = 0;
  unsigned long durationMillis;
  uint16_t iters = 0;
  uint16_t Speed;
  RGB Color;

  // Program with default args
  Program(const String &name, Callback callback, uint16_t speed)
      : Name(name), Speed(speed), program(callback) {}

  // Program with extra RGB args
  Program(const String &name, CallbackWargs callback, uint16_t speed, RGB color)
      : Name(name), Speed(speed), program(nullptr), rgbProgram(callback),
        Color(color) {}

  void Run() {
    BRIGHTNESS_EVENT_PENDING = false;
    iters++;
    if (beginMillis == 0) {
      beginMillis = millis();
    }
    if (program != nullptr) {
      program();
    } else {
      rgbProgram(Color);
    }
    durationMillis = (millis() - beginMillis);
  }

  void runDuration(unsigned long duration) {
    do {
      Run();
    } while (durationMillis < duration);
  }

  void runWhile(bool (*condition)()) {
    while (condition()) {
      Run();
    }
  }
  void setColor(RGB color) { this->Color = color; }

private:
  Callback program;
  CallbackWargs rgbProgram;
};

// Sine wave intensity array for every 360 degrees of a circle
const uint8_t sineIntensity[360] PROGMEM = {
    129, 131, 134, 136, 138, 140, 143, 145, 147, 149, 151, 154, 156, 158, 160,
    162, 164, 166, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189, 191,
    193, 195, 196, 198, 200, 202, 204, 205, 207, 209, 211, 212, 214, 216, 217,
    219, 220, 222, 223, 225, 226, 227, 229, 230, 231, 233, 234, 235, 236, 237,
    239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250,
    251, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 255,
    254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 251, 251, 250,
    250, 249, 248, 248, 247, 246, 245, 244, 243, 243, 242, 241, 240, 239, 237,
    236, 235, 234, 233, 231, 230, 229, 227, 226, 225, 223, 222, 220, 219, 217,
    216, 214, 212, 211, 209, 207, 205, 204, 202, 200, 198, 196, 195, 193, 191,
    189, 187, 185, 183, 181, 179, 177, 175, 173, 171, 169, 166, 164, 162, 160,
    158, 156, 154, 151, 149, 147, 145, 143, 140, 138, 136, 134, 131, 129, 127,
    125, 123, 120, 118, 116, 114, 111, 109, 107, 105, 103, 100, 98,  96,  94,
    92,  90,  88,  85,  83,  81,  79,  77,  75,  73,  71,  69,  67,  65,  63,
    61,  59,  58,  56,  54,  52,  50,  49,  47,  45,  43,  42,  40,  38,  37,
    35,  34,  32,  31,  29,  28,  27,  25,  24,  23,  21,  20,  19,  18,  17,
    15,  14,  13,  12,  11,  11,  10,  9,   8,   7,   6,   6,   5,   4,   4,
    3,   3,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   3,   4,
    4,   5,   6,   6,   7,   8,   9,   10,  11,  11,  12,  13,  14,  15,  17,
    18,  19,  20,  21,  23,  24,  25,  27,  28,  29,  31,  32,  34,  35,  37,
    38,  40,  42,  43,  45,  47,  49,  50,  52,  54,  56,  58,  59,  61,  63,
    65,  67,  69,  71,  73,  75,  77,  79,  81,  83,  85,  88,  90,  92,  94,
    96,  98,  100, 103, 105, 107, 109, 111, 114, 116, 118, 120, 123, 125, 127};

void setupPrgMgr();
RGB captureColorPress(RGB &oldColor);
void programStats(Program &program);
void bblock();
void belay(long period);
void run_program_for_duration(Program program, unsigned long durationMillis);
void run_program_until_interrupt(Program program);
void run_program_while(Program program, bool (*condition)());

void _programBreathe();
void _programColorBlaster();
void _programColorFade();
void _programFlash();
void _programKaTT();
void _programStaticColor(RGB color);
void _programWave();

/*
  Led effect programs wrapped in Program container with typical example values.
*/
Program ProgramBreathe = Program("Breathing Leds", &_programBreathe, 24);
Program ProgramColorBlaster =
    Program("Color Blaster", &_programColorBlaster, 500);
Program ProgramColorFade =
    Program("Linear Color Fader", &_programColorFade, 24);
Program ProgramFlash = Program("Flash Leds", &_programFlash, 480);
Program ProgramKaTT = Program("Program KaTT", &_programKaTT, 120);
Program ProgramStaticColor =
    Program("Static Color Leds", &_programStaticColor, 240, WHITE);
Program ProgramWave = Program("Wave Generator Color Fader", &_programWave, 24);
#endif
