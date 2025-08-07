#include "animacion.h"
#include <Adafruit_SSD1306.h>
#include "src/DeltaTime/DeltaTime.h"

Animacion::Animacion(const unsigned char* _sprites[], int _frameRate, int _width, int _height) {
  sprites = _sprites;
  frameRate = _frameRate;
  width = _width;
  height = _height;
  isLoop = true;      // Inicialización añadida
  isPlaying = false;  // Inicialización añadida
}

Animacion::~Animacion() {
}

void Animacion::play() {
  deltaTime.Run();
  Serial.println("animacion de huevo");
  Serial.print(deltaTime.Get());
  //if (!isPlaying || frameCount == 0) return;

  // uint32_t currentTime = millis();
  // if (currentTime - lastFrameTime >= frameInterval) {
  //     lastFrameTime = currentTime;

  //     uint16_t effectiveFrameCount = (maxPlayFrames > 0) ? maxPlayFrames : frameCount;
  //     uint16_t nextFrame = currentFrame + 1;

  //     if (nextFrame >= effectiveFrameCount) {
  //         if (loopEnabled) {
  //             currentFrame = 0;
  //         } else {
  //             pause();
  //         }
  //     } else {
  //         currentFrame = nextFrame;
  //     }
  // }
}


int Animacion::GetWidth() {
  return width;
}

int Animacion::GetHeight() {
  return height;
}

const unsigned char* Animacion::GetSprite(int index) {
  return sprites[index];
}