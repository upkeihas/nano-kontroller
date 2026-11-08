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
#include "LED.h" // LEDs manipulation interface

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

class Program
{
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
  Program(const String &name, Callback callback, uint16_t speed) : Name(name), Speed(speed), program(callback) {}

  // Program with extra RGB args
  Program(const String &name, CallbackWargs callback, uint16_t speed, RGB color) : Name(name), Speed(speed), program(nullptr), rgbProgram(callback), Color(color) {}

  void Run()
  {
    BRIGHTNESS_EVENT_PENDING = false;
    iters++;
    if (beginMillis == 0)
    {
      beginMillis = millis();
    }
    if (program != nullptr)
    {
      program();
    }
    else
    {
      rgbProgram(Color);
    }
    durationMillis = (millis() - beginMillis);
  }

  void runDuration(unsigned long duration)
  {
    do
    {
      Run();
    } while (durationMillis < duration);
  }

  void runWhile(bool (*condition)())
  {
    while (condition())
    {
      Run();
    }
  }
  void setColor(RGB color)
  {
    this->Color = color;
  }

private:
  Callback program;
  CallbackWargs rgbProgram;
};

void setupPrgMgr();
void bblock();
void belay(long period);
RGB _captureColorPress(RGB &oldColor);
void _program_stats(Program program);
void run_program_for_duration(Program program, unsigned long durationMillis);
void run_program_until_interrupt(Program program);
void run_program_while(Program program, bool (*condition)());

void _programBreathe();
void _programColorBlaster();
void _programColorFade();
void _programFlash();
void _programKaTT();
void _programProgress();
void _programRunLed();
void _programStaticColor(RGB color);
void _programWave();

/*
  Led effect programs wrapped in Program container with typical example values.
*/
Program ProgramBreathe = Program("Breathing Leds", &_programBreathe, 60);
Program ProgramColorBlaster = Program("Color Blaster", &_programColorBlaster, 500);
Program ProgramColorFade = Program("Linear Color Fader", &_programColorFade, 24);
Program ProgramFlash = Program("Flash Leds", &_programFlash, 480);
Program ProgramKaTT = Program("Program KaTT", &_programKaTT, 120);
Program ProgramProgress = Program("Progress Through Leds", &_programProgress, 240);
Program ProgramStaticColor = Program("Static Color Leds", &_programStaticColor, 240, WHITE);
Program ProgramWave = Program("Wave Generator Color Fader", &_programWave, 12);
#endif
