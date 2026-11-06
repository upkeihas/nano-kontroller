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

// Container for RGB color codes
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// Increment step to change brightness and effect speed.
const uint8_t ADJUST_STEP = 5;
// Maximum brightness for leds.
const uint8_t LED_MAX_INTENSITY = 255;
const uint8_t LED_OFF = 0;

// Signal our program that brightness setting just changed
volatile bool BRIGHTNESS_EVENT_PENDING = false;

/**
  Current effect intensity.
  Value gets updated externally via callback function.
  Optimally divisible by ADJUST_STEP.
*/
volatile uint8_t LED_BRIGHTNESS = 130;

// Current effect minimum intensity. Optimally divisible by ADJUST_STEP
const uint8_t LED_MIN_BRIGHTNESS = 5;

/**
  Default effect speed for currently running program.
  Value gets updated externally via callback function
  and by running programs.
  Optimally divisible by ADJUST_STEP.
*/
volatile uint16_t LED_DELAYMILLIS = 100;


void setupLeds();
uint8_t adjustColorBrightness(uint8_t c);
void setLedDelayMillis(uint16_t speed);

void InterruptSpeedUp();
void InterruptSpeedDown();
void InterruptBriteUp();
void InterruptBriteDown();

void forEachLed(uint8_t brightness);
void ledsOff();
void setLed(int led, uint8_t brightness);
void setColor(RGB color);


const RGB RED = {255, 0, 0};
const RGB GREEN = {0, 255, 0};
const RGB BLUE = {0, 0, 255};
const RGB WHITE = {255, 255, 255};
const RGB VERMILLION = {227, 66, 52};
const RGB MINT = {173, 235, 179};
const RGB DBLUE = {6, 6, 235};
const RGB ORANGE = {255, 165, 0};
const RGB TEAL = {0, 128, 128};
const RGB VIOLET = {130, 12, 130};
const RGB CARROT = {237, 145, 33};
const RGB MBLUE = {25, 24, 208};
const RGB INDIGO = {75, 0, 130};
const RGB YELLOW = {235, 235, 0};
const RGB TIBER = {0, 51, 51};
const RGB PURPLE = {255, 0, 255};


#endif
