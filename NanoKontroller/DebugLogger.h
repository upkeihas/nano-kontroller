/**
 * This file is part of NanoKontroller.
 *
 * Made by upkeihas@github
 * See project repository for license information.
 *
 *
 * DebugLogger class.
 * Provides LOGGER for fast serial console debug logging.
*/

#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

const unsigned long SERIAL_BAUD = 115200;
const String LOGGER_DEFAULTNAME = "DebLog";
bool _LOGGER_ISSETUP = false;


/**
 * New DebugLogger instance.
 * Prints log message with timestamp and optional module name to debug serial.
 * Call setupLogger() in sketch setup once to init serial session first.
 *
 * Usage:
 *
 *     DebugLogger ModuleLogger = DebugLogger("MyModuleName");
 *     ModuleLogger.info("Hello from module!");
 *
 * @param name String, logger name. 
*/
class DebugLogger {
  private:
    String _millisTs();
    void _printLogLn(String module, String level, String msg);
  public:
    String Name;
    DebugLogger(String name = LOGGER_DEFAULTNAME) {
      Name = name;
    };
    void debug(String module, String msg);
    void debug(String msg);
    void info(String module, String msg);
    void info(String msg);
    void raw(String msg);
};

void setupLogger(unsigned long baud = SERIAL_BAUD);
String moduleNameStr(String module, String func);

/**
 * Default Debug Logger.
 * Call setupLogger() in sketch setup to init serial session before calling!
 *
 * Usage example:
 *
 *    LOGGER.info("This is an info message!");
 *    LOGGER.info("MYMODULE.Name", "This is an info message.");
 *    LOGGER.debug("This is a debug message.");
 *    LOGGER.debug("MYMODULE.Name", "This is a debug message?");
 *
 *    `1124 | INFO | DebLog | This is an info message!`
 *    `1128 | INFO | MYMODULE.Name | This is an info message.`
 *    `1132 | DEBG | DebLog | This is a debug message.`
 *    `1136 | DEBG | MYMODULE.Name | This is a debug message?`
 *
*/
DebugLogger LOGGER = DebugLogger();

#endif
