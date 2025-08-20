//patrón de "fluent interface" o "method chaining" como el de DOTween

//Eventos personalizados añadadibles por play.On...
// OnStart
// OnComplete
// OnFrameChange(int frame)
// OnLoop

#ifndef __ANIMACION_H__
#define __ANIMACION_H__

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "src/DeltaTime/DeltaTime.h"
//Lo declaro antes para poder usarlo en la Clase "Animacion"
class AnimacionPlayback;



class Animacion {
private:
  //Datos de la animacion
  const unsigned char** sprites;  //puntero doble
  int width, height;
  int frameCount;
  int frameRate;
  int currentFrame;
  float frameInterval;
  Adafruit_SSD1306* display;

  //Variables
  int posX, posY;
  float time;
  bool isLoop = true;
  bool isPlaying = false;
  bool forceStop = false;
  bool hasCompleted = false;
  bool areEventsLoaded = false;
  bool isCenterMode = true;

  // Callbacks para eventos
  std::function<void()> onCompleteCallback;
  std::function<void()> onStartCallback;
  std::function<void(int)> onFrameChangeCallback;
  std::function<void()> onLoopCallback;

  //Funciones privadas
  void SetPosition(int _x, int _y);
  //Triggers de eventos
  void TriggerOnComplete();
  void TriggerOnStart();
  void TriggerOnFrameChange(int frame);
  void TriggerOnLoop();





public:
  Animacion(Adafruit_SSD1306* display, const unsigned char* _sprites[], int _frameCount, int _frameRate, int _width, int _height);
  ~Animacion();

  //Devuelve un AnimacionPlayBack para poder agregar los eventos como DotWeen
  AnimacionPlayback Play(int16_t posX, int16_t posY, uint16_t color, float _deltaTime);

  //Cambios en la ejecucion de la animacion
  //void Play(float _deltaTime);
  void Stop();
  void Pause();
  void Continue();
  void SetLoop(bool _isLoop);
  void SetCenterMode(bool _val);

  // Getters
  int GetWidth();
  int GetHeight();
  int GetPosX();
  int GetPosY();
  const unsigned char* GetCurrentSprite();
  const unsigned char* GetSprite(int index);

  //Eventos 
  void SetOnComplete(std::function<void()> action) {
    onCompleteCallback = action;
  }
  void SetOnStart(std::function<void()> action) {
    onStartCallback = action;
  }
  void SetOnFrameChange(std::function<void(int)> action) {
    onFrameChangeCallback = action;
  }
  void SetOnLoop(std::function<void()> action) {
    onLoopCallback = action;
  }

};




// Clase para el fluent interface
class AnimacionPlayback {
private:
  Animacion* animacion;
  
public:
  //Constructor + "animacion(anim)" que inicializa el puntero "Animacion*" (* : Indica que es un puntero)
  AnimacionPlayback(Animacion* anim) : animacion(anim) {}
  
  // Método para asignar callback cuando se completa la animación
  AnimacionPlayback& OnComplete(std::function<void()> action) {
    if (animacion) {
      animacion->SetOnComplete(action);
    }
    return *this;
  }
  
  // Método para asignar callback cuando inicia la animación
  AnimacionPlayback& OnStart(std::function<void()> action) {
    if (animacion) {
      animacion->SetOnStart(action);
    }
    return *this;
  }
  
  // Método para asignar callback cuando cambia de frame
  AnimacionPlayback& OnFrameChange(std::function<void(int)> action) {
    if (animacion) {
      animacion->SetOnFrameChange(action);
    }
    return *this;
  }
  
  // Método para asignar callback cuando se reinicia el loop
  AnimacionPlayback& OnLoop(std::function<void()> action) {
    if (animacion) {
      animacion->SetOnLoop(action);
    }
    return *this;
  }
};

#endif