// Starts serial connection session for outputting log messages.
void setupLogger(unsigned long baud = SERIAL_BAUD) {
  if (!_LOGGER_ISSETUP) {
  Serial.begin(baud);
  _LOGGER_ISSETUP = true;
  LOGGER.info(moduleNameStr(LOGGER.Name, "setupLogger"), "Debug logger initialised.");
  }
}

// Combine module and function name for logging.
String moduleNameStr(String module, String func) {
  String _mod = module + "." + func;
  return _mod;
}

/*
  DebugLogger class
*/

// Current millis as string.
String DebugLogger::_millisTs() {
  return String(millis());
}

// Print message to serial console with timestamp and module name.
void DebugLogger::_printLogLn(String module, String level, String msg) {
  Serial.print(millis());
  Serial.print(" | ");
  Serial.print(level);
  Serial.print(" | ");
  Serial.print(module);
  Serial.print(" | "); 
  Serial.println(msg);
}

// Log DEBUG message with module name.
void DebugLogger::debug(String module, String msg) {
  #ifdef DEBUG
  _printLogLn(module, "DEBG", msg);
  #endif
}

// Log DEBUG message.
void DebugLogger::debug(String msg) {
  #ifdef DEBUG
  _printLogLn(this->Name, "DEBG", msg);
  #endif
}

// Log INFO message with module name.
void DebugLogger::info(String module, String msg) {
  _printLogLn(module, "INFO", msg);
}

// Log INFO message.
void DebugLogger::info(String msg) {
  _printLogLn(this->Name, "INFO", msg);
}

// Output the provided message to serial log as-is.
void DebugLogger::raw(String msg) {
  Serial.println(msg);
}
