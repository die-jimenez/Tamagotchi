#ifndef __DELTATIME_H__
#define __DELTATIME_H__

#include <Arduino.h>

class DeltaTime {
private:
  unsigned long lastTime;
  unsigned long currentTime;
  float deltaTime;

public:
  DeltaTime();
  ~DeltaTime();

  void Run();
  float Get();
};





#endif
