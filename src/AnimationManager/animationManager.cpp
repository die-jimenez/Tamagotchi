#include "animationManager.h"

AnimationManager::AnimationManager(Adafruit_SSD1306* _display) {
  display = _display;
}

AnimationManager::~AnimationManager() {
}

void AnimationManager::Update(float _deltaTime) {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    animations[i]->Play(display, WHITE, _deltaTime);
  }
}

//Reproducir las animaciones con este "Play" de animationManager se diferencia del "Play" de cada animacion
//En que esta version debes ejecutarla a modo de evento, se ha de reproducir una sola vez, 
//mientras que desde las animaciones, debe estar en el Update
void AnimationManager::Play(Animation* _anim) {
  for (int i = 0; i < maxAnimations; i++) {
    if (!isEmptyAnimation(i)) continue;
    animations[i] = _anim;
    return;
  }
}

void AnimationManager::PlayOneShot(Animation* _anim) {
  Play(_anim);
  _anim->SetLoop(false);
  //La función anonima no tiene acceso a la variables de la función por defecto.
  //"[_anim, this]": Primero agarra la ref de "_anim" y luego con "this" toma la referencia de la calse
  _anim->SetOnAfterComplete([_anim, this]() {
    this->Remove(_anim);
    Serial.println("Termino playOneShot");
  });
}

void AnimationManager::Stop(Animation* _anim) {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    animations[i]->Stop();
    return;
  }
}

void AnimationManager::StopAll() {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    animations[i]->Stop();
  }
}

void AnimationManager::Remove(Animation* _anim) {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    if (animations[i] == _anim) {
      animations[i]->Stop();
      animations[i] = nullptr;
      return;
    }
  }
}

void AnimationManager::RemoveAll() {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    animations[i]->Stop();
    animations[i] = nullptr;
  }
  Serial.println("-AnimationManager: Se eliminaron todas las animaciones ");
}


void AnimationManager::Continue(Animation* _anim) {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    if (animations[i] == _anim) {
      animations[i]->Continue();
      return;
    }
  }
}

void AnimationManager::ContinueAll() {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    animations[i]->Continue();
  }
}

bool AnimationManager::isEmptyAnimation(int index) {
  if (animations[index]) return false;
  else return true;
}

void AnimationManager::PrintAnimationsSpaces() {
  Serial.println("-AnimationManager: Espacios del array animations");
  for (int i = 0; i < maxAnimations; i++) {
    Serial.print("Espacio ");
    Serial.print(i);
    Serial.print(": ");
    if (isEmptyAnimation(i)) Serial.println("Vacio");
    else Serial.println("Lleno");
  }
  Serial.println();
  Serial.println();
}
