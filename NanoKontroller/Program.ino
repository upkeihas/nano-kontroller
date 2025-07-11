// Setup program manager.
void setupProgMan() {
  setupLogger();
  LOGGER.info("PRGMAN.setupProgMan", "Program Manager initialised.");
}

// Print program run stats to debug log.
void _program_stats(Program program) {
  // only if the program actually ran
  if (program.iters > 0) {
    String _stats = "Ran ";
    _stats += String(program.iters);
    _stats += " iterations in ";
    _stats += String(program.durationMillis);
    _stats += " millis.";
    LOGGER.debug("PRGMAN._program_stats", _stats);
  }
}

// Run provided program function in a loop for provided duration millis.
void run_program_for_duration(Program program, unsigned long duration) {
  LOGGER.info("PRGMAN.run_program_for_duration", program.Name);
  setLedDelayMillis(program.Speed);
  program.runDuration(duration);
  _program_stats(program);
}

// Run provided program function in loop while condition function returns true.
void run_program_while(Program program, bool (*condition)()) {
  LOGGER.debug("PRGMAN.run_program_while", program.Name);
  setLedDelayMillis(program.Speed);
  program.runWhile(condition);
  _program_stats(program);
}
