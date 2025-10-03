#ifndef SCENE_H
#define SCENE_H

#include <Arduino.h>  // para delay()
#include "../../stateMachine.h"
#include "../AnimationManager/animationManager.h"
#include "../AnimationManager/animation.h"

class Scene {
public:
  Adafruit_SSD1306* display;
  AnimationManager* animationManager;

  virtual void Init();
  virtual void Update(float _deltaTime);
  virtual void Render() {};
  virtual void OnExit() {};
  virtual ~Scene() {};
};

#endif