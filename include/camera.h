#ifndef BALL_GAME_CAMERA_H
#define BALL_GAME_CAMERA_H

#include "raylib.h"

typedef struct
{
    Vector2 targetPoint;
    Vector2 currentPoint;
    float amount;
    float speed;
    double startTime;
    double endTime;
} Shake;

typedef struct
{
    float amountIntensity;
    float speedIntensity;
    double durationIntensity;
} ShakeSettings;

extern Camera2D worldSpaceCamera;

void CameraSetShake(float amount, float speed, float duration);
void CameraShakeUpdate();

#endif //BALL_GAME_CAMERA_H
