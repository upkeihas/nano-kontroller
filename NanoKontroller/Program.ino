// Setup program manager.
void setupPrgMgr()
{
  // Setup led controller
  setupLeds();
  LOGGER.info("PrgMgr.setupPrgMgr", "Program Manager initialised.");
}


/**
  Helper function to capture color button press during program execution,
  before actual IR interrupt handler runs.
  Returns captured RGB color or provided default value.
*/
RGB captureColorPress(RGB &oldColor)
{
  if (IR_COMMAND_PENDING && _IR_COMMAND >= 4 && _IR_COMMAND <= 22)
  {
    const uint8_t colorCommands[] = {4, 5, 6, 7, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22};
    for (uint8_t command : colorCommands)
      if (command == _IR_COMMAND)
      {
        IR_COMMAND_PENDING = false;
        switch (_IR_COMMAND)
        {
        case 4:
          return RED;
        case 5:
          return GREEN;
        case 6:
          return BLUE;
        case 7:
          return WHITE;
        case 10:
          return DBLUE;
        case 12:
          return ORANGE;
        case 13:
          return TEAL;
        case 14:
          return VIOLET;
        case 16:
          return CARROT;
        case 17:
          return MBLUE;
        case 18:
          return INDIGO;
        case 20:
          return YELLOW;
        case 21:
          return TIBER;
        case 22:
          return PURPLE;
        }
      }
  }
  return oldColor;
}

// Print program run stats to debug log.
void programStats(Program &program)
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

// Run provided program function in a loop for provided duration millis.
void run_program_for_duration(Program program, unsigned long duration)
{
  LOGGER.info("PrgMgr.run_program_for_duration", program.Name);
  setLedDelayMillis(program.Speed);
  program.runDuration(duration);
  programStats(program);
}

// Run provided program function in loop while condition function returns true.
void run_program_while(Program program, bool (*condition)())
{
  LOGGER.debug("PrgMgr.run_program_while", program.Name);
  setLedDelayMillis(program.Speed);
  program.runWhile(condition);
  programStats(program);
}

// Run provided program in a loop until IR interrupt.
void run_program_until_interrupt(Program program)
{
  run_program_while(program,
                    []()
                    { return (IR_COMMAND_PENDING == false); });
}

/**
  Effect program functions.
  Program functions can either be one-offs that are automatically run in a loop,
  or they can safely block regular program execution flow by monitoring the state of
  global boolean IR_COMMAND_PENDING, signaling an IR command has been received and needs handling.
*/

// Fade through all the channels one by one in a loop
void _programBreathe()
{
  int fadeAmount = 1;
  int brightness = LED_OFF;
  for (int pin : ledPins)
  {
    while (!IR_COMMAND_PENDING)
    {
      brightness += fadeAmount;
      // reverse polarity!
      if ((brightness <= LED_OFF) || ((brightness >= LED_BRIGHTNESS) && (fadeAmount > 0)))
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
  RGB color;
  while (!IR_COMMAND_PENDING) {
    color = {static_cast<uint8_t>(random(LED_MIN_BRIGHTNESS, LED_BRIGHTNESS)),
             static_cast<uint8_t>(random(LED_MIN_BRIGHTNESS, LED_BRIGHTNESS)),
             static_cast<uint8_t>(random(LED_MIN_BRIGHTNESS, LED_BRIGHTNESS))};
    setColor(color);
    belay(LED_DELAYMILLIS);
  }
}

// Fade through 2 color channels at a time
void _programColorFade()
{
  RGB color = {LED_OFF, LED_OFF, LED_MAX_INTENSITY};
  while (!IR_COMMAND_PENDING)
  {
    // run once per color channel
    for (uint8_t i = 0; (i < 3 && !IR_COMMAND_PENDING); i++)
    {
      // For each brightness step
      for (uint8_t j = 0; (j < LED_MAX_INTENSITY && !IR_COMMAND_PENDING); j++)
      {
        // Red
        if (i == 0)
        {
          color.r += 1;
          color.b -= 1;
        }
        // Green
        else if (i == 1)
        {
          color.r -= 1;
          color.g += 1;
        }
        // Blue
        else if (i == 2)
        {
          color.b += 1;
          color.g -= 1;
        }
        setBrightnessAdjustedColor(color);
        belay(LED_DELAYMILLIS);
      }
    }
  }
}

// Flash color on and off
void _programFlash()
{
  RGB color = WHITE;
  while (!IR_COMMAND_PENDING)
  {
    setBrightnessAdjustedColor(color);
    belay(LED_DELAYMILLIS);
    ledsOff();
    belay(LED_DELAYMILLIS);

    // Toggles effect color if user presses color buttons on remote
    color = captureColorPress(color);
  }
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

// Static color: set defined RGB color
void _programStaticColor(RGB color)
{
  setBrightnessAdjustedColor(color);
  bblock();
}

// Wave generator program to cycle through colors smoothly
void _programWave()
{
  float rad = 0.0;              // starting point of color wheel
  float step = TWO_PI / 360.0f; // one degree steps
  float amplitude;
  RGB color;
  while (!IR_COMMAND_PENDING && (rad < TWO_PI))
  {
    amplitude = LED_BRIGHTNESS / 2.0f; // divide by 2 because sin() returns -1 .. 1
    // TODO: less calls to sin(), consider using static time series of sine wave intensity values instead
    color.r = static_cast<uint8_t>((sin(rad) + 1) * amplitude);
    color.g = static_cast<uint8_t>((sin(rad + 2.09f) + 1) * amplitude);
    color.b = static_cast<uint8_t>((sin(rad + 4.19f) + 1) * amplitude);
    setColor(color);
    belay(LED_DELAYMILLIS);
    rad += step;
  }
}
