#include "shared.h"

bool Shared::isTimeElapsed(unsigned long &lastUpdateTime, unsigned long update_interval_ms)
{
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= update_interval_ms)
  {
    lastUpdateTime = currentTime;
    return true;
  }
  return false;
}