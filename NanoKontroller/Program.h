/**
 * This file is part of NanoKontroller.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
 *
 * Program manager for running led effect functions.
*/

#ifndef PROGRAM_H
#define PROGRAM_H

#include "DebugLogger.h"  // provides LOGGER

/**
 * Program container class.
 *
 * Keeps track of program execution timestamps and stuff.
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

//TODO: Program selection interrupt handler functions

void setupProgMan();
void _program_stats(Program program);
void run_program_for_duration(Program program, unsigned long durationMillis);
void run_program_while(Program program, bool (*condition)());

#endif
