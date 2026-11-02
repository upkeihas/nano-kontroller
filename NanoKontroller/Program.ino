// Setup program manager.
void setupPrgMgr()
{
  // Setup led controller
  setupLeds();
  LOGGER.info("PrgMgr.setupPrgMgr", "Program Manager initialised.");
}

// Better block that responds to IR remote events.
void bblock()
{
  while (!IR_COMMAND_PENDING && !BRIGHTNESS_EVENT_PENDING)
  {
    belay(LED_DELAYMILLIS);
  }
}

// Better delay that responds to IR remote events.
void belay(long period)
{
  unsigned long start = millis();
  while (!IR_COMMAND_PENDING)
  {
    if ((millis() - start) >= (unsigned long)period)
      break;
  }
}

// Print program run stats to debug log.
void _program_stats(Program program)
{
#ifdef DEBUG
  // only if the program actually ran
  if (program.iters > 0)
  {
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
void run_program_for_duration(Program program, unsigned long duration)
{
  LOGGER.info("PrgMgr.run_program_for_duration", program.Name);
  setLedDelayMillis(program.Speed);
  program.runDuration(duration);
  _program_stats(program);
}

// Run provided program function in loop while condition function returns true.
void run_program_while(Program program, bool (*condition)())
{
  LOGGER.debug("PrgMgr.run_program_while", program.Name);
  setLedDelayMillis(program.Speed);
  program.runWhile(condition);
  _program_stats(program);
}

// Run provided program in a loop until IR interrupt.
void run_program_until_interrupt(Program program)
{
  run_program_while(program,
                    []()
                    { return (IR_COMMAND_PENDING == false); });
}

/**
  Effect program functions
*/

// Fade through all the channels one by one in a loop
void _programBreathe()
{
  int fadeAmount = ADJUST_STEP;
  for (int pin : ledPins)
  {
    int brightness = LED_OFF;
    while (!IR_COMMAND_PENDING)
    {
      brightness = brightness + fadeAmount;
      // reverse polarity!
      if ((brightness <= LED_OFF) || (brightness >= LED_BRIGHTNESS))
      {
        fadeAmount = -fadeAmount;
      }
      setLed(pin, static_cast<uint8_t>(brightness));
      belay(LED_DELAYMILLIS);

      // exit loop when back at zero intensity again
      if (brightness <= LED_OFF)
      {
        break;
      }
    }
  }
}

// Set all channels to random value, creating a vomit of color output!
void _programColorBlaster()
{
  setColor(static_cast<uint8_t>(random(LED_MIN_BRIGHTNESS, LED_BRIGHTNESS)),
           static_cast<uint8_t>(random(LED_MIN_BRIGHTNESS, LED_BRIGHTNESS)),
           static_cast<uint8_t>(random(LED_MIN_BRIGHTNESS, LED_BRIGHTNESS)));
  belay(LED_DELAYMILLIS);
}

// Flash all leds on and off
void _programFlash()
{
  forEachLed(LED_BRIGHTNESS);
  belay(LED_DELAYMILLIS);
  ledsOff();
  belay(LED_DELAYMILLIS);
}

// Like that car in the eighties series, you know?
void _programKaTT()
{
  for (int i = 0; i < nLedPins; i++)
  {
    setLed(ledPins[i],
           LED_BRIGHTNESS);
    belay(LED_DELAYMILLIS);
    setLed(ledPins[i],
           LED_OFF);
  }

  // omit first and last led in list
  for (int i = (nLedPins - 2); i > 0; i--)
  {
    setLed(ledPins[i],
           LED_BRIGHTNESS);
    belay(LED_DELAYMILLIS);
    setLed(ledPins[i],
           LED_OFF);
  }
}

// Progress through leds smoothly
void _programProgress()
{
  for (int i = -1; i <= nLedPins; i++)
  {
    // previous pin to dim
    if (i - 1 >= 0 && i - 1 < nLedPins)
    {
      setLed(ledPins[i - 1],
             static_cast<uint8_t>(LED_BRIGHTNESS / 3));
    }
    // current pin to max
    if (i >= 0 && i < nLedPins)
    {
      setLed(ledPins[i], LED_BRIGHTNESS);
    }
    // next pin to dim
    if (i + 1 > 0 && i + 1 < nLedPins)
    {
      setLed(ledPins[i + 1],
             static_cast<uint8_t>(LED_BRIGHTNESS / 3));
    }

    belay(LED_DELAYMILLIS);

    // prev pin turns off
    if (i - 1 >= 0 && i - 1 < nLedPins)
    {
      setLed(ledPins[i - 1], LED_OFF);
    }
  }
}

// Static color: set defined RGB color
void _programStaticColor(uint8_t r, uint8_t g, uint8_t b)
{
  setColor(adjustColorBrightness(r),
           adjustColorBrightness(g),
           adjustColorBrightness(b));
  bblock();
}

// Wave generator program to cycle through colors smoothly
void _programWave()
{
  float rad = 0.0;             // starting point of color wheel
  float step = TWO_PI / 360.0; // one degree steps
  float amplitude;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  while (!IR_COMMAND_PENDING && (rad < TWO_PI))
  {
    amplitude = LED_BRIGHTNESS / 2.0; // divide by 2 because sin() returns -1 .. 1
    // TODO: less calls to sin(), consider using static time series of sine wave intensity values instead
    r = static_cast<uint8_t>((sin(rad) + 1) * amplitude);
    g = static_cast<uint8_t>((sin(rad + 2) + 1) * amplitude);
    b = static_cast<uint8_t>((sin(rad + 4) + 1) * amplitude);
    setColor(r, g, b);
    belay(LED_DELAYMILLIS);
    rad += step;
  }
}
