//patrón de "fluent interface" o "method chaining" como el de DOTween
//Eventos personalizados añadadibles por play.On...
// OnStart
// OnComplete
// OnFrameChange(int frame)
// OnLoop

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "InterpolationLib.h"
//Lo declaro antes para poder usarlo en la Clase "Animacion"
class AnimationPlayback;



class Animation {
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

  //Interpolación
  bool isMoving;
  float startPosX, startPosY;
  float targetPosX, targetPosY;
  unsigned long moveStartTime;
  unsigned long moveDuration;
  int interpolationType;  // 0=Linear, 1=Smooth, 2=Spline

  // Callbacks para eventos
  std::function<void()> onCompleteCallback;
  std::function<void()> onStartCallback;
  std::function<void(int)> onFrameChangeCallback;
  std::function<void()> onLoopCallback;
  std::function<void()> onAfterCompleteCallback;
  std::function<void()> onMoveCompleteCallback;


  //Triggers de eventos
  void TriggerOnComplete();
  void TriggerOnStart();
  void TriggerOnFrameChange(int frame);
  void TriggerOnLoop();
  //Solo se utiliza para el PlayOneShot del aniamtionManager
  void TriggerOnAfterComplete();
  //Se utiliza en la interpolacion
  void TriggerOnMoveComplete();





public:
  Animation(Adafruit_SSD1306* display, const unsigned char* _sprites[], int _frameCount, int _frameRate, int _width, int _height);
  ~Animation();

  //Devuelve un AnimacionPlayBack para poder agregar los eventos como DotWeen
  AnimationPlayback Play(uint16_t color, float _deltaTime);

  //Cambios en la ejecucion de la animacion
  //void Play(float _deltaTime);
  void Stop();
  void Pause();
  void Continue();
  void SetLoop(bool _isLoop);
  void SetCenterMode(bool _val);
  void SetPosition(int _x, int _y);
  void ClearEvents();

  // Interpolación
  void MoveTo(float endX, float endY, unsigned long duration, int type = 1);
  void UpdateMovement(unsigned long currentTime);
  bool IsMoving() const;


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
  //Solo se utiliza para el PlayOneShot del aniamtionManager
  void SetOnAfterComplete(std::function<void()> action) {
    onAfterCompleteCallback = action;
  }
  void SetOnMoveComplete(std::function<void()> action) {
    onMoveCompleteCallback = action;
  };
};




// Clase para el fluent interface
class AnimationPlayback {
private:
  Animation* animation;

public:
  //Constructor + "animacion(anim)" que inicializa el puntero "Animacion*" (* : Indica que es un puntero)
  AnimationPlayback(Animation* anim)
    : animation(anim) {}

  // Método para asignar callback cuando se completa la animación
  AnimationPlayback& OnComplete(std::function<void()> action) {
    if (animation) {
      animation->SetOnComplete(action);
    }
    return *this;
  }

  // Método para asignar callback cuando inicia la animación
  AnimationPlayback& OnStart(std::function<void()> action) {
    if (animation) {
      animation->SetOnStart(action);
    }
    return *this;
  }

  // Método para asignar callback cuando cambia de frame
  AnimationPlayback& OnFrameChange(std::function<void(int)> action) {
    if (animation) {
      animation->SetOnFrameChange(action);
    }
    return *this;
  }

  // Método para asignar callback cuando se reinicia el loop
  AnimationPlayback& OnLoop(std::function<void()> action) {
    if (animation) {
      animation->SetOnLoop(action);
    }
    return *this;
  }

  // Método para asignar callback cuando se completa la animación
  AnimationPlayback& onAfterComplete(std::function<void()> action) {
    if (animation) {
      animation->SetOnAfterComplete(action);
    }
    return *this;
  }

  AnimationPlayback& OnMoveComplete(std::function<void()> action) {
    if (animation) {
      animation->SetOnMoveComplete(action);
    }
    return *this;
  }
};

#endif