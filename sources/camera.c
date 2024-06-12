#include <stdlib.h>
#include "camera.h"
#include "raymath.h"

Camera2D worldSpaceCamera = { 0, 0 };

typedef struct
{
    Vector2 target;
    float amount;
    float speed;
    double startTime;
    double endTime;
} Shake;

Shake shake = {(Vector2){0.f, 0.f}, 10.f, .5, 0, 10};

void CameraSetShake(float amount, float speed, float duration)
{
    shake.amount = amount;
    shake.speed = speed;
    shake.startTime = GetTime();
    shake.endTime = GetTime() + duration;

    shake.target = Vector2Zero();
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

    //worldSpaceCamera.offset = Vector2MoveTowards(worldSpaceCamera.offset, shake.target, shake.speed * 6);

    worldSpaceCamera.offset = Vector2Lerp(worldSpaceCamera.offset, shake.target, effectiveSpeed);

    if (Vector2Distance(worldSpaceCamera.offset, shake.target) <= 1.f)
    {
        worldSpaceCamera.offset = shake.target;

        float randAngle = (rand() / (float)RAND_MAX) * (PI * 2);
        shake.target = (Vector2){cosf(randAngle) * effectiveAmount, sinf(randAngle) * effectiveAmount};
    }
}