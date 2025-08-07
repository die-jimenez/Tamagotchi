#include <Arduino.h>
#include <functional>  // Necesario para std::function


//Libreria de dibujo: Codigo extraido de: https://github.com/FluxGarage/RoboEyes
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "src/DeltaTime/DeltaTime.h"
DeltaTime deltaTime;
float humanTime;

#include "src/Buttons/button.h"
Button buttonL(7, "LEFT");    //0
Button buttonC(6, "CENTER");  //1
Button buttonR(5, "RIGHT");   //3 y... 4


#include "src/Animations/Egg.h"
#include "animacion.h"
Animacion egg_open(anim_egg_open, egg_framerate, egg_width, egg_height);

// //Colores
// int WHITE = 1;
// int BLACK = 0;




void setup() {
  Serial.begin(115200);
  buttonL.Init();
  buttonC.Init();
  buttonR.Init();

  OLEDInit();

  Serial.println("setup terminado");
  Serial.println(egg_framerate);
}

void loop() {
  deltaTime.Run();
  buttonL.Update([]() {
    Serial.println("L");
    humanTime = 0;
  });
  buttonC.Update([]() {
    Serial.println("C");
  });
  buttonR.Update([]() {
    Serial.println("R");
  });


  display.drawBitmap(0, 0, egg_open.GetSprite(0), egg_open.GetWidth(), egg_open.GetHeight(), WHITE);
  display.fillRect(100, 10, 20, 15, SSD1306_WHITE);
  display.fillCircle(70, 40, 10, SSD1306_WHITE);


  //ApplyGlobalDither();
  humanTime = humanTime + deltaTime.Get();
  display.display();
  delay(10);
}



void OLEDInit() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
}


void ApplyGlobalDither() {
  for (int y = 0; y < display.height(); y++) {
    for (int x = 0; x < display.width(); x++) {
      if ((x + y) % 2 != 0) {  // Apaga píxeles impares
        display.drawPixel(x, y, SSD1306_BLACK);
      }
    }
  }
}








void drawGreyCircle(int x, int y, int radio, uint8_t brightness) {
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







void funcionesDibujado() {
  // 1. FUNCIONES BÁSICAS
  display.clearDisplay();  // Borra todo el contenido de la pantalla
  display.display();       // Envía los cambios a la pantalla (¡siempre necesario!)

  // 2. FUNCIONES DE TEXTO
  display.setTextSize(1);               // Tamaño del texto (1 = 6x8 píxeles)
  display.setTextColor(SSD1306_WHITE);  // Color del texto (BLACK para fondo blanco)
  display.setCursor(0, 0);              // Posición inicial (x, y)
  display.print("Hola OLED!");          // Escribe texto (println para salto de línea)

  // 3. FUNCIONES DE DIBUJO (FIGURAS)
  display.drawPixel(10, 10, SSD1306_WHITE);                       // Píxel en (x, y)
  display.drawLine(0, 15, 50, 15, SSD1306_WHITE);                 // Línea de (x0,y0) a (x1,y1)
  display.drawRect(60, 10, 30, 20, SSD1306_WHITE);                // Rectángulo (x,y,ancho,alto)
  display.fillRect(100, 10, 20, 15, SSD1306_WHITE);               // Rectángulo relleno
  display.drawCircle(30, 40, 10, SSD1306_WHITE);                  // Círculo (x,y,radio)
  display.fillCircle(70, 40, 10, SSD1306_WHITE);                  // Círculo relleno
  display.drawTriangle(90, 35, 110, 35, 100, 50, SSD1306_WHITE);  // Triángulo (3 vértices)

  // 4. FUNCIONES AVANZADAS
  display.invertDisplay(true);           // Invierte colores (true = fondo blanco)
  display.startscrollright(0x00, 0x0F);  // Texto en movimiento (animación)
  delay(2000);
  display.stopscroll();  // Detiene animación
}
