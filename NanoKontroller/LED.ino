// Setup led pin states and debug logger
void setupLeds() {
  setupLogger();
  String _pins = "";
  for (int pin : ledPins) {
    _pins += String(pin);
    _pins += ", ";
    pinMode(pin, OUTPUT);
  }
  LOGGER.info("LEDCTL.setupLeds", String("Led controller initialised. Configured PWM pins: " + _pins));
}

// Set led delay millis to provided value.
void setLedDelayMillis(unsigned long speed) {
  LED_DELAYMILLIS = speed;
}

/**
  Interrupt handlers
*/

// Speed up interrupt handler.
void InterruptSpeedUp() {
  if (LED_DELAYMILLIS > 1) {
    LED_DELAYMILLIS--;
  }
}

// Speed down interrupt handler.
void InterruptSpeedDown() {
  if (LED_DELAYMILLIS < 65535)  // this is ridiculous max delay, right??
  LED_DELAYMILLIS++;
}

// Brightness up interrupt handler.
void InterruptBriteUp() {
  if (LED_MAXBRITE < 255) {
    LED_MAXBRITE++;
  }
}

// Brightness down interrupt handler.
void InterruptBriteDown() {
  if (LED_MAXBRITE > 0) {
    LED_MAXBRITE--;
  }
}

/**
  Led manipulation
*/

// Set brightness value for each possible pwm channel.
void forEachLed(int brightness) {
  LOGGER.debug("LEDCTL.forEachLed", String("brightness: " + String(brightness)));
  for (int pin : ledPins) {
    setLed(pin, brightness);
  }
}

// Sets provided pwm pin intensity (0 .. 255).
void setLed(int pin, int brightness) {
  analogWrite(pin, brightness);
}

// Set each configured PWM channel to zero intensity.
void ledsOff() {
  forEachLed(0);
}

/**
  Effect program functions
*/

// Flash all leds on and off
void _programFlash() {
  forEachLed(LED_MAXBRITE);
  belay(LED_DELAYMILLIS);
  ledsOff();
  belay(LED_DELAYMILLIS);
}

// Like that car in the eighties series, you know?
void _programKaTT() {
  for (int i = 0; i < nLedPins; i++) {
    setLed(ledPins[i],
           LED_MAXBRITE);
    belay(LED_DELAYMILLIS);
    setLed(ledPins[i],
           LED_MINBRITE);
  }

  // omit first and last led in list
  for (int i = (nLedPins - 2); i > 0; i--) {
    setLed(ledPins[i],
           LED_MAXBRITE);
    belay(LED_DELAYMILLIS);
    setLed(ledPins[i],
           LED_MINBRITE);
  }
}

// Run through leds
void _programRunLed() {
  for (int pin : ledPins) {
    setLed(pin, LED_MAXBRITE);
    belay(LED_DELAYMILLIS);
    setLed(pin, LED_MINBRITE);
  }
}

// Progress through leds smoothly
void _programProgress() {
  for (int i = -1; i <= nLedPins; i++) {
    // previous pin to dim
    if (0 < i-1 < nLedPins) {
      setLed(ledPins[i-1], LED_MAXBRITE/3);
    }
    // current pin to max
    if (i < nLedPins) {
      setLed(ledPins[i], LED_MAXBRITE);
    }
    // next pin to dim
    if (0 < i+1 < nLedPins) {
      setLed(ledPins[i+1], LED_MAXBRITE/3);
    }

    belay(LED_DELAYMILLIS);

    // prev pin turns off
    if (0 < i-1 < nLedPins) {
      setLed(ledPins[i-1], LED_MINBRITE);
    }
  }
}
