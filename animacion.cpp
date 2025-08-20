#include "animacion.h"
#include "src/DeltaTime/DeltaTime.h"

Animacion::Animacion(Adafruit_SSD1306* _display, const unsigned char* _sprites[], int _frameCount, int _frameRate, int _width, int _height) {
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

  //Variable configuradas
  frameInterval = 1.0 / frameRate;
}


Animacion::~Animacion() {
}

AnimacionPlayback Animacion::Play(int16_t _posX, int16_t _posY, uint16_t _color, float _deltaTime) {
  //Detiene la ejecucion
  if (forceStop) return AnimacionPlayback(this);
  if (currentFrame >= frameCount) return AnimacionPlayback(this);

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
    SetPosition(_posX, _posY);
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
        }
      }
    }
  }
  time = time + _deltaTime;
  //Este display es un puntero del "display" en el .ino. "->" hace que se trabaje con el "display" real y no con el puntero
  display->drawBitmap(posX, posY, GetCurrentSprite(), GetWidth(), GetHeight(), _color);
  if (!areEventsLoaded) areEventsLoaded = true;
  return AnimacionPlayback(this);
}



void Animacion::Stop() {
  time = 0;
  currentFrame = 0;
  isPlaying = false;
  hasCompleted = false;
  areEventsLoaded = false;
  forceStop = true;
}

void Animacion::Pause() {
  isPlaying = false;
  forceStop = true;
}

void Animacion::Continue() {
  forceStop = false;
}

void Animacion::SetLoop(bool _isLoop) {
  isLoop = _isLoop;
}



void Animacion::SetPosition(int _x, int _y) {
  if (isCenterMode) {
    posX = _x - GetWidth() / 2;
    posY = _y - GetHeight() / 2;
  } else {
    posX = _x;
    posY = _y;
  }
}

void Animacion::SetCenterMode(bool _val) {
  isCenterMode = _val;
}

int Animacion::GetWidth() {
  return width;
}

int Animacion::GetHeight() {
  return height;
}

int Animacion::GetPosX() {
  return posX;
}

int Animacion::GetPosY() {
  return posY;
}

const unsigned char* Animacion::GetCurrentSprite() {
  return sprites[currentFrame];
}

const unsigned char* Animacion::GetSprite(int index) {
  return sprites[index];
}

// Métodos privados para disparar callbacks
void Animacion::TriggerOnComplete() {
  if (onCompleteCallback) {
    onCompleteCallback();
  }
}

void Animacion::TriggerOnStart() {
  if (onStartCallback) {
    onStartCallback();
  }
}

void Animacion::TriggerOnFrameChange(int frame) {
  if (onFrameChangeCallback) {
    onFrameChangeCallback(frame);
  }
}

void Animacion::TriggerOnLoop() {
  if (onLoopCallback) {
    onLoopCallback();
  }
}