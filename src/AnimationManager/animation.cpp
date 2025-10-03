#include "animation.h"
//El display lo paso por el método Play(), cual es molesto porque se repite mucho pero evito problema de 
//referencias al inicializar el objeto, que empezo a ser complicado cuando crees las subclases de las escenas

Animation::Animation(const unsigned char* _sprites[], int _frameCount, int _frameRate, int _width, int _height) {
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

  //Variable por default para la interpolacion
  isMoving = false;
  startPosX = 0;
  startPosY = 0;
  targetPosX = 0;
  targetPosY = 0;
  moveStartTime = 0;
  moveDuration = 0;
  interpolationType = 1;  // Smooth por defecto

  //Variable configuradas
  frameInterval = 1.0 / frameRate;
}

Animation::~Animation() {
}


AnimationPlayback Animation::Play(Adafruit_SSD1306* display, uint16_t _color, float _deltaTime) {
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
  //Actualiza el movimiento de la interpolacion
  //UpdateMovement(millis());
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

void Animation::Restart() {
    Stop();
    forceStop = false;  // ← La diferencia clave con Stop()
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
  SetOnStart([]() {});
  SetOnLoop([]() {});
  SetOnComplete([]() {});
  SetOnFrameChange([](int) {});
  SetOnAfterComplete([]() {});
  SetOnMoveComplete([]() {});
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


//Interpolacion -----------------------------------------------------
// void Animation::MoveTo(float endX, float endY, unsigned long duration, int type) {
//   if (duration == 0) {
//     // Movimiento instantáneo
//     SetPosition(endX, endY);
//     return;
//   }

//   // Configurar interpolación
//   startPosX = isCenterMode ? posX + GetWidth() / 2 : posX;
//   startPosY = isCenterMode ? posY + GetHeight() / 2 : posY;
//   targetPosX = endX;
//   targetPosY = endY;
//   moveStartTime = millis();
//   moveDuration = duration;
//   interpolationType = type;
//   isMoving = true;
// }

// // AGREGAR ESTA LÍNEA ANTES DE DIBUJAR
// void Animation::UpdateMovement(unsigned long currentTime) {
//   if (!isMoving) return;

//   unsigned long elapsed = currentTime - moveStartTime;

//   if (elapsed >= moveDuration) {
//     // Movimiento completado
//     SetPosition(targetPosX, targetPosY);
//     isMoving = false;

//     // Disparar callback si existe
//     if (onMoveCompleteCallback) {
//       onMoveCompleteCallback();
//     }
//     return;
//   }

//   // Calcular posición interpolada
//   double xValues[2] = { 0, (double)moveDuration };
//   double yValuesX[2] = { (double)startPosX, (double)targetPosX };
//   double yValuesY[2] = { (double)startPosY, (double)targetPosY };

//   float newX, newY;

//   switch (interpolationType) {
//     case 0:  // Linear
//       newX = (float)Interpolation::Linear(xValues, yValuesX, 2, (double)elapsed, false);
//       newY = (float)Interpolation::Linear(xValues, yValuesY, 2, (double)elapsed, false);
//       break;

//     case 1:  // Smooth (por defecto)
//       newX = (float)Interpolation::SmoothStep(xValues, yValuesX, 2, (double)elapsed);
//       newY = (float)Interpolation::SmoothStep(xValues, yValuesY, 2, (double)elapsed);
//       break;

//     case 2:  // Spline
//       newX = (float)Interpolation::CatmullSpline(xValues, yValuesX, 2, (double)elapsed);
//       newY = (float)Interpolation::CatmullSpline(xValues, yValuesY, 2, (double)elapsed);
//       break;

//     default:
//       newX = (float)Interpolation::SmoothStep(xValues, yValuesX, 2, (double)elapsed);
//       newY = (float)Interpolation::SmoothStep(xValues, yValuesY, 2, (double)elapsed);
//       break;
//   }

//   SetPosition(newX, newY);
// }

// bool Animation::IsMoving() const {
//   return isMoving;
// }



// Métodos privados para disparar callbacks -------------------------
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

void Animation::TriggerOnMoveComplete() {
  if (onMoveCompleteCallback) {
    onMoveCompleteCallback();
  }
}
