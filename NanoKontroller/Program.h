/**
 * This file is part of NanoKontroller.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
 *
 * Program manager for running led effect functions and a few simple example programs.
*/


#ifndef PROGRAM_H
#define PROGRAM_H
#define PI 3.14159265359
#define TWO_PI 6.2831853072
#include "LED.h"  // LEDs manipulation interface

/**
 * Program container class.
 *
 * Keeps track of program execution timestamps and stuff. Much important.
 * @param name String, Program name.
 * @param prog function pointer, Actual program function reference.
 * @param speed uint8_t, Delay in millis, shorter is faster.
 *
**/

class Program {
  public:
    using Callback = void (*)();
    using CallbackWargs = void (*)(uint8_t, uint8_t, uint8_t);

    String Name;
    unsigned long beginMillis = 0;
    unsigned long durationMillis;
    uint8_t iters = 0;
    uint8_t Speed;
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;

    // Program with default args
    Program(const String& name, Callback callback, uint8_t speed):
    Name(name), Speed(speed), program(callback) {}

    // Program with extra RGB args
    Program(const String& name, CallbackWargs callback, uint8_t speed, uint8_t r, uint8_t g, uint8_t b):
    Name(name), Speed(speed), program(nullptr), rgbProgram(callback), R(r), G(g), B(b) {}

    void Run() {
      iters++;
      if (beginMillis == 0) {
        beginMillis = millis();
      }
      if (program != nullptr) {
        program();
      } else {
        rgbProgram(R, G, B);
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
    void setColor(uint8_t r, uint8_t g, uint8_t b) {
      this->R=r;
      this->G=g;
      this->B=b;
    }
  private:
    Callback program;
    CallbackWargs rgbProgram;
};


void setupPrgMgr();
void belay(long period);
void _program_stats(Program program);
void run_program_for_duration(Program program, unsigned long durationMillis);
void run_program_until_interrupt(Program program);
void run_program_while(Program program, bool (*condition)());

void _programBreathe();
void _programColorBlaster();
void _programFlash();
void _programKaTT();
void _programProgress();
void _programRunLed();
void _programStaticColor(uint8_t r, uint8_t g, uint8_t b);
void _programWave();
void _altProgramWave();


/*
  Led effect programs wrapped in Program container with typical example values.
*/
Program ProgramBreathe = Program("Breathing Leds", &_programBreathe, 60);
Program ProgramColorBlaster = Program("Color Blaster", &_programColorBlaster, 500);
Program ProgramFlash = Program("Flash Leds", &_programFlash, 480);
Program ProgramKaTT = Program("Program KaTT", &_programKaTT, 120);
Program ProgramProgress = Program("Progress Through Leds", &_programProgress, 240);
Program ProgramStaticColor = Program("Static color Leds", &_programStaticColor, 240, 0, 0, 0);
Program ProgramWave = Program("Wave generator program", &_programWave, 12);
#endif
