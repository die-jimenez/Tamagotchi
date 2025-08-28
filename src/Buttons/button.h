//#ifndef Evita posibles errores de duplicar clases
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include <functional>  // Necesario para std::function

class Button {
private:
    uint8_t pinButton;
    bool isKeyDown;
    const char* direction;

public:
    Button(uint8_t _pinButton, const char* _direction);
    ~Button();

    void Init();
    void PressEvent(std::function<void()> _action);  
    bool isPressed();
    bool Read();
};

#endif