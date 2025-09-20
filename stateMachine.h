#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "src/AnimationManager/animationManager.h"
// Declaración de solo referencias
extern AnimationManager animationManager;
extern String state;
extern String lastState;


void ChangeState(String _newState) {
  animationManager.StopAll();
  lastState = state;
  state = _newState;
}

void GoToMainMenu() {
  //La idea es siempre pasar por el preload para cargar eventos o animaciones
  ChangeState("preload_main");
}


#endif