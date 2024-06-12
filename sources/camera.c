#include <stdlib.h>
#include "camera.h"
#include "raymath.h"
#include "global.h"

Camera2D worldSpaceCamera = { 0, 0 };

Shake shake = {(Vector2){0.f, 0.f}, (Vector2){0.f, 0.f}, 0.f, 0.f, 0, 0};

ShakeSettings shakeSettings = {1.f, 1.f, 1};

void CameraSetShake(float amount, float speed, float duration)
{
    shake.amount = amount * shakeSettings.amountIntensity;
    shake.speed = speed  * shakeSettings.speedIntensity;
    shake.startTime = GetTime();
    shake.endTime = GetTime() + duration  * shakeSettings.durationIntensity;

    shake.targetPoint = Vector2Zero();
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

    shake.currentPoint = Vector2MoveTowards(worldSpaceCamera.offset, shake.targetPoint, effectiveSpeed * 6);
    //shake.currentPoint = Vector2Lerp(worldSpaceCamera.offset, shake.targetPoint, effectiveSpeed);

    worldSpaceCamera.offset = Vector2Round(shake.currentPoint);

    if (Vector2Distance(worldSpaceCamera.offset, shake.targetPoint) <= 1.f)
    {
        worldSpaceCamera.offset = shake.targetPoint;

        float randAngle = (rand() / (float)RAND_MAX) * (PI * 2);
        shake.targetPoint = (Vector2){cosf(randAngle) * effectiveAmount, sinf(randAngle) * effectiveAmount};
    }
}