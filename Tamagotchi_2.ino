#include <Arduino.h>
#include <functional>  // Necesario para std::function

//Contiene todas las variables que comparto con "screensBehavior" 
#include "globals.h" 
#include "stateMachine.h"

//Scenes
#include "src/Scenes/credits_scene.h"
CreditsScene credits_scene(&display, &animationManager);
#include "src/Scenes/main_scene.h"
MainScene main_scene(&display, &animationManager);



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
    EggClosed_SCREEN();
  }

  if (state == "egg_opened") {
    EggOpened_SCREEN();
  }

  if (state == "preload_main_menu") {
    Preload_Menu_SCREEN();
  }

  if (state == "main_menu") {
    main_scene.Update(deltaTime.Get());
    DogWalk();
  }


  //ApplyGlobalDither();
  //DrawGrilla();
  timerAnimation.Update(deltaTime.Get());
  animationManager.Update(deltaTime.Get());
  humanTime = humanTime + deltaTime.Get();
  display.display();
  delay(10);
}




//Funciones que dibujan e implementan toda la lógica de cada pantalla

void EggClosed_SCREEN() {
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

void EggOpened_SCREEN() {
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

void Preload_Menu_SCREEN() {
  ChangeState("main_menu");
}




//Funciones para !!!COMPLEMENTAR!!! la maquina de estados -----------------------------------------------------------------------------
void OpenEgg() {
  egg_idle.Stop();
  egg_open.SetLoop(false);
  egg_open.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  animationManager.Play(&egg_open);
  egg_open.SetOnComplete([] {
    ChangeState("egg_opened");
  });
}

void DogWalk() {
  dog_walk.SetLoop(true);
  dog_walk.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 26);
  dog_walk.Play(WHITE, deltaTime.Get());

  dog_run.SetLoop(true);

  //dog_run.SetPosition(30, SCREEN_HEIGHT - 26);
  dog_run.Play(WHITE, deltaTime.Get());
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
