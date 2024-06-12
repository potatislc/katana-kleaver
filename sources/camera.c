#include "camera.h"
#include "raymath.h"

Camera2D worldSpaceCamera = { 0 };

typedef struct
{
    float amount;
    float speed;
    double startTime;
    double endTime;
} Shake;

Shake shake = {100.f, 0.f, 0, 10};

void CameraSetShake(float amount, float speed, float duration)
{
    shake.amount = amount;
    shake.speed = speed;
    shake.startTime = GetTime();
    shake.endTime = GetTime() + duration;
}

void CameraShakeUpdate()
{
    if (shake.endTime <= GetTime())
    {
        worldSpaceCamera.offset = Vector2Zero();
        return;
    }

    float shakeProgress = 1.f - (float)((GetTime() - shake.startTime) / (shake.endTime - shake.startTime));

    float effectiveAmount = shake.amount * shakeProgress;
    float effectiveSpeed = shake.speed * shakeProgress;

    worldSpaceCamera.offset = (Vector2){effectiveAmount, effectiveAmount};
}