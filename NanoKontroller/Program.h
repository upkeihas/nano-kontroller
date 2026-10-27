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
#include "LED.h"  // LEDs manipulation interface

/**
 * Program container class.
 *
 * Keeps track of program execution timestamps and stuff. Much important.
 * @param name String, Program name.
 * @param prog function pointer, Actual program function reference.
 * @param speed unsigned int, Delay in millis, shorter is faster.
 * 
**/

class Program {
  public:
    using Callback = void (*)();
    using CallbackWargs = void (*)(unsigned int, unsigned int, unsigned int);

    String Name;
    unsigned long beginMillis = 0;
    unsigned long durationMillis;
    unsigned int iters = 0;
    unsigned int Speed;
    unsigned int R = 0;
    unsigned int G = 0;
    unsigned int B = 0;

    // Program with default args
    Program(const String& name, Callback callback, unsigned int speed):
    Name(name), Speed(speed), program(callback) {}

    // Program with extra RGB args
    Program(const String& name, CallbackWargs callback, unsigned int speed, unsigned int r, unsigned int g, unsigned int b):
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
    void setColor(unsigned int r, unsigned int g, unsigned int b) {
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
void _programStaticColor(unsigned int r, unsigned int g, unsigned int b);


/*
  Led effect programs wrapped in Program container with typical example values.
*/
Program ProgramBreathe = Program("Breathing Leds", &_programBreathe, 60);
Program ProgramColorBlaster = Program("Color Blaster", &_programColorBlaster, 500);
Program ProgramFlash = Program("Flash Leds", &_programFlash, 480);
Program ProgramKaTT = Program("Program KaTT", &_programKaTT, 120);
Program ProgramProgress = Program("Progress Through Leds", &_programProgress, 240);
Program ProgramRunLeds = Program("Run Through Leds", &_programRunLed, 60);
Program ProgramStaticColor = Program("Static color Leds", &_programStaticColor, 240, 0, 0, 0);
#endif
