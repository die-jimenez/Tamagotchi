#include "animacion.h"
#include "src/DeltaTime/DeltaTime.h"

Animacion::Animacion(const unsigned char* _sprites[], int _frameRate, int _frameCount, int _width, int _height) {
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

void Animacion::Play(float _deltaTime) {
  if (forceStop) {
    return;
  }

  isPlaying = true;
  if (time >= frameInterval && currentFrame < frameCount) {
    time = 0;
    currentFrame++;

    if (isLoop && currentFrame >= frameCount) {
      currentFrame = 0;
    }
    else if (!isLoop && currentFrame >= frameCount) {
      currentFrame = frameCount-1;
    }
  }
  time = time + _deltaTime;
}



void Animacion::Stop() {
  time = 0;
  currentFrame = 0;
  isPlaying = false;
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
  posX = _x;
  posY = _y;
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