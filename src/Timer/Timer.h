//Escrito por Claude

#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>

// EnumeraciÃ³n para tipos de datos que puede manejar el timer
enum TimerValueType {
  TIMER_INT,
  TIMER_FLOAT,
  TIMER_BOOL,
  TIMER_STRING
};

class Timer {
private:
  float duration;     // Duracionn del timer en segundos
  float currentTime;  // Tiempo actual del timer
  bool isRunning;     // Si el timer estÃ¡ activo
  bool isFinished;    // Si el timer terminÃ³
  bool autoReset;     // Si se reinicia automÃ¡ticamente

  // Punteros a las variables que modificarÃ¡
  int* intPtr;
  float* floatPtr;
  bool* boolPtr;
  String* stringPtr;

  // Valores que asignarÃ¡ cuando termine
  int targetIntValue;
  float targetFloatValue;
  bool targetBoolValue;
  String targetStringValue;

  // Tipo de dato que manejarÃ¡
  TimerValueType valueType;

  // Variables para callback opcional
  std::function<void()> callbackFunction;
  bool hasCallback;

  // Método privado para auto-iniciar el timer
  void AutoStart();
#include <functional>

public:
  Timer();
  Timer(float timerDuration);
  ~Timer();

  // MÃ©todos principales
  void Update(float deltaTime);
  void Start();
  void Start(float newDuration);
  void Stop();
  void Reset();
  void Restart();

  // MÃ©todos para configurar variables objetivo INT
  void SetTarget(int* variable, int value);
  void SetTarget(int* variable, int value, float timerDuration);

  // MÃ©todos para configurar variables objetivo FLOAT
  void SetTarget(float* variable, float value);
  void SetTarget(float* variable, float value, float timerDuration);

  // MÃ©todos para configurar variables objetivo BOOL
  void SetTarget(bool* variable, bool value);
  void SetTarget(bool* variable, bool value, float timerDuration);

  // MÃ©todos para configurar variables objetivo STRING
  void SetTarget(String* variable, String value);
  void SetTarget(String* variable, String value, float timerDuration);

  // MÃ©todos para callback
  void SetEvent(std::function<void()> callback);
  void SetEvent(std::function<void()> callback, float timerDuration);

  // ConfiguraciÃ³n
  void SetDuration(float newDuration);
  void SetAutoReset(bool autoResetEnabled);

  // Consultas de estado
  bool IsRunning();
  bool IsFinished();
  float GetTimeRemaining();
  float GetTimeElapsed();
  float GetProgress();  // Retorna 0.0 a 1.0

  // MÃ©todo para limpiar configuraciÃ³n
  void ClearTarget();
};

#endif