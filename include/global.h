#ifndef MY_RAYLIB_GAME_GLOBAL_H
#define MY_RAYLIB_GAME_GLOBAL_H

#include "raylib.h"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (1280)
#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)

extern const float screenRatio;
extern const Color shadowColor;
extern bool freezeBalls;

Vector2 toVirtualCoordsVector2(Vector2 coords);
Vector2 roundVector2(Vector2 vector);

#endif //MY_RAYLIB_GAME_GLOBAL_H
