//patrón de "fluent interface" o "method chaining" como el de DOTween

#ifndef __ANIMACION_H__
#define __ANIMACION_H__

#include <Arduino.h>
#include "src/DeltaTime/DeltaTime.h"

class Animacion {
private:
  //Datos de la animacion
  const unsigned char** sprites;  //puntero doble
  int width, height;
  int frameCount;
  int frameRate;
  float frameInterval;

  //Variables
  int posX, posY;
  float time;
  bool isLoop;
  bool isPlaying;
  bool forceStop;
  int currentFrame;

  //Funciones privadas
  void SetPosition(int _x, int _y);


public:
  Animacion(const unsigned char* _sprites[], int _frameRate, int _frameCount, int _width, int _height);
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
  void Play(float _deltaTime);
  void Stop();
  void Pause();
  void Continue();
  void SetLoop(bool _isLoop);

  // Getters
  int GetWidth();
  int GetHeight();
  int GetPosX();
  int GetPosY();
  const unsigned char* GetCurrentSprite();
  const unsigned char* GetSprite(int index);

  // uint16_t getCurrentFrameIndex();
  // uint16_t getFrameCount();
  // bool isLooping();
  // bool isPlaying();
};

#endif