#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)

#include "src/Buttons/button.h"
// Declaración de botones (solo referencia)
extern Button buttonL;
extern Button buttonC;
extern Button buttonR;



void DrawGrilla(Adafruit_SSD1306* _display) {
  // Líneas verticales
  for (int x = 0; x <= SCREEN_WIDTH; x += 20) {
    _display->drawLine(x, 0, x, SCREEN_HEIGHT, WHITE);
  }

  // Líneas horizontales
  for (int y = 0; y <= SCREEN_HEIGHT; y += 20) {
    _display->drawLine(0, y, SCREEN_WIDTH, y, WHITE);
  }
}

void DrawCircle(int x, int y, int radio, uint8_t brightness, Adafruit_SSD1306* _display) {
  for (int i = -radio; i <= radio; i++) {
    for (int j = -radio; j <= radio; j++) {
      if (i * i + j * j <= radio * radio) {
        // Genera un valor aleatorio entre 0 y 255 para cada píxel
        uint8_t randomValue = random(256);
        // Enciende el píxel solo si el valor aleatorio < brillo deseado
        if (randomValue < brightness) {
          _display->drawPixel(x + j, y + i, SSD1306_WHITE);
        }
      }
    }
  }
}

void DrawTextInRect(String _text, int _posX, int _posY, float _textSize, Adafruit_SSD1306* _display) {
  // Calcular dimensiones del rectángulo
  int textWidth = _text.length() * 6 * _textSize;  // Aproximado: 6 píxeles por carácter
  int textHeight = 8 * _textSize;                  // 8 píxeles de altura para tamaño 1
  int padding = 3;                                 // Espacio interno del recuadro

  // Dibujar rectángulo
  _display->drawRect(_posX - padding, _posY - padding,
                     textWidth + (padding * 2), textHeight + (padding * 2), WHITE);

  // Dibujar texto
  _display->setCursor(_posX, _posY);
  _display->setTextSize(_textSize);
  _display->print(_text);
}

void DrawCenteredText(const char* text, int posX, int posY, Adafruit_SSD1306* _display) {
  int16_t x1, y1;
  uint16_t w, h;

  // Obtiene las dimensiones del texto
  _display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Ajusta posición para que el centro del texto coincida con posX, posY
  int16_t x = posX - (w / 2);
  int16_t y = posY - (h / 2);

  _display->setCursor(x, y);
  _display->print(text);
}

void ApplyGlobalDither(Adafruit_SSD1306* _display) {
  for (int y = 0; y < _display->height(); y++) {
    for (int x = 0; x < _display->width(); x++) {
      if ((x + y) % 2 != 0) {  // Apaga píxeles impares
        _display->drawPixel(x, y, SSD1306_BLACK);
      }
    }
  }
}

void ApplyDitherToRect(int posX, int posY, int width, int height, Adafruit_SSD1306* _display, bool showRect = false) {
  if (showRect) {
    _display->drawRect(posX, posY, width, height, WHITE);
  }
  for (int y = posY; y < posY + height; y++) {
    for (int x = posX; x < posX + width; x++) {
      if ((x + y) % 2 != 0) {  // Apaga píxeles impares
        _display->drawPixel(x, y, SSD1306_BLACK);
      }
    }
  }
}



#endif