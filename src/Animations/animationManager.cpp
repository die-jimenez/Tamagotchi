#include "animationManager.h"

AnimationManager::AnimationManager(Adafruit_SSD1306* _display) {
  display = _display;
}

AnimationManager::~AnimationManager() {
}


void AnimationManager::Update(float _deltaTime) {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    animations[i]->Play(WHITE, _deltaTime);
  }
}

void AnimationManager::Add(Animation* _anim) {
  for (int i = 0; i < maxAnimations; i++) {
    if (!isEmptyAnimation(i)) continue;
    animations[i] = _anim;
    // Serial.print("-AnimationManager: Se guardo una animacion en la posicion ");
    // Serial.println(i);
    return;
  }
}

void AnimationManager::Remove(Animation* _anim) {
  for (int i = 0; i < maxAnimations; i++) {
    if (isEmptyAnimation(i)) continue;
    if (animations[i] == _anim) {
      animations[i]->Stop();
      animations[i] = nullptr;
      // Serial.print("-AnimationManager: Se elimino la animacion en la posicion ");
      // Serial.println(i);
      return;
    }
  }
}

void AnimationManager::RemoveAll() {
  for (int i = 0; i < maxAnimations; i++) {
    animations[i] = nullptr;
  }
  Serial.println("-AnimationManager: Se eliminaron todas las animaciones ");
}

void AnimationManager::PlayOneShot(Animation* _anim) {
  Add(_anim);
  _anim->SetLoop(false);

  //La función anonima no tiene acceso a la variables de la función por defecto.
  //"[_anim, this]": Primero agarra la ref de "_anim" y luego con "this" toma la referencia de la calse
  _anim->SetAfterOnComplete([_anim, this]() {
    this->Remove(_anim);
    Serial.println("Termino playOneShot");
  });
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
