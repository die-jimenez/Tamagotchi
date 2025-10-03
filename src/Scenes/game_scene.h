#ifndef GAME_SCENE_H
#define GAME_SCENE_H

//Codigo hecho con ChatGpt y luego modificado por mi

#include "scene.h"
#include "../Timer/Timer.h"

class GameScene : public Scene {
private:
  enum class GameState { INTRO,
                         PLAYING,
                         VICTORY,
                         GAME_OVER };
  GameState currentState = GameState::INTRO;

  // --- Intro pelota
  int ballX = 0;
  int ballY = 20;
  int ballVelX = 6;
  int ballVelY = -4;
  bool introFinished = false;
  Timer introTimer;
  Timer delayBeforePlay;

  // --- Perro (jugador)
  int dinoX = 25;
  int dinoY = 42;
  int dinoSize = 10;
  int velocityY = 0;
  int dinoY_floor = 42;
  bool isJumping = false;
  bool isLanding = false;

  // --- Pelota objetivo
  int targetBallX = 0;
  float targetBallY = 40.0f;
  float targetBallVelY = -3.0f;
  int targetBallSize = 8;
  bool ballSpawned = false;
  Timer ballSpawnTimer;
  float ballSpawnTime = 0.0f;

  // --- Obstáculos
  int obsWidth = 8;
  int obsHeight = 10;
  int espaciadoEntreObs;
  int distanciaMinimaEntreObs = 50;
  int obsX_1 = SCREEN_WIDTH;
  int obsY_1 = 40;
  int obsX_2 = SCREEN_WIDTH + 60;
  int obsY_2 = 40;

  // --- Juego
  bool isGameOver = false;
  int score = 0;

  // Pipo
  Animation dog_run{ anim_dog_run, dog_run_length, dog_run_framerate, dog_width, dog_height };
  Animation dog_jump{ anim_dog_jump, dog_jump_length, dog_jump_framerate, dog_width, dog_height };
  Animation dog_land{ anim_dog_land, dog_land_length, dog_land_framerate, dog_width, dog_height };



public:
  GameScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
  }

  ~GameScene() {}

  void Init() override {
    currentState = GameState::INTRO;
    introFinished = false;

    // Reset intro ball
    ballX = 0;
    ballY = 20;
    ballVelX = 6;
    ballVelY = -4;

    introTimer.SetDuration(3.0f);
    introTimer.Start();
    delayBeforePlay.SetDuration(1.0f);

    // Reset player
    dinoY = dinoY_floor;
    velocityY = 0;
    isJumping = false;
    isLanding = false;

    // Reset target ball
    ballSpawned = false;
    ballSpawnTime = random(1000, 1300) / 100.0f;  // 10-13 s
    ballSpawnTimer.SetDuration(ballSpawnTime);
    targetBallX = SCREEN_WIDTH + 50;
    targetBallY = 20.0f;
    targetBallVelY = -6.0f;

    // Reset game
    isGameOver = false;
    score = 0;
    obsX_1 = SCREEN_WIDTH;
    espaciadoEntreObs = random(40, 60);
    obsX_2 = SCREEN_WIDTH + espaciadoEntreObs;

    Serial.println("GameScene inicializada");
    Serial.print("Pelota aparecerá en: ");
    Serial.print(ballSpawnTime);
    Serial.println(" segundos");
  }

  void Update(float _deltaTime) override {
    introTimer.Update(_deltaTime);
    delayBeforePlay.Update(_deltaTime);
    ballSpawnTimer.Update(_deltaTime);

    switch (currentState) {
      case GameState::INTRO: UpdateIntro(_deltaTime); break;
      case GameState::PLAYING: UpdatePlaying(_deltaTime); break;
      case GameState::VICTORY: UpdateVictory(_deltaTime); break;
      case GameState::GAME_OVER: UpdateGameOver(_deltaTime); break;
    }

    Render();
  }

  void Render() override {
    switch (currentState) {
      case GameState::INTRO: RenderIntro(); break;
      case GameState::PLAYING: RenderPlaying(); break;
      case GameState::VICTORY: RenderVictory(); break;
      case GameState::GAME_OVER: RenderGameOver(); break;
    }
  }

private:
  // --- INTRO
  void UpdateIntro(float _dt) {
    ballX += ballVelX;
    ballY += ballVelY;
    ballVelY += 1;

    if (ballY > 50) {
      ballY = 50;
      ballVelY = -ballVelY * 0.6;
      if (abs(ballVelY) < 2) ballVelY = 0;
    }

    if (ballX > SCREEN_WIDTH + 10 && !introFinished) {
      introFinished = true;
      delayBeforePlay.Start();
      Serial.println("Intro terminada, esperando...");
    }

    if (introFinished && delayBeforePlay.IsFinished()) {
      currentState = GameState::PLAYING;
      ballSpawnTimer.Start();
      Serial.println("¡Juego iniciado!");
    }
  }

  void RenderIntro() {
    display->fillCircle(ballX, ballY, 5, WHITE);
    display->drawLine(0, 50, SCREEN_WIDTH, 50, WHITE);

    if (introFinished) {
      DrawCenteredText("Get Ready!", SCREEN_WIDTH / 2, 10, display);
    }
  }


  void Jump() {
    velocityY = -7;
    isJumping = true;
  }


  // --- PLAYING
  void UpdatePlaying(float _dt) {
    // Salto jugador
    buttonC.PressEvent([this]() {
      if (!isJumping) {
        Jump();
      }
    });

    // Física jugador
    dinoY += velocityY;
    velocityY += 1;
    //--aterrizando
    if (velocityY == 0 && isJumping && !isLanding) {
      isLanding = true;
    }
    //--corriendo en tierra
    if (dinoY >= dinoY_floor) {
      dinoY = dinoY_floor;
      isJumping = false;
      isLanding = false;
      velocityY = 0;
    }

    // Pelota objetivo
    UpdateTargetBall();

    // Obstáculos
    obsX_1 -= 3;
    if (obsX_1 + obsWidth < 0) {
      ReSpawnObstacle(obsX_1);
      score++;
    }
    obsX_2 -= 3;
    if (obsX_2 + obsWidth < 0) {
      ReSpawnObstacle(obsX_2);
      score++;
    }

    // Colisiones con obstáculos
    if (CheckCollision(dinoX, dinoY, dinoSize, dinoSize,
                       obsX_1, obsY_1, obsWidth, obsHeight)
        || CheckCollision(dinoX, dinoY, dinoSize, dinoSize,
                          obsX_2, obsY_2, obsWidth, obsHeight)) {
      currentState = GameState::GAME_OVER;
    }
  }

  void UpdateTargetBall() {
    if (!ballSpawned) {
      if (ballSpawnTimer.IsFinished()) {
        SpawnTargetBall();
      }
      return;
    }

    targetBallX -= 1;
    ApplyBallPhysics();

    if (targetBallX + targetBallSize < 0) {
      RespawnTargetBall();
    }

    if (CheckCollision(dinoX, dinoY, dinoSize, dinoSize,
                       targetBallX, (int)targetBallY - targetBallSize / 2,
                       targetBallSize, targetBallSize)) {
      currentState = GameState::VICTORY;
      Serial.println("¡VICTORIA! ¡Pelota atrapada!");
    }
  }

  void SpawnTargetBall() {
    ballSpawned = true;
    targetBallX = SCREEN_WIDTH;
    targetBallY = 20.0f;
    targetBallVelY = -3.0f;
    Serial.println("¡Pelota objetivo spawneada!");
  }

  void RespawnTargetBall() {
    targetBallX = SCREEN_WIDTH;
    targetBallY = 20.0f;
    targetBallVelY = -6.0f;
    Serial.println("Pelota perdida, reapareciendo...");
  }

  void ApplyBallPhysics() {
    targetBallY += targetBallVelY;
    targetBallVelY += 0.5f;

    if (targetBallY >= 50.0f) {
      targetBallY = 50.0f;
      targetBallVelY = -targetBallVelY * 0.8f;

      Serial.print("Rebote! VelY: ");
      Serial.println(targetBallVelY);

      if (abs(targetBallVelY) < 2.5f) {
        targetBallVelY = -5.0f;
      }
    }
  }

  void ReSpawnObstacle(int& _obsPosX) {
    espaciadoEntreObs = random(10, 30);
    _obsPosX = SCREEN_WIDTH + espaciadoEntreObs;
    if (abs(obsX_1 - obsX_2) < distanciaMinimaEntreObs) {
      _obsPosX = SCREEN_WIDTH + distanciaMinimaEntreObs;
    }
  }

  void RenderPlaying() {
    //Suelo
    display->drawLine(0, 50, SCREEN_WIDTH, 50, WHITE);

    //Pipo
    if (!isJumping && !isLanding) {
      dog_run.SetLoop(true);
      dog_run.SetPosition(dinoX, dinoY);
      dog_run.Play(display, WHITE, deltaTime.Get());
    } else if (isJumping && !isLanding) {
      dog_jump.SetPosition(dinoX, dinoY);
      dog_jump.Play(display, WHITE, deltaTime.Get());
    } else if (isJumping && isLanding) {
      dog_land.SetPosition(dinoX, dinoY);
      dog_land.Play(display, WHITE, deltaTime.Get());
    }
    //display->fillRect(dinoX, dinoY, dinoSize, dinoSize, WHITE);

    //Obstaculos
    display->fillRect(obsX_1, obsY_1, obsWidth, obsHeight, WHITE);
    display->fillRect(obsX_2, obsY_2, obsWidth, obsHeight, WHITE);

    if (ballSpawned) {
      display->fillCircle(targetBallX + targetBallSize / 2,
                          (int)targetBallY - targetBallSize / 2,
                          targetBallSize / 2, WHITE);
    }

    //Puntaje
    display->setCursor(75, 0);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->print("Score:");
    display->print(score);
  }

  // --- VICTORY
  void UpdateVictory(float _dt) {
    buttonL.PressEvent([this]() {
      ChangeState("main");
      Init();
    });
    buttonC.PressEvent([this]() {
      ChangeState("main");
      Init();
    });
    buttonR.PressEvent([this]() {
      ChangeState("main");
      Init();
    });
  }

  void RenderVictory() {
    display->fillRect(5, 5, SCREEN_WIDTH - 10, 55, BLACK);
    display->drawRect(5, 5, SCREEN_WIDTH - 10, 55, WHITE);

    DrawCenteredText("PIPO FELIZ!", SCREEN_WIDTH / 2, 15, display);
    DrawCenteredText("He got", SCREEN_WIDTH / 2, 27, display);
    DrawCenteredText("the ball!", SCREEN_WIDTH / 2, 37, display);

    display->setTextSize(1);
    display->setCursor(10, 48);
    display->print("Score: ");
    display->print(score);
  }

  // --- GAME OVER
  void UpdateGameOver(float _dt) {
    buttonL.PressEvent([this]() {
      ChangeState("main");
      Init();
    });
    buttonC.PressEvent([this]() {
      ChangeState("main");
      Init();
    });
    buttonR.PressEvent([this]() {
      ChangeState("main");
      Init();
    });
  }

  void RenderGameOver() {
    DrawCenteredText("GAME OVER", SCREEN_WIDTH / 2, 15, display);
    DrawCenteredText("Press any btn", SCREEN_WIDTH / 2, 30, display);
  }

  // --- UTIL
  bool CheckCollision(int x1, int y1, int w1, int h1,
                      int x2, int y2, int w2, int h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
  }
};

#endif
