#include "esp32-hal-gpio.h"
#include "button.h"

Button::Button(uint8_t _pinButton, const char* _direction) {
  pinButton = _pinButton;
  direction = _direction;
  isKeyDown = false;
}

Button::~Button() {
}

void Button::Init() {
  pinMode(pinButton, INPUT_PULLUP);
}

void Button::Press(std::function<void()> _action) {
  Serial.printf("El boton %s direction");
  Serial.println(" fue presionado");
  _action();
}

void Button::Update(std::function<void()> _action) {
  if (isPressed()) {
    _action();
  } 
}

bool Button::Read() {
  return digitalRead(pinButton);
}

bool Button::isPressed() {
  // Al presionar es TRUE, al soltar es FALSE
  if (Read() == LOW && isKeyDown == false) {
    isKeyDown = true;
    return true;
  } else if (Read() == HIGH && isKeyDown == true) {
    isKeyDown = false;
    return false;
  } else {
    return false;
  }
}