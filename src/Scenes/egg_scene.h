#ifndef EGG_SCENE_H
#define EGG_SCENE_H

#include "scene.h"
#include "../Timer/Timer.h"
#include "../Animations/egg.h"



class EggScene : public Scene {

private:
  Timer timerAnimation;
  bool isEggOpen = false;

  //Animaciones
  Animation egg_close{ anim_egg_close, egg_close_length, egg_close_framerate, egg_width, egg_height };
  Animation egg_open{ anim_egg_open, egg_open_length, egg_open_framerate, egg_width, egg_height };


public:
  //Constructor
  EggScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
  }

  ~EggScene() {}

  void Init() override {
  }

  // Sobrescribimos Update para reemplazar tu función original
  void Update(float _deltaTime) override {
    if (!isEggOpen) {
      egg_close.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
      egg_close.Play(display, WHITE, deltaTime.Get());
      buttonL.PressEvent([this]() {
        OpenEgg();
      });
      buttonC.PressEvent([this]() {
        OpenEgg();
      });
      buttonR.PressEvent([this]() {
        OpenEgg();
      });
    }  //
    else {
      DrawCenteredText("PIPO", SCREEN_WIDTH / 2, 20, display);
      buttonL.PressEvent([this]() {
        GoToMainMenu();
      });
      buttonC.PressEvent([this]() {
        GoToMainMenu();
      });
      buttonR.PressEvent([this]() {
        GoToMainMenu();
      });
    }
  }

  void OpenEgg() {
    egg_close.Stop();
    egg_open.SetLoop(false);
    egg_open.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    animationManager->Play(&egg_open);
    egg_open.SetOnComplete([this] {
      isEggOpen = true;
    });
  }


  void Render() override {}
  void OnExit() override {}
};

#endif