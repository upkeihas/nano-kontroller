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

const int R_1 = 3;
const int G_1 = 5;
const int B_1 = 6;
const int R_2 = 9;
const int G_2 = 10;
const int B_2 = 11;
const int nLedPins = 6;
const int ledPins[nLedPins] = {R_1, G_1, B_1, R_2, G_2, B_2};


// Increment step to change brightness and effect speed.
const unsigned int ADJUST_STEP = 5;
// Maximum brightness for leds.
const unsigned int LED_MAX_INTENSITY = 255;
const unsigned int LED_OFF = 0;

/**
  Current effect intensity.
  Value gets updated externally via callback function.
  Optimally divisible by ADJUST_STEP.
*/
extern unsigned int LED_BRIGHTNESS = 130;

// Current effect minimum intensity. Optimally divisible by ADJUST_STEP
extern unsigned int LED_MIN_BRIGHTNESS = 5;

/**
  Default effect speed for currently running program.
  Value gets updated externally via callback function
  and by running programs.
  Optimally divisible by ADJUST_STEP.
*/
extern unsigned int LED_DELAYMILLIS = 100;


void setupLeds();
unsigned int adjustColorBrightness(unsigned int c);
void setLedDelayMillis(unsigned long speed);

void InterruptSpeedUp();
void InterruptSpeedDown();
void InterruptBriteUp();
void InterruptBriteDown();

void forEachLed(unsigned int brightness);
void ledsOff();
void setLed(int led, unsigned int brightness);

#endif
