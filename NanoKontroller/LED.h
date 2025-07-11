/**
 * This file is part of NanoKontroller.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
 *
 * Led controller to manage current state of configured LED pins,
 * provide basic led manipulation calls and to handle user interrupts.
*/

#ifndef LED_H
#define LED_H

#include "DebugLogger.h"  // provides LOGGER
#include "Utils.h"

const int ledPins[6] = {3, 5, 6, 9, 10, 11};
const int nLedPins = 6;

/**
  Default effect maximum intensity.
  Value gets updated externally via callback function.
*/
extern byte LED_MAXBRITE = 127;

// Default effect minimum intensity. This just might be useless.
extern byte LED_MINBRITE = 0;

/**
  Default effect speed for currently running program.
  Value gets updated externally via callback function.
*/
extern unsigned int LED_DELAYMILLIS = 100;


void setupLeds();
void setLedDelayMillis(unsigned long speed);

void InterruptSpeedUp();
void InterruptSpeedDown();
void InterruptBriteUp();
void InterruptBriteDown();

void forEachLed(int brightness);
void ledsOff();
void setLed(int led, int brightness);

void _programFlash();
void _programKaTT();
void _programProgress();
void _programRunLed();


#endif
