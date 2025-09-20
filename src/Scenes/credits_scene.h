#ifndef CREDITSSCENE_SCENE_H
#define CREDITSSCENE_SCENE_H

#include "scene.h"
#include "../Timer/Timer.h"
#include "../Animations/eye.h"


class CreditsScene : public Scene {

private:
  //Animaciones
  //C++ cuando estas en un private de un objeto, entiende los "()" como una función y las "{}" como inicializador de un objeto
  Animation eye{anim_eye, eye_length, eye_framerate, eye_width, eye_height};
  Timer timerAnimation;

public:
  //Constructor
  CreditsScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
  }

  ~CreditsScene() {
    //delete eye;
  }

  void Init() override {
  }

  // Sobrescribimos Update para reemplazar tu función original
  void Update(float _deltaTime) override {
    DrawTextInRect("Creado por: ", 0, 0, 1, display);
    display->setCursor(0, 15);
    display->print("Diego Jimenez");
    display->display();
    delay(1000);

    // Pantalla en negro
    display->clearDisplay();
    delay(1000);

    // Animación del ojo
    ChangeState("waiting");
    eye.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    animationManager->PlayOneShot(&eye);

    //timerAnimation y ChangeState() son miembros de la clase (Scene).
    //Dentro de la lambda, el compilador no sabe qué this usar. POR ESO SE PONE EL "this"
    eye.SetOnComplete([this] {
      delay(500);
      ChangeState("egg_closed");
    });
  }

  void Render() override {}
  void OnExit() override {}
};

#endif