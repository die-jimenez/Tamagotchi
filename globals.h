#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>






//Libreria de dibujo: Codigo extraido de: https://github.com/FluxGarage/RoboEyes
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "src/AnimationManager/animationManager.h"
AnimationManager animationManager(&display);



#include "src/DeltaTime/DeltaTime.h"
DeltaTime deltaTime;
float humanTime;

#include "src/Buttons/button.h"
Button buttonL(7, "LEFT");    //0
Button buttonC(6, "CENTER");  //1
Button buttonR(5, "RIGHT");   //3 y... 4

#include "src/Timer/Timer.h"
Timer timerAnimation;

//Animaciones

#include "src/AnimationManager/animation.h"

//--huevo
#include "src/Animations/egg.h"
Animation egg_idle(&display, anim_egg_idle, egg_idle_length, egg_idle_framerate, egg_width, egg_height);
Animation egg_open(&display, anim_egg_open, egg_open_length, egg_open_framerate, egg_width, egg_height);
// Animation eye(&display, anim_eye, eye_length, eye_framerate, eye_width, eye_height);

//--doggo
#include "src/Animations/doggo.h"
Animation dog_walk(&display, anim_dog_walk, dog_walk_length, dog_walk_framerate, dog_width, dog_height);
Animation dog_bite(&display, anim_dog_bite, dog_bite_length, dog_bite_framerate, dog_width, dog_height);
Animation dog_run(&display, anim_dog_run, dog_run_length, dog_run_framerate, dog_width, dog_height);








void ApplyGlobalDither() {
  for (int y = 0; y < display.height(); y++) {
    for (int x = 0; x < display.width(); x++) {
      if ((x + y) % 2 != 0) {  // Apaga píxeles impares
        display.drawPixel(x, y, SSD1306_BLACK);
      }
    }
  }
}

void DrawGrilla() {
  // Líneas verticales
  for (int x = 0; x <= SCREEN_WIDTH; x += 20) {
    display.drawLine(x, 0, x, SCREEN_HEIGHT, WHITE);
  }

  // Líneas horizontales
  for (int y = 0; y <= SCREEN_HEIGHT; y += 20) {
    display.drawLine(0, y, SCREEN_WIDTH, y, WHITE);
  }
}

void DrawCircle(int x, int y, int radio, uint8_t brightness) {
  for (int i = -radio; i <= radio; i++) {
    for (int j = -radio; j <= radio; j++) {
      if (i * i + j * j <= radio * radio) {
        // Genera un valor aleatorio entre 0 y 255 para cada píxel
        uint8_t randomValue = random(256);
        // Enciende el píxel solo si el valor aleatorio < brillo deseado
        if (randomValue < brightness) {
          display.drawPixel(x + j, y + i, SSD1306_WHITE);
        }
      }
    }
  }
}

void DrawTextInRect(String _text, int _posX, int _posY, float _textSize) {
  // Calcular dimensiones del rectángulo
  int textWidth = _text.length() * 6 * _textSize;  // Aproximado: 6 píxeles por carácter
  int textHeight = 8 * _textSize;                  // 8 píxeles de altura para tamaño 1
  int padding = 3;                                 // Espacio interno del recuadro

  // Dibujar rectángulo
  display.drawRect(_posX - padding, _posY - padding,
                   textWidth + (padding * 2), textHeight + (padding * 2), WHITE);

  // Dibujar texto
  display.setCursor(_posX, _posY);
  display.setTextSize(_textSize);
  display.print(_text);
}

void DrawCenteredText(const char* text, int posX, int posY) {
  int16_t x1, y1;
  uint16_t w, h;

  // Obtiene las dimensiones del texto
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Ajusta posición para que el centro del texto coincida con posX, posY
  int16_t x = posX - (w / 2);
  int16_t y = posY - (h / 2);

  display.setCursor(x, y);
  display.print(text);
}



#endif