#ifndef __ANIMACION_H__
#define __ANIMACION_H__

#include <Arduino.h>
#include "src/DeltaTime/DeltaTime.h"

class Animacion {
private:
  const unsigned char** sprites;  //puntero doble
  int frameRate;
  int frameCount;
  int currentFrame;
  int width, height;
  bool isLoop;
  bool isPlaying;
  int anim_length;

  uint32_t frameInterval;
  DeltaTime deltaTime;

public:
  Animacion(const unsigned char* _sprites[], int _frameRate, int _width, int _height);
  ~Animacion();

  // // Configuración básica
  // void setFPS(float fps);
  // void setLoop(bool enable);
  // void setSize(uint16_t w, uint16_t h);

  // // Control de reproducción
  // void play();
  // void pause();
  // void stop();
  // void reset();

  // // Control de segmentos de animación
  // void playSegment(uint16_t start, uint16_t end);

  // Actualización de animación
  void play();

  // Getters
  int GetWidth();
  int GetHeight();
  const unsigned char* GetSprite(int index);

  // uint16_t getCurrentFrameIndex();
  // uint16_t getFrameCount();
  // bool isLooping();
  // bool isPlaying();
};

#endif