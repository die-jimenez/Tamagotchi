//Escrito por Claude

#include "Timer.h"

Timer::Timer() {
    duration = 1.0f;
    currentTime = 0.0f;
    isRunning = false;
    isFinished = false;
    autoReset = false;
    
    // Inicializar punteros como nullptr
    intPtr = nullptr;
    floatPtr = nullptr;
    boolPtr = nullptr;
    stringPtr = nullptr;
    
    // Inicializar valores por defecto
    targetIntValue = 0;
    targetFloatValue = 0.0f;
    targetBoolValue = false;
    targetStringValue = "";
    
    valueType = TIMER_INT;
    callbackFunction = nullptr;
    hasCallback = false;
}

Timer::Timer(float timerDuration) {
    duration = timerDuration;
    currentTime = 0.0f;
    isRunning = false;
    isFinished = false;
    autoReset = false;
    
    intPtr = nullptr;
    floatPtr = nullptr;
    boolPtr = nullptr;
    stringPtr = nullptr;
    
    targetIntValue = 0;
    targetFloatValue = 0.0f;
    targetBoolValue = false;
    targetStringValue = "";
    
    valueType = TIMER_INT;
    callbackFunction = nullptr;
    hasCallback = false;
}

Timer::~Timer() {
    // No necesitamos liberar memoria ya que no manejamos los punteros
}

void Timer::Update(float deltaTime) {
    if (!isRunning) return;
    
    currentTime += deltaTime;
    
    if (currentTime >= duration) {
        // El timer terminÃ³
        isFinished = true;
        
        // Ejecutar la acciÃ³n segÃºn el tipo de dato
        switch (valueType) {
            case TIMER_INT:
                if (intPtr != nullptr) {
                    *intPtr = targetIntValue;
                }
                break;
                
            case TIMER_FLOAT:
                if (floatPtr != nullptr) {
                    *floatPtr = targetFloatValue;
                }
                break;
                
            case TIMER_BOOL:
                if (boolPtr != nullptr) {
                    *boolPtr = targetBoolValue;
                }
                break;
                
            case TIMER_STRING:
                if (stringPtr != nullptr) {
                    *stringPtr = targetStringValue;
                }
                break;
        }
        
        // Ejecutar callback si existe
        if (hasCallback && callbackFunction != nullptr) {
            callbackFunction();
        }
        
        if (autoReset) {
            currentTime = 0.0f;
            isFinished = false;
        } else {
            isRunning = false;
        }
    }
}

void Timer::Start() {
    currentTime = 0.0f;
    isRunning = true;
    isFinished = false;
}

void Timer::Start(float newDuration) {
    duration = newDuration;
    Start();
}

void Timer::Stop() {
    isRunning = false;
}

void Timer::Reset() {
    currentTime = 0.0f;
    isFinished = false;
}

void Timer::Restart() {
    Reset();
    Start();
}

// Método privado para auto-iniciar
void Timer::AutoStart() {
    currentTime = 0.0f;
    isRunning = true;
    isFinished = false;
}

// MÃ©todos para INT
void Timer::SetTarget(int* variable, int value) {
    intPtr = variable;
    targetIntValue = value;
    valueType = TIMER_INT;
    AutoStart();
}

void Timer::SetTarget(int* variable, int value, float timerDuration) {
    duration = timerDuration;
    SetTarget(variable, value);
}

// MÃ©todos para FLOAT
void Timer::SetTarget(float* variable, float value) {
    floatPtr = variable;
    targetFloatValue = value;
    valueType = TIMER_FLOAT;
    AutoStart();
}

void Timer::SetTarget(float* variable, float value, float timerDuration) {
    duration = timerDuration;
    SetTarget(variable, value);
}

// MÃ©todos para BOOL
void Timer::SetTarget(bool* variable, bool value) {
    boolPtr = variable;
    targetBoolValue = value;
    valueType = TIMER_BOOL;
    AutoStart();
}

void Timer::SetTarget(bool* variable, bool value, float timerDuration) {
    duration = timerDuration;
    SetTarget(variable, value);
}

// MÃ©todos para STRING
void Timer::SetTarget(String* variable, String value) {
    stringPtr = variable;
    targetStringValue = value;
    valueType = TIMER_STRING;
    AutoStart();
}

void Timer::SetTarget(String* variable, String value, float timerDuration) {
    duration = timerDuration;
    SetTarget(variable, value);
}

// Sin duración extra
void Timer::SetEvent(std::function<void()> callback) {
    callbackFunction = callback;
    hasCallback = true;
    AutoStart();
}

// Con duración
void Timer::SetEvent(std::function<void()> callback, float timerDuration) {
    duration = timerDuration;
    SetEvent(callback);
}

void Timer::SetDuration(float newDuration) {
    duration = newDuration;
}

void Timer::SetAutoReset(bool autoResetEnabled) {
    autoReset = autoResetEnabled;
}

bool Timer::IsRunning() {
    return isRunning;
}

bool Timer::IsFinished() {
    return isFinished;
}

float Timer::GetTimeRemaining() {
    if (!isRunning) return 0.0f;
    return duration - currentTime;
}

float Timer::GetTimeElapsed() {
    return currentTime;
}

float Timer::GetProgress() {
    if (duration <= 0) return 1.0f;
    return currentTime / duration;
}

void Timer::ClearTarget() {
    intPtr = nullptr;
    floatPtr = nullptr;
    boolPtr = nullptr;
    stringPtr = nullptr;
    callbackFunction = nullptr;
    hasCallback = false;
    Stop(); // Detener el timer cuando se limpian los targets
}