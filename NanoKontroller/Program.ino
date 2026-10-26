// Setup program manager.
void setupPrgMgr() {
  // Setup led controller
  setupLeds();
  LOGGER.info("PrgMgr.setupPrgMgr", "Program Manager initialised.");
}

// Better Delay that responds faster to IR remote events
void belay(long period) {
  unsigned long start = millis();
  while (!IR_COMMAND_PENDING) {
    if ((millis() - start) >= (unsigned long)period)
      break;
  }
}

// Print program run stats to debug log.
void _program_stats(Program program) {
  #ifdef DEBUG
  // only if the program actually ran
  if (program.iters > 0) {
    String _stats = "Ran ";
    _stats += String(program.iters);
    _stats += " iters in ";
    _stats += String(program.durationMillis);
    _stats += " millis.";
    LOGGER.debug(String(program.Name), _stats);
  }
  #endif
}

// Run provided program function in a loop for provided duration millis.
void run_program_for_duration(Program program, unsigned long duration) {
  LOGGER.info("PrgMgr.run_program_for_duration", program.Name);
  setLedDelayMillis(program.Speed);
  program.runDuration(duration);
  _program_stats(program);
}

// Run provided program function in loop while condition function returns true.
void run_program_while(Program program, bool (*condition)()) {
  LOGGER.debug("PrgMgr.run_program_while", program.Name);
  setLedDelayMillis(program.Speed);
  program.runWhile(condition);
  _program_stats(program);
}

// Run provided program in a loop until IR interrupt.
void run_program_until_interrupt(Program program) {
  run_program_while(program,
                    [](){return (IR_COMMAND_PENDING == false);});
}


/**
  Effect program functions
*/

// Static color: set defined RGB color
void _programStaticColor(unsigned int r, unsigned int g, unsigned int b) {
  setLed(R_1, r);
  setLed(G_1, g);
  setLed(B_1, b);
  setLed(R_2, r);
  setLed(G_2, g);
  setLed(B_2, b);
  belay(LED_DELAYMILLIS);
}

// Flash all leds on and off
void _programFlash() {
  forEachLed(LED_BRIGHTNESS);
  belay(LED_DELAYMILLIS);
  ledsOff();
  belay(LED_DELAYMILLIS);
}

// Like that car in the eighties series, you know?
void _programKaTT() {
  for (int i = 0; i < nLedPins; i++) {
    setLed(ledPins[i],
           LED_BRIGHTNESS);
    belay(LED_DELAYMILLIS);
    setLed(ledPins[i],
           LED_MIN_BRIGHTNESS);
  }

  // omit first and last led in list
  for (int i = (nLedPins - 2); i > 0; i--) {
    setLed(ledPins[i],
           LED_BRIGHTNESS);
    belay(LED_DELAYMILLIS);
    setLed(ledPins[i],
           LED_MIN_BRIGHTNESS);
  }
}

// Run through leds
void _programRunLed() {
  for (int pin : ledPins) {
    setLed(pin, LED_BRIGHTNESS);
    belay(LED_DELAYMILLIS);
    setLed(pin, LED_MIN_BRIGHTNESS);
  }
}

// Progress through leds smoothly
void _programProgress() {
  for (int i = -1; i <= nLedPins; i++) {
    // previous pin to dim
    if (i - 1 >= 0 && i - 1 < nLedPins) {
      setLed(ledPins[i-1], LED_BRIGHTNESS/3);
    }
    // current pin to max
    if (i >= 0 && i < nLedPins) {
      setLed(ledPins[i], LED_BRIGHTNESS);
    }
    // next pin to dim
    if (i + 1 > 0 && i + 1 < nLedPins) {
      setLed(ledPins[i+1], LED_BRIGHTNESS/3);
    }

    belay(LED_DELAYMILLIS);

    // prev pin turns off
    if (i - 1 >= 0 && i - 1 < nLedPins) {
      setLed(ledPins[i-1], LED_MIN_BRIGHTNESS);
    }
  }
}

// Fade through all the channels one by one in a loop
void _programBreathe() {
  int fadeAmount = 5;
  while (!IR_COMMAND_PENDING) {
    // for each led pin in turn
    for (int pin : ledPins) {
      int brightness = LED_MIN_BRIGHTNESS;
      while (!IR_COMMAND_PENDING) {
        brightness = brightness + fadeAmount;
        // reverse polarity!
        if ((brightness <= LED_MIN_BRIGHTNESS) or (brightness >= LED_BRIGHTNESS)) {
          fadeAmount = -fadeAmount;
        }
        // alter brightness
        setLed(pin, brightness);
        belay(LED_DELAYMILLIS);

        // exit loop when back at zero intensity again
        if (brightness <= 0) {
          break;
        }
      }
    }
  }
}
