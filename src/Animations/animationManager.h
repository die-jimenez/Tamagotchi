//CUIDADO:
//Animation tiene un sistema de eventos similar a DotWeen
//sin embargo, al utilizar el "animationManager" para ejecutar las animaciones
//los eventos se deben agregar manualmente "anim.SetOnComplete()" y no como se hace en DotWeen "anim.Play().OnComplete()"

#ifndef __ANIMATIONMANAGER_H__
#define __ANIMATIONMANAGER_H__

#include <Arduino.h>
#include "animation.h"
#include <Adafruit_SSD1306.h>
#include "src/DeltaTime/DeltaTime.h"


class AnimationManager {
private:
  int maxAnimations = 10;
  Animation* animations[10];
  Adafruit_SSD1306* display;
  float deltaTime;



public:
  AnimationManager(Adafruit_SSD1306* _display);
  ~AnimationManager();

  //Update ejecuta todas las animaciones guardadas. Debe estar siempre activo en el Loop
  void Update(float _deltaTime);

  void Play(Animation* _anim);
  void Stop(Animation* _anim);
  void Remove(Animation* _anim);
  void Continue(Animation* _anim);
  void RemoveAll();  
  void ContinueAll();
  void StopAll();
  //PlayOneShot ejecuta la animacion una vez y luego se borra de la memoria del manager
  void PlayOneShot(Animation* _anim);



  bool isEmptyAnimation(int index);
  void PrintAnimationsSpaces();
};
#endif