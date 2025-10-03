#ifndef DIALOG_SCENE_H
#define DIALOG_SCENE_H

#include "scene.h"
#include "../Timer/Timer.h"

// --- Struct de pregunta/respuesta con opción Shi/No ---
struct Dialog {
  const char* pregunta;
  const char* respuestaShi;
  const char* respuestaNo;
  const char* endButtonText;
};

// --- Array de preguntas/respuestas (editable fácilmente) ---
Dialog dialogos[] = {
  {"Quieres jugar conmigo?", "¡Sabia que dirias que si!", "Oh, que mal...", "Volver"},
  {"Te gusta la pizza?", "Genial, somos amigos!", "Bueno, cada uno con su gusto.", "Menu"},
  {"Eres un robot?", "Bienvenido a nuestro club!", "Humano confirmado...", "Salir"},
  {"Quieres descansar?", "Perfecto, tomate tu tiempo.", "Sigue adelante entonces!", "Inicio"}
};
const int dialogosCount = sizeof(dialogos) / sizeof(dialogos[0]);

class DialogScene : public Scene {
private:
  int currentDialog = 0;    // Índice de la pregunta actual
  int currentOption = 0;    // 0 = Shi, 1 = No, 2 = Botón final
  int alturaBoton = 44;
  bool answered = false;    // Para saber si ya se contestó
  const int maxChars = 20;  // límite equivalente a "¿Te gusta la pizza?"

public:
  DialogScene(Adafruit_SSD1306* _display, AnimationManager* _animationManager) {
    display = _display;
    animationManager = _animationManager;
  }

  ~DialogScene() {}

  void Init() override {
    currentDialog = random(0, dialogosCount); // Pregunta aleatoria
    currentOption = 0;
    answered = false;
  }

  void Update(float _deltaTime) override {
    DrawDialog();
    MenuNavegation();
  }

  // --- Dibuja texto centrado ---
  void DrawCenteredText(const char* text, int posX, int posY, Adafruit_SSD1306* _display) {
    int16_t x1, y1;
    uint16_t w, h;
    _display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int16_t x = posX - (w / 2);
    int16_t y = posY - (h / 2);
    _display->setCursor(x, y);
    _display->print(text);
  }

  // --- Dibuja texto en dos líneas si excede maxChars, sin cortar palabras ---
  void DrawWrappedText(const char* text, int posX, int posY, Adafruit_SSD1306* _display) {
    int len = strlen(text);
    if (len <= maxChars) {
      DrawCenteredText(text, posX, posY, _display);
    } else {
      char buffer1[64];
      char buffer2[64];
      int cutIndex = maxChars;
      for (int i = maxChars; i >= 0; i--) {
        if (text[i] == ' ') {
          cutIndex = i;
          break;
        }
      }
      strncpy(buffer1, text, cutIndex);
      buffer1[cutIndex] = '\0';
      strncpy(buffer2, text + cutIndex + (text[cutIndex] == ' ' ? 1 : 0), len - cutIndex);
      buffer2[len - cutIndex] = '\0';
      DrawCenteredText(buffer1, posX, posY, _display);
      DrawCenteredText(buffer2, posX, posY + 12, _display);
    }
  }

  void DrawDialog() {
    display->setTextSize(1);
    display->setTextColor(WHITE);

    if (!answered) {
      // Pregunta (puede ocupar 2 líneas)
      DrawWrappedText(dialogos[currentDialog].pregunta, SCREEN_WIDTH / 2, 10, display);

      // Opciones
      DrawCenteredText("Shi", SCREEN_WIDTH / 2 - 20, alturaBoton + 6, display);
      DrawCenteredText("No", SCREEN_WIDTH / 2 + 20, alturaBoton + 6, display);

      // Rectángulos de selección
      if (currentOption == 0) {
        display->drawRect((SCREEN_WIDTH / 2 - 20) - 12, alturaBoton, 24, 12, WHITE);
      } else {
        display->drawRect((SCREEN_WIDTH / 2 + 20) - 12, alturaBoton, 24, 12, WHITE);
      }
    } else {
      // Respuesta según elección
      const char* respuesta = (currentOption == 0) ? dialogos[currentDialog].respuestaShi
                                                    : dialogos[currentDialog].respuestaNo;
      DrawWrappedText(respuesta, SCREEN_WIDTH / 2, 10, display);

      // Botón final centrado
      DrawCenteredText(dialogos[currentDialog].endButtonText, SCREEN_WIDTH / 2, alturaBoton + 6, display);

      if (currentOption == 2) {
        int16_t x1, y1;
        uint16_t w, h;
        display->getTextBounds(dialogos[currentDialog].endButtonText, 0, 0, &x1, &y1, &w, &h);
        display->drawRect((SCREEN_WIDTH / 2) - (w / 2) - 2, alturaBoton, w + 4, 14, WHITE);
      }
    }
  }

  void MenuNavegation() {
    buttonL.PressEvent([this]() {
      if (!answered) currentOption = (currentOption == 0) ? 1 : 0;
    });

    buttonR.PressEvent([this]() {
      if (!answered) currentOption = (currentOption == 0) ? 1 : 0;
    });

    buttonC.PressEvent([this]() {
      if (!answered) {
        answered = true;
        // Mantener la elección de Shi o No
        // Luego se moverá al botón final
        currentOption = 2;
      } else {
        if (currentOption == 2) {
          Init();
          ChangeState("main");
        }
      }
    });
  }

  void Render() override {}
  void OnExit() override {}
};

#endif

