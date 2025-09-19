#ifndef CREDITSSCENE_SCENE_H
#define CREDITSSCENE_SCENE_H

#include "scene.h"
#include "../Timer/Timer.h"
#include "src/Animations/eye.h"


class CreditsScene: public Scene {

private:
  //Animaciones
  Animation* eyePtr;
  Timer timerAnimation;

public:
  //Constructor
  CreditsScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
    eyePtr = new Animation(display, anim_eye, eye_length, eye_framerate, eye_width, eye_height);
  }

  ~CreditsScene() {
    //delete eye;
  }

  void Init() override {
  }

  // Sobrescribimos Update para reemplazar tu función original
  void Update(float _deltaTime) override {
    DrawTextInRect("Creado por: ", 0, 0, 1);
    display->setCursor(0, 15);
    display->print("Diego Jimenez");
    display->display();
    delay(1000);

    // Pantalla en negro
    display->clearDisplay();
    delay(1000);

    // Animación del ojo
    ChangeState("waiting");
    eyePtr->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    animationManager->PlayOneShot(eyePtr);

    //timerAnimation y ChangeState() son miembros de la clase (Scene).
    //Dentro de la lambda, el compilador no sabe qué this usar. POR ESO SE PONE EL "this"
    eyePtr->SetOnComplete([this] {
      delay(500);
      ChangeState("egg_closed");
    });
  }

  void Render() override {}
  void OnExit() override {}
};

#endif