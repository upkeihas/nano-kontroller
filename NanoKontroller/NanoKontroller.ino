/**
 * NanoKontroller, IR remote controllable 6 channel LED driver made for Arduino NANO/Uno R3.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
*/

#define DEBUG  // Enable debug prints to USB serial for logging
#define NO_LED_FEEDBACK_CODE  // No led feedback for IR receive
#define IR_RECEIVE_PIN 2  // IR sensor pin
#define USE_CALLBACK_FOR_TINY_RECEIVER

#include "DebugLogger.h"  // provides LOGGER
#include "Program.h"  // LED effect programs and manager
#include "TinyIRReceiver.hpp"  // Use interrupt-based TinyReceiver to free up all PWM channels for LED use

const String VERSION = "v0.0.7";  // Current controller SW version
const unsigned long SERIAL_BAUD = 115200;  // USB debug serial speed
volatile bool IR_COMMAND_PENDING = false;  // Flag to handle pending IR command event
uint8_t _IR_COMMAND;  // Captured command
uint8_t _IR_FLAGS;  // Flags to determine command success
unsigned int CURRENT_PROGRAM = 127;  // Program counter
const unsingned int NO_PROGRAM = 0;  // Do not run a program


// Starts listening to IR events
void setupIRRemote() {
  if (!Serial) {
    Serial.begin(SERIAL_BAUD);
  }
  if (initPCIInterruptForTinyReceiver()) {
    LOGGER.info("setupIRRemote", "TinyReceiver started.");
    return;
  }
  LOGGER.debug("Failed to start TinyReceiver!");
}

// IR interrupt event handler. If calling a simple interrupt function cannot handle
// our event then store values to global variables and raise IR_COMMAND_PENDING flag.
void handleReceivedTinyIRData() {
  _IR_COMMAND = TinyIRReceiverData.Command;
  _IR_FLAGS = TinyIRReceiverData.Flags;
  // TODO: add interrupt calls for program speed
  switch (_IR_COMMAND) {
    case 0:
      InterruptBriteUp();
      break;
    case 1:
      InterruptBriteDown();
      break;
    default:
      // Can't be handled by simple interrupt, so escalate!
      IR_COMMAND_PENDING = true;
      break;
  }
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
  CURRENT_PROGRAM = NO_PROGRAM;
  ledsOff();
}

// Find out what all that ruckus is and do something about it.
// Returns command name string after handling event.
String handleIRCommand(uint16_t command) {
  // We got it, reset interrupt flag
  IR_COMMAND_PENDING = false;
  switch (command) {
    case 2:
      // Press OFF twice to enable intro program
      if (CURRENT_PROGRAM == NO_PROGRAM) {
        CURRENT_PROGRAM = 127;
      } else {
        noProgram();
      }
      return "OFF";
    case 3:
      CURRENT_PROGRAM = 126;
      return "ON";
    case 4:
      noProgram();
      setLed(3, LED_BRIGHTNESS);
      setLed(9, LED_BRIGHTNESS);
      return "R";
    case 5:
      noProgram();
      setLed(5, LED_BRIGHTNESS);
      setLed(10, LED_BRIGHTNESS);
      return "G";
    case 6:
      noProgram();
      setLed(6, LED_BRIGHTNESS);
      setLed(11, LED_BRIGHTNESS);
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
      LOGGER.debug("handleIRCommand", "Unknown command: " + String(command));
      return "NOT FOUND";
  }
}


// Main Loop
void loop() {
  if (IR_COMMAND_PENDING) {
    String commandName = handleIRCommand(_IR_COMMAND);
    LOGGER.debug("handleIRCommand", "COMMAND: " + commandName);
  }


  // Welcome program
  if (CURRENT_PROGRAM == 127) {
    run_program_until_interrupt(ProgramBreathe);
  }

  // Static leds, run as program to keep responsive for brightness adjustment
  else if (CURRENT_PROGRAM == 126) {
    run_program_until_interrupt(ProgramStatic);
  }

  // Flash
  else if (CURRENT_PROGRAM == 1) {
    run_program_until_interrupt(ProgramFlash);
  }

  // Strobe
  else if (CURRENT_PROGRAM == 2) {
    run_program_until_interrupt(ProgramKaTT);
  }

  // Fade
  else if (CURRENT_PROGRAM == 3) {
    run_program_until_interrupt(ProgramProgress);
  }

  // Smooth
  else if (CURRENT_PROGRAM == 4) {
    run_program_until_interrupt(ProgramRunLeds);
  }
}
