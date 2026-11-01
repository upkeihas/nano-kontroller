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
uint8_t adjustColorBrightness(uint8_t c) {
  float brt_factor = static_cast<float>(LED_BRIGHTNESS)/LED_MAX_INTENSITY;
  return static_cast<uint8_t>(c*brt_factor);
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
void forEachLed(uint8_t brightness) {
  for (int pin : ledPins) {
    setLed(pin, brightness);
  }
}

// Sets raw color value as pwm output.
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  setLed(R_1, r);
  setLed(R_2, r);
  setLed(G_1, g);
  setLed(G_2, g);
  setLed(B_1, b);
  setLed(B_2, b);
  #ifdef DEBUG
  LOGGER.raw(String(String(r) + "," + String(g) + "," + String(b)));
  #endif
}

// Sets provided pwm pin intensity (0 .. 255).
void setLed(int pin, uint8_t brightness) {
  analogWrite(pin, brightness);
}

// Set each configured PWM channel to zero intensity.
void ledsOff() {
  forEachLed(LED_OFF);
}
