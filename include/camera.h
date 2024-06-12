#ifndef BALL_GAME_CAMERA_H
#define BALL_GAME_CAMERA_H

#include "raylib.h"

extern Camera2D worldSpaceCamera;

void CameraSetShake(float amount, float speed, float duration);
void CameraShakeUpdate();

#endif //BALL_GAME_CAMERA_H
