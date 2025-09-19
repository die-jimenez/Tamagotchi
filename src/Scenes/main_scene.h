#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "scene.h"
#include "../Timer/Timer.h"
#include "src/Animations/menu.h"


class MainScene : public Scene {

private:
  //Animaciones
  Animation* eyePtr;
  Timer timerAnimation;

  //--menu
  Animation* menu_fork;
  Animation* menu_play;
  Animation* menu_dialog;


public:
  //Constructor
  MainScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
    menu_fork = new Animation(display, anim_fork, fork_length, 1, menu_icon_width, menu_icon_height);
    menu_play = new Animation(display, anim_play, play_length, 1, menu_icon_width, menu_icon_height);
    menu_dialog = new Animation(display, anim_dialog, dialog_length, 1, menu_icon_width, menu_icon_height);
  }

  ~MainScene() {
    //delete eye;
  }

  void Init() override {
  }

  // Sobrescribimos Update para reemplazar tu función original
  void Update(float _deltaTime) override {
    menu_fork->SetLoop(true);
    menu_fork->SetPosition(8, 10);
    menu_fork->Play(WHITE, deltaTime.Get());

    menu_play->SetLoop(true);
    menu_play->SetPosition(44, 10);
    menu_play->Play(WHITE, deltaTime.Get());

    menu_dialog->SetLoop(true);
    menu_dialog->SetPosition(80, 10);
    menu_dialog->Play(WHITE, deltaTime.Get());

    menu_fork->SetLoop(true);
    menu_fork->SetPosition(116, 10);
    menu_fork->Play(WHITE, deltaTime.Get());
  }

  void Render() override {}
  void OnExit() override {}
};

#endif