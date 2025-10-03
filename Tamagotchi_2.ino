#include <Arduino.h>
#include <functional>  // Necesario para std::function
#include "globals.h" 
#include "stateMachine.h"

//Lista de todos los estados
String state = "credits"
  //wainting
  //credits
  //egg_closed
  //preload_main
  //main
  ; 
String lastState;

//Libreria de dibujo: Codigo extraido de: https://github.com/FluxGarage/RoboEyes
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "src/AnimationManager/animationManager.h"
AnimationManager animationManager(&display);

#include "src/Buttons/button.h"
Button buttonL(7, "LEFT");    //0
Button buttonC(6, "CENTER");  //1
Button buttonR(5, "RIGHT");   //3 y... 4


#include "src/DeltaTime/DeltaTime.h"
DeltaTime deltaTime;
float humanTime;

//Scenes
#include "src/Scenes/credits_scene.h"
CreditsScene credits_scene(&display, &animationManager);
#include "src/Scenes/egg_scene.h"
EggScene egg_scene(&display, &animationManager);
#include "src/Scenes/main_scene.h"
MainScene main_scene(&display, &animationManager);
#include "src/Scenes/game_scene.h"
GameScene game_scene(&display, &animationManager);



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
  state = "credits";
}



void loop() {
  deltaTime.Run();
  display.clearDisplay();

  if (state == "waiting") {
  }

  if (state == "credits") {
    credits_scene.Update(deltaTime.Get());
  }

  if (state == "egg_closed") {
    egg_scene.Update(deltaTime.Get());
  }

  if (state == "preload_main") {
    main_scene.PreloadMain();
  }

  if (state == "main") {
    main_scene.Update(deltaTime.Get());
  }

  if (state == "food") {
    //main_scene.Update(deltaTime.Get());
  }

  if (state == "game") {
    game_scene.Update(deltaTime.Get());
  }

  if (state == "dialog") {
    //main_scene.Update(deltaTime.Get());
  }

  //ApplyGlobalDither();
  //DrawGrilla();
  //timerAnimation.Update(deltaTime.Get());
  animationManager.Update(deltaTime.Get());
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
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
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
