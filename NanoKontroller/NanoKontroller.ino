/**
 * NanoKontroller, IR remote controllable 6 channel LED strip driver made for Arduino NANO/Uno.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
*/

#define DEBUG  // Enable debug prints to USB serial for logging
#include "DebugLogger.h"  // provides LOGGER
#include "LED.h"  // LEDs manipulation interface
#include "Program.h"  // LED effect program manager
#include "IRremote.hpp"  // IRremote library


// Controller version string.
const String VERSION = "v0.0.6";
const unsigned long SERIAL_BAUD = 115200;

// IRremote settings
#define IR_RECEIVE_PIN 13
#define ENABLE_LED_FEEDBACK false

void setupIRremote() {
  if (!Serial) {
    Serial.begin(SERIAL_BAUD);
  }
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

// LED

void setup() {
  // Setup serial debug logger
  setupLogger(SERIAL_BAUD);

  // Setup led controller
  setupLeds();

  // Setup program manager
  setupPrgMgr();

  // Setup IRremote
  setupIRremote();

  // Print a nice message to start debug log
  LOGGER.info("");
  LOGGER.info(String("*** Always Amazing Arduino NANO Led Kontroller " + VERSION + " ***"));
  LOGGER.info("");
}


/*
  Led effect programs wrapped in Program container.
*/
Program ProgramFlash = Program("Flash Leds", &_programFlash, 480);
Program ProgramKaTT = Program("Program KaTT", &_programKaTT, 120);
Program ProgramProgress = Program("Progress Through Leds", &_programProgress, 240);
Program ProgramRunLeds = Program("Run Through Leds", &_programRunLed, 60);


void loop() {
  // TODO: when remote solution arrives rework all this...
  // This thing is supposed to be remote controlled but IR receiver has not arrived in mail yet.
  // Everything else is pretty much implemented except the receiver command handling.
  unsigned long DURATION = 6000;

  // As user would by design select program using the IR remote, we'll just run all programs in loop for now
  // Run FAST KaTT until Arduino has been on ~60 seconds.
  run_program_while(
    Program("Fast KaTT", &_programKaTT, 20),
    [](){return (millis() < 60000UL);});

  // Run SLOW KaTT until Arduino has been on ~120 seconds.
  run_program_while(
    Program("Slow KaTT", &_programKaTT, 500),
    [](){return (millis() < 120000UL);});

  // Run flash leds
  run_program_for_duration(ProgramFlash, DURATION);

  // Run KaTT
  run_program_for_duration(ProgramKaTT, DURATION);

  // Run progression
  run_program_for_duration(ProgramProgress, DURATION);

  // Run through leds
  run_program_for_duration(ProgramRunLeds, DURATION);
}
