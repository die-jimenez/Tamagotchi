#ifndef SCREENSBEHAVIOR_H
#define SCREENSBEHAVIOR_H

void Credits_SCREEN() {
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
      ChangeState("egg_closed");
    });
  });
}

#endif