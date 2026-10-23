/**
 * NanoKontroller, IR remote controllable 6 channel LED strip driver made for Arduino NANO/Uno.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
*/

#define DEBUG  // Enable debug prints to USB serial for logging
#define NO_LED_FEEDBACK_CODE  // No led feedback.
#define IR_RECEIVE_PIN 2  // IR sensor pin
#define DECODE_NEC

#include "DebugLogger.h"  // provides LOGGER
#include "Program.h"  // LED effect programs and manager
// #include <TinyIRReceiver.hpp>  // Use interrupt-based TinyReceiver to free up all PWM channels for LED use
#include <IRremote.hpp>

const String VERSION = "v0.0.6";  // Current controller SW version
const unsigned long SERIAL_BAUD = 115200;  // USB debug serial speed
volatile bool IR_COMMAND_PENDING = false;  // Flag to handle pending IR command event
uint8_t _IR_COMMAND;
uint16_t _IR_ADDRESS;
uint8_t _IR_FLAGS;
unsigned int CURRENT_PROGRAM = 127;

// Starts listening to IR events
void setupIRRemote() {
  if (!Serial) {
    Serial.begin(SERIAL_BAUD);
  }
  IrReceiver.begin(IR_RECEIVE_PIN, false);
  // if (initPCIInterruptForTinyReceiver()) {
  //   LOGGER.debug("setupIRRemote", "TinyReceiver started.");
  //   return;
  // } 
  // LOGGER.debug("Failed to start TinyReceiver!");
}

// IR interrupt event handler. Just store values and raise IR_COMMAND_PENDING flag.
void handleReceivedIRData(uint16_t aAddress, uint8_t aCommand, uint8_t aFlags) {
  IR_COMMAND_PENDING = true;
  _IR_ADDRESS = aAddress;
  _IR_COMMAND = aCommand;
  _IR_FLAGS = aFlags;
}

// Global Setup
void setup() {
  // Setup serial debug logger
  setupLogger(SERIAL_BAUD);

  // Setup program manager
  setupPrgMgr();

  // Setup IRremote
  setupIRRemote();

  // Print a nice message to start debug log
  LOGGER.info("");
  LOGGER.info(String("*** Always Amazing Arduino NANO Led Kontroller " + VERSION + " ***"));
  LOGGER.info("");
}

// Helper to instantly stop current program execution.
void noProgram() {
  CURRENT_PROGRAM = 0;
  ledsOff();
}

// Find out what all that ruckus is and do something about it.
// Returns command name string after handling event.
String handleIRCommand(uint16_t command) {
  // We got it, reset interrupt flag
  IR_COMMAND_PENDING = false;
  switch (command) {
    case 0:
      InterruptBriteUp();
      LOGGER.debug(String(LED_BRIGHTNESS));
      return "BrightUp";
    case 1:
      InterruptBriteDown();
      LOGGER.debug(String(LED_BRIGHTNESS));
      return "BrightDn";
    case 2:
      // Press OFF twice to enable intro program
      if (CURRENT_PROGRAM == 0) {
        CURRENT_PROGRAM = 127;
      } else {
        noProgram();
      }
      return "OFF";
    case 3:
      noProgram();
      forEachLed(LED_BRIGHTNESS);
      return "ON";
    case 4:
      noProgram();
      setLed(6, LED_BRIGHTNESS);
      return "R";
    case 5:
      noProgram();
      setLed(9, LED_BRIGHTNESS);
      return "G";
    case 6:
      noProgram();
      setLed(10, LED_BRIGHTNESS);
      return "B";
    case 7:
      noProgram();
      forEachLed(LED_BRIGHTNESS);
      return "W";
    case 20:
      noProgram();
      setLed(5, LED_BRIGHTNESS);
      return "Y";
    case 11:
      CURRENT_PROGRAM = 1;
      return "FLASH";
    case 15:
      CURRENT_PROGRAM = 2;
      return "STROBE";
    case 19:
      CURRENT_PROGRAM = 3;
      return "FADE";
    case 23:
      CURRENT_PROGRAM = 4;
      return "SMOOTH";
    default:
      return "NOT FOUND";
  }
}


// Main Loop
void loop() {
  if (IrReceiver.decode()) {
    _IR_COMMAND = IrReceiver.decodedIRData.command;
    String commandName = handleIRCommand(_IR_COMMAND);
    IrReceiver.resume();
    LOGGER.debug(String("Handled IR command " + commandName));
  }


  // Welcome program
  if (CURRENT_PROGRAM == 127) {
    run_program_until_interrupt(ProgramBreathe);
  }

  // Flash
  else if (CURRENT_PROGRAM == 1) {
    run_program_until_interrupt(ProgramFlash);

  // Strobe
  } else if (CURRENT_PROGRAM == 2) {
    run_program_until_interrupt(ProgramKaTT);

  // Fade
  } else if (CURRENT_PROGRAM == 3) {
    run_program_until_interrupt(ProgramProgress);

  // Smooth
  } else if (CURRENT_PROGRAM == 4) {
    run_program_until_interrupt(ProgramRunLeds);
  }
}
