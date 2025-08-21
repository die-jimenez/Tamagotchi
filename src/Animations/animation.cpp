#include "animation.h"
#include "../DeltaTime/DeltaTime.h"

Animation::Animation(Adafruit_SSD1306* _display, const unsigned char* _sprites[], int _frameCount, int _frameRate, int _width, int _height) {
  display = _display;
  sprites = _sprites;
  //Compruebo que los datos son correctos
  frameRate = _frameRate >= 0 ? _frameRate : 1;
  frameCount = _frameCount >= 0 ? _frameCount : 1;
  width = _width;
  height = _height;

  //Variables por default
  isLoop = true;
  isPlaying = false;
  forceStop = false;
  currentFrame = 0;
  time = 0.0f;
  hasCompleted = false;
  areEventsLoaded = false;

  //Variable configuradas
  frameInterval = 1.0 / frameRate;
}

Animation::~Animation() {
}


AnimationPlayback Animation::Play(uint16_t _color, float _deltaTime) {
  //Detiene la ejecucion
  if (forceStop) return AnimationPlayback(this);
  if (currentFrame >= frameCount) return AnimationPlayback(this);

  //Espera que se carguen los eventos tras la primera ejecucion
  //Explicacion: "animacion.Play(x).OnStart(...)" Ejecuta primero solo "animacion.Play(x)" y despues carga los eventos. Asi que estos funcionan en la segunda ejecucion
  if (!isPlaying && areEventsLoaded) {
    isPlaying = true;
    hasCompleted = false;
    TriggerOnStart();
  }
  //Cambio de frame
  if (time >= frameInterval) {
    time = 0;
    currentFrame++;
    TriggerOnFrameChange(currentFrame);

    //Reinicia o acaba la animacion
    if (currentFrame >= frameCount) {
      if (isLoop) {
        currentFrame = 0;
        TriggerOnLoop();
      } else {
        currentFrame = frameCount - 1;
        if (!hasCompleted) {
          hasCompleted = true;
          TriggerOnComplete();
          TriggerOnAfterComplete();
        }
      }
    }
  }
  time = time + _deltaTime;
  //Este display es un puntero del "display" en el .ino. "->" hace que se trabaje con el "display" real y no con el puntero
  display->drawBitmap(posX, posY, GetCurrentSprite(), GetWidth(), GetHeight(), _color);
  if (!areEventsLoaded) areEventsLoaded = true;
  return AnimationPlayback(this);
}



void Animation::Stop() {
  time = 0;
  currentFrame = 0;
  isPlaying = false;
  hasCompleted = false;
  areEventsLoaded = false;
  forceStop = true;
  ClearEvents();
}

void Animation::Pause() {
  isPlaying = false;
  forceStop = true;
}

void Animation::Continue() {
  forceStop = false;
}

void Animation::SetLoop(bool _isLoop) {
  isLoop = _isLoop;
}

void Animation::SetPosition(int _x, int _y) {
  if (isCenterMode) {
    posX = _x - GetWidth() / 2;
    posY = _y - GetHeight() / 2;
  } else {
    posX = _x;
    posY = _y;
  }
}

void Animation::SetCenterMode(bool _val) {
  isCenterMode = _val;
}

void Animation::ClearEvents() {
  SetOnStart([](){});
  SetOnLoop([](){});
  SetOnComplete([](){});
  SetOnFrameChange([](int){});
  SetOnAfterComplete([](){});
}

int Animation::GetWidth() {
  return width;
}
int Animation::GetHeight() {
  return height;
}
int Animation::GetPosX() {
  return posX;
}
int Animation::GetPosY() {
  return posY;
}


const unsigned char* Animation::GetCurrentSprite() {
  return sprites[currentFrame];
}
const unsigned char* Animation::GetSprite(int index) {
  return sprites[index];
}

// Métodos privados para disparar callbacks
void Animation::TriggerOnComplete() {
  if (onCompleteCallback) {
    onCompleteCallback();
  }
}

void Animation::TriggerOnStart() {
  if (onStartCallback) {
    onStartCallback();
  }
}

void Animation::TriggerOnFrameChange(int frame) {
  if (onFrameChangeCallback) {
    onFrameChangeCallback(frame);
  }
}

void Animation::TriggerOnLoop() {
  if (onLoopCallback) {
    onLoopCallback();
  }
}

void Animation::TriggerOnAfterComplete() {
  if (onAfterCompleteCallback) {
    onAfterCompleteCallback();
  }
}
