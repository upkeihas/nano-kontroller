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
    String Name;
    void (*program)();
    unsigned long beginMillis = 0;
    unsigned long durationMillis;
    unsigned int iters = 0;
    unsigned int Speed;
    Program(String name, void (*prog)(), unsigned int speed) {
      Name = name;
      program = prog;
      Speed = speed;
    }
    void Run() {
      iters++;
      if (beginMillis == 0) {
        beginMillis = millis();
      }
      program();
      durationMillis = (millis() - beginMillis);
    }
    void runWhile(bool (*condition)()) {
      while (condition()) {
        Run();
      }
    }
    void runDuration(unsigned long duration) {
      do {
        Run();
      } while (durationMillis < duration);
    }
};


void setupPrgMgr();
void belay(long period);
void _program_stats(Program program);
void run_program_for_duration(Program program, unsigned long durationMillis);
void run_program_until_interrupt(Program program);
void run_program_while(Program program, bool (*condition)());

void _programBreathe();
void _programFlash();
void _programKaTT();
void _programProgress();
void _programRunLed();
void _programStatic();


/*
  Led effect programs wrapped in Program container with typical example values.
*/
Program ProgramBreathe = Program("Breathing Leds", &_programBreathe, 60);
Program ProgramFlash = Program("Flash Leds", &_programFlash, 480);
Program ProgramKaTT = Program("Program KaTT", &_programKaTT, 120);
Program ProgramProgress = Program("Progress Through Leds", &_programProgress, 240);
Program ProgramRunLeds = Program("Run Through Leds", &_programRunLed, 60);
Program ProgramStatic = Program("Static Leds", &_programStatic, 100);
#endif
