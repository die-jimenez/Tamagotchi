#include <Arduino.h>
#include <functional>  // Necesario para std::function

String state = "credits"
  //wainting
  //creepy_eye
  //egg_close
  //egg_open
  //main_menu
  ;
String lastState;

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

#include "src/AnimationManager/animationManager.h"
#include "src/AnimationManager/animation.h"
#include "src/Animations/egg.h"
#include "src/Animations/eye.h"
AnimationManager animationManager(&display);
Animation egg_idle(&display, anim_egg_idle, egg_idle_length, egg_idle_framerate, egg_width, egg_height);
Animation egg_open(&display, anim_egg_open, egg_open_length, egg_open_framerate, egg_width, egg_height);
Animation eye(&display, anim_eye, eye_length, eye_framerate, eye_width, eye_height);

#include "src/Timer/Timer.h"
Timer timerAnimation;



void setup() {
  Serial.begin(115200);
  buttonL.Init();
  buttonC.Init();
  buttonR.Init();

  //Esta funcion tarda en completarse, hay que darle un delay
  OLEDInit();
  delay(1000);
  Serial.println("Configuración terminada");


  //---> Aquí ya se pueden sumar las animaciones iniciales
  //animationManager.Play(&egg_idle);
  //animationManager.Remove(&egg_idle);
  //animationManager.PlayOneShot(&eye);
}



void loop() {
  deltaTime.Run();
  display.clearDisplay();

  if (state == "waiting") {
  }

  if (state == "credits") {
    DrawTextInRect("Creado por: ", 0, 0, 1);
    display.setCursor(0, 15);
    display.print("Diego Jimenez");
    display.display();
    delay(1000);

    //Pantalla en negro
    display.clearDisplay();
    delay(1000);

    //Animación del ojo
    ChangeState("waiting");
    eye.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    animationManager.PlayOneShot(&eye);
    eye.SetOnComplete([] {
      timerAnimation.SetDuration(1);
      timerAnimation.SetEvent([] {
        ChangeState("egg_close");
      });
    });
  }

  if (state == "egg_close") {
    egg_idle.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    egg_idle.Play(WHITE, deltaTime.Get());
    buttonL.PressEvent([]() {
      OpenEgg();
    });
    buttonC.PressEvent([]() {
      OpenEgg();
    });
    buttonR.PressEvent([]() {
      OpenEgg();
    });
  }

  if (state == "egg_open") {
    DrawCenteredText("PIPO", SCREEN_WIDTH / 2, 20);
    buttonL.PressEvent([]() {
      GoToMainMenu();
    });
    buttonC.PressEvent([]() {
      GoToMainMenu();
    });
    buttonR.PressEvent([]() {
      GoToMainMenu();
    });
  }





  //ApplyGlobalDither();
  //DrawGrilla();
  timerAnimation.Update(deltaTime.Get());
  animationManager.Update(deltaTime.Get());
  humanTime = humanTime + deltaTime.Get();
  display.display();
  delay(10);
}



//FUNCIONES DE MAQUINA DE ESTADOS -----------------------------------------------------------------------------
void ChangeState(String _newState) {
  lastState = state;
  state = _newState;
}

void OpenEgg() {
  egg_idle.Stop();
  egg_open.SetLoop(false);
  egg_open.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  animationManager.Play(&egg_open);
  egg_open.SetOnComplete([] {
    ChangeState("egg_open");
  });
}

void GoToMainMenu() {
  animationManager.StopAll();
  ChangeState("main_menu");
}



//--------------------------------------------------------------------------------------------------------------





void OLEDInit() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
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
