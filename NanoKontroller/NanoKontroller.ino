/**
 * NANO-Kontroller, IR remote controllable 6 channel LED driver made for Arduino
 * NANO/Uno R3.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
 */

// #define DEBUG                // Enable debug prints to USB serial for logging
#define NO_LED_FEEDBACK_CODE // No led feedback for IR receive
#define IR_RECEIVE_PIN 2     // IR sensor pin
#define USE_CALLBACK_FOR_TINY_RECEIVER

#include "DebugLogger.h" // provides LOGGER
#include "Program.h"     // LED effect programs and manager
#include "TinyIRReceiver.hpp" // Use interrupt-based TinyReceiver to free up all PWM channels for LED use

const String VERSION = "v0.1.2";          // Current controller SW version
const unsigned long SERIAL_BAUD = 115200; // USB debug serial speed
// Flag to handle pending IR command event
volatile bool IR_COMMAND_PENDING = false;
uint8_t _IR_COMMAND;           // Captured IR command
uint8_t _IR_FLAGS;             // Flags to determine command success
uint8_t CURRENT_PROGRAM = 127; // Program counter
const uint8_t STATIC_COLOR_PROGRAM = 126;
const uint8_t NO_PROGRAM = 0; // Do not run a program

// Starts listening to IR events
void setupIRRemote() {
  if (!Serial) {
    Serial.begin(SERIAL_BAUD);
  }
  if (initPCIInterruptForTinyReceiver()) {
    LOGGER.info("setupIRRemote", "TinyIRReceiver started.");
    return;
  }
}

// IR interrupt event handler. Store interrupt values to global variables and
// raise IR_COMMAND_PENDING flag if calling a simple interrupt function cannot
// handle our event.
void handleReceivedTinyIRData() {
  _IR_COMMAND = TinyIRReceiverData.Command;
  _IR_FLAGS = TinyIRReceiverData.Flags;
  switch (_IR_COMMAND) {
  case 0:
    InterruptBriteUp();
    break;
  case 1:
    InterruptBriteDown();
    break;
  case 8:
    InterruptSpeedUp();
    break;
  case 9:
    InterruptSpeedDown();
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
  LOGGER.info(
      String("*** Always Amazing Arduino NANO-Kontroller " + VERSION + " ***"));
  LOGGER.info("");
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
      CURRENT_PROGRAM = NO_PROGRAM;
      ledsOff();
    }
    return "OFF";
  case 3:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(WHITE);
    return "ON";
  case 4:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(RED);
    return "RED";
  case 5:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(GREEN);
    return "GREEN";
  case 6:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(BLUE);
    return "BLUE";
  case 7:
    CURRENT_PROGRAM = 5;
    return "WHITE";
  // Color buttons 8 and 9 currently overridden for speed control
  // case 8:
  //   CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
  //   ProgramStaticColor.setColor(VERMILLION);
  //   return "VERMILLION";
  // case 9:
  //   CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
  //   ProgramStaticColor.setColor(MINT);
  //   return "MINT";
  case 10:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(DBLUE);
    return "DBLUE";
  case 12:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(ORANGE);
    return "ORANGE";
  case 13:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(TEAL);
    return "TEAL";
  case 14:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(VIOLET);
    return "VIOLET";
  case 16:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(CARROT);
    return "CARROT";
  case 17:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(MBLUE);
    return "MBLUE";
  case 18:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(INDIGO);
    return "INDIGO";
  case 20:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(YELLOW);
    return "YELLOW";
  case 21:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(TIBER);
    return "TIBER";
  case 22:
    CURRENT_PROGRAM = STATIC_COLOR_PROGRAM;
    ProgramStaticColor.setColor(PURPLE);
    return "PURPLE";
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
    LOGGER.debug("handleIRCommand", String("COMMAND: " + commandName));
  }

  // Welcome program
  if (CURRENT_PROGRAM == 127) {
    run_program_until_interrupt(ProgramBreathe);
  }

  // Static color on
  if (CURRENT_PROGRAM == STATIC_COLOR_PROGRAM) {
    run_program_until_interrupt(ProgramStaticColor);
  }

  // Color blaster
  else if (CURRENT_PROGRAM == 5) {
    run_program_until_interrupt(ProgramColorBlaster);
  }

  // Smooth
  else if (CURRENT_PROGRAM == 4) {
    run_program_until_interrupt(ProgramWave);
  }

  // Fade
  else if (CURRENT_PROGRAM == 3) {
    run_program_until_interrupt(ProgramColorFade);
  }

  // Strobe
  else if (CURRENT_PROGRAM == 2) {
    run_program_until_interrupt(ProgramKaTT);
  }

  // Flash
  else if (CURRENT_PROGRAM == 1) {
    run_program_until_interrupt(ProgramFlash);
  }

  // NO PROGRAM
  // else if (CURRENT_PROGRAM == NO_PROGRAM) {
  //   // Do nothing. Or something?
  // }
}
