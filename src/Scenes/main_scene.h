#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "scene.h"
#include "../Timer/Timer.h"
#include "../Animations/menu.h"
#include "../Animations/doggo.h"



class MainScene : public Scene {

private:
  //Iconos de menu
  Animation menu_fork{ anim_fork, fork_length, 1, menu_icon_width, menu_icon_height };
  Animation menu_play{ anim_play, play_length, 1, menu_icon_width, menu_icon_height };
  Animation menu_dialog{ anim_dialog, dialog_length, 1, menu_icon_width, menu_icon_height };

  //Aniamciones de pipo
  Animation dog_walk{ anim_dog_walk, dog_walk_length, dog_walk_framerate, dog_width, dog_height };
  Animation dog_bite{ anim_dog_bite, dog_bite_length, dog_bite_framerate, dog_width, dog_height };
  Animation dog_run{ anim_dog_run, dog_run_length, dog_run_framerate, dog_width, dog_height };
  Animation dog_sit{ anim_dog_sit, dog_sit_length, dog_sit_framerate, dog_width, dog_height };
  Animation dog_eat{ anim_dog_eat, dog_eat_length, dog_eat_framerate, dog_width, dog_height };

  int currentOption = 0;
  Timer timerAnimation;



public:
  //Constructor
  MainScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
  }

  ~MainScene() {}

  void Init() override {
  }

  // Sobrescribimos Update para reemplazar tu función original
  void Update(float _deltaTime) override {
    DrawMenu();
    MenuNavegation();

    DogWalking();
  }



  void DrawMenu() {
    menu_fork.SetLoop(true);
    menu_fork.SetPosition(8, 10);
    menu_fork.Play(display, WHITE, deltaTime.Get());
    if (currentOption == 0) ApplyDitherToRect(0, 0, 18, 20, display, true);

    menu_play.SetLoop(true);
    menu_play.SetPosition(44, 10);
    menu_play.Play(display, WHITE, deltaTime.Get());
    if (currentOption == 1) ApplyDitherToRect(33, 0, 21, 20, display, true);

    menu_dialog.SetLoop(true);
    menu_dialog.SetPosition(80, 10);
    menu_dialog.Play(display, WHITE, deltaTime.Get());
    if (currentOption == 2) ApplyDitherToRect(69, 0, 22, 20, display, true);

    menu_fork.SetLoop(true);
    menu_fork.SetPosition(116, 10);
    menu_fork.Play(display, WHITE, deltaTime.Get());
    if (currentOption == 3) ApplyDitherToRect(108, 0, 18, 20, display, true);
  }


  void DogWalking() {
    dog_walk.SetLoop(true);
    dog_walk.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 15);
    dog_walk.Play(display, WHITE, deltaTime.Get());

    dog_run.SetLoop(true);
    //dog_run.SetPosition(30, SCREEN_HEIGHT - 26);
    //dog_run.Play(display, WHITE, deltaTime.Get());

    // dog_sit.SetLoop(true);
    // dog_sit.SetPosition((SCREEN_WIDTH / 2) + 40, SCREEN_HEIGHT - 15);
    // dog_sit.Play(display, WHITE, deltaTime.Get());

    // dog_eat.SetLoop(true);
    // dog_eat.SetPosition((SCREEN_WIDTH / 2) - 45, SCREEN_HEIGHT - 15);
    // dog_eat.Play(display, WHITE, deltaTime.Get());
  }


  void MenuNavegation() {
    buttonL.PressEvent([this]() {
      if (currentOption == 0) {
        currentOption = 3;
        return;
      }
      currentOption--;
    });

    buttonR.PressEvent([this]() {
      if (currentOption == 3) {
        currentOption = 0;
        return;
      }
      currentOption++;
    });

    buttonC.PressEvent([this]() {
      switch (currentOption) {
        case 0:
          Serial.println("hola");
          ChangeState("food");
          break;
        case 1:
          ChangeState("game");
          break;
        case 2:
          ChangeState("dialog");
          break;
        case 3:
          break;
      }
    });
  }

  //Funciones que dibujan e implementan toda la lógica de cada pantalla
  void PreloadMain() {
    ChangeState("main");
  }


  void Render() override {}
  void OnExit() override {}
};

#endif