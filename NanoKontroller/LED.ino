// Setup led pin states
void setupLeds() {
  String _pins = "";
  for (int pin : ledPins) {
    _pins += String(pin);
    _pins += ", ";
    pinMode(pin, OUTPUT);
  }
  LOGGER.info("LED.setupLeds", String("Led controller initialised. Configured PWM pins: " + _pins));
}

// Adjust color code value to respect current LED_BRIGHTNESS value
unsigned int adjustColorBrightness(unsigned int c) {
  float brt_factor = static_cast<float>(LED_BRIGHTNESS)/LED_MAX_INTENSITY;
  return static_cast<unsigned int>(c*brt_factor);
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

// Brightness down interrupt handler. Do not allow to go all the way to 0.
void InterruptBriteDown() {
  if (LED_BRIGHTNESS > LED_MIN_BRIGHTNESS) {
    LED_BRIGHTNESS -= ADJUST_STEP;
  }
}


/**
  Led manipulation
*/

// Set brightness value for each possible pwm channel.
void forEachLed(unsigned int brightness) {
  for (int pin : ledPins) {
    setLed(pin, brightness);
  }
}

// Sets provided pwm pin intensity (0 .. 255).
void setLed(int pin, unsigned int brightness) {
  analogWrite(pin, brightness);
}

// Set each configured PWM channel to zero intensity.
void ledsOff() {
  forEachLed(0);
}
