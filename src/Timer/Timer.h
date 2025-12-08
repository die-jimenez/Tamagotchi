#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>
#include <functional>

enum TimerValueType {
    TIMER_INT,
    TIMER_FLOAT,
    TIMER_BOOL,
    TIMER_STRING
};

class Timer {
public:
    Timer();
    explicit Timer(float timerDuration);
    ~Timer();

    // Update debe llamarse con deltaTime (segundos)
    void Update(float deltaTime);

    // Control básico
    void Start();
    void Start(float newDuration);
    void Stop();
    void Reset();
    void Restart();

    // Setters de targets (autoarrancan el timer)
    void SetTarget(int* variable, int value);
    void SetTarget(int* variable, int value, float timerDuration);

    void SetTarget(float* variable, float value);
    void SetTarget(float* variable, float value, float timerDuration);

    void SetTarget(bool* variable, bool value);
    void SetTarget(bool* variable, bool value, float timerDuration);

    void SetTarget(String* variable, const String& value);
    void SetTarget(String* variable, const String& value, float timerDuration);

    // Eventos
    void SetEvent(std::function<void()> callback);
    void SetEvent(std::function<void()> callback, float timerDuration);

    // Configuración
    void SetDuration(float newDuration);
    void SetAutoReset(bool autoResetEnabled);

    // Consultas
    bool IsRunning() const;
    bool IsFinished() const;
    float GetTimeRemaining() const;
    float GetTimeElapsed() const;
    float GetProgress() const;

    void ClearTarget();

private:
    void AutoStart();

    // estado del timer
    float duration;
    float currentTime;
    bool isRunning;
    bool isFinished;
    bool autoReset;

    // targets
    TimerValueType valueType;
    int* intPtr;
    float* floatPtr;
    bool* boolPtr;
    String* stringPtr;

    int targetIntValue;
    float targetFloatValue;
    bool targetBoolValue;
    String targetStringValue;

    // callback
    std::function<void()> callbackFunction;
    bool hasCallback;
};

#endif
