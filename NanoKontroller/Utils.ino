/**
  Better less-blocking delay.
  Use this instead of delay() to maintain responsiveness while inside a program.
*/
void belay(long period) {
  unsigned long start = millis();
  while (true) {
    if ((millis() - start) >= (unsigned long)period)
      break;
  }
}
