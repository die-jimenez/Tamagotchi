#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//Lista de todos los estados
String state = "credits"
  //wainting
  //creepy_eye
  //egg_closed
  //egg_opened
  //main_menu
  ;


String lastState;
void ChangeState(String _newState) {
  animationManager.StopAll();
  lastState = state;
  state = _newState;
}

void GoToMainMenu() {
  //La idea es siempre pasar por el preload para cargar eventos o animaciones
  ChangeState("preload_main_menu");
}


#endif