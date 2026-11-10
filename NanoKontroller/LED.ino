// Setup led pin states
void setupLeds() {
  String _pins = "";
  for (int pin : ledPins) {
    _pins += String(pin);
    _pins += ", ";
    pinMode(pin, OUTPUT);
  }
  LOGGER.info(
      "LED.setupLeds",
      String("Led controller initialised. Configured PWM pins: " + _pins));
}

// Adjust color code value to respect current LED_BRIGHTNESS value
uint8_t adjustColorBrightness(uint8_t c) {
  float brt_factor = static_cast<float>(LED_BRIGHTNESS) / LED_MAX_INTENSITY;
  return static_cast<uint8_t>(c * brt_factor);
}

// Set led delay millis to provided value.
void setLedDelayMillis(uint16_t speed) { LED_DELAYMILLIS = speed; }

/**
  Interrupt handlers
*/

// Brightness down interrupt handler. Do not allow to go all the way to 0.
void InterruptBriteDown() {
  if ((LED_BRIGHTNESS - ADJUST_STEP) >= LED_MIN_BRIGHTNESS) {
    LED_BRIGHTNESS -= ADJUST_STEP;
    BRIGHTNESS_EVENT_PENDING = true;
  }
}

// Brightness up interrupt handler.
void InterruptBriteUp() {
  if ((LED_BRIGHTNESS + ADJUST_STEP) <= LED_MAX_INTENSITY) {
    LED_BRIGHTNESS += ADJUST_STEP;
    BRIGHTNESS_EVENT_PENDING = true;
  }
}

// Speed down interrupt handler.
void InterruptSpeedDown() {
  if (LED_DELAYMILLIS < 10) {
    LED_DELAYMILLIS += 1;
  } else if (LED_DELAYMILLIS < 100) {
    LED_DELAYMILLIS += ADJUST_STEP;
  } else {
    LED_DELAYMILLIS += 10;
  }
}

// Speed up interrupt handler.
void InterruptSpeedUp() {
  // Keep some sensible minimum delay
  if (LED_DELAYMILLIS > ADJUST_STEP) {
    if (LED_DELAYMILLIS < 10) {
      LED_DELAYMILLIS -= 1;
    } else if (LED_DELAYMILLIS < 100) {
      LED_DELAYMILLIS -= ADJUST_STEP;
    } else {
      LED_DELAYMILLIS -= 10;
    }
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

// Sets brightness adjusted color value for both led groups.
void setBrightnessAdjustedColor(RGB color) {
  color.r = adjustColorBrightness(color.r);
  color.g = adjustColorBrightness(color.g);
  color.b = adjustColorBrightness(color.b);
  setColor(color);
}

// Sets raw color value for both led groups.
void setColor(RGB &color) {
  setLed(R_1, color.r);
  setLed(R_2, color.r);
  setLed(G_1, color.g);
  setLed(G_2, color.g);
  setLed(B_1, color.b);
  setLed(B_2, color.b);
#ifdef DEBUG
  LOGGER.raw(
      String(String(color.r) + "," + String(color.g) + "," + String(color.b)));
#endif
}

// Sets provided pwm pin intensity (0 .. 255).
void setLed(int pin, uint8_t brightness) { analogWrite(pin, brightness); }

// Set each configured PWM channel to zero intensity.
void ledsOff() { forEachLed(LED_OFF); }
