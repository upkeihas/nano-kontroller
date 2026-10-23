// Setup led pin states and debug logger
void setupLeds() {
  String _pins = "";
  for (int pin : ledPins) {
    _pins += String(pin);
    _pins += ", ";
    pinMode(pin, OUTPUT);
  }
  LOGGER.info("LED.setupLeds", String("Led controller initialised. Configured PWM pins: " + _pins));
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
  if (LED_DELAYMILLIS >= ADJUST_STEP) {
    LED_DELAYMILLIS -= ADJUST_STEP;
  }
}

// Speed down interrupt handler.
void InterruptSpeedDown() {
  // Cap max delay millis to ~30k
  if (LED_DELAYMILLIS < 30000) {
    LED_DELAYMILLIS += ADJUST_STEP;
  }
}

// Brightness up interrupt handler.
void InterruptBriteUp() {
  if ((LED_BRIGHTNESS + ADJUST_STEP) <= LED_MAX_INTENSITY) {
    LED_BRIGHTNESS += ADJUST_STEP;
  }
}

// Brightness down interrupt handler.
void InterruptBriteDown() {
  if (LED_BRIGHTNESS >= ADJUST_STEP) {
    LED_BRIGHTNESS -= ADJUST_STEP;
  }
}

/**
  Led manipulation
*/

// Set brightness value for each possible pwm channel.
void forEachLed(int brightness) {
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
