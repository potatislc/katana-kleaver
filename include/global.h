#ifndef MY_RAYLIB_GAME_GLOBAL_H
#define MY_RAYLIB_GAME_GLOBAL_H

#include "raylib.h"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (1280)
#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)
#define VIRTUAL_SCREEN_WIDTH_CENTER (128)
#define VIRTUAL_SCREEN_HEIGHT_CENTER (128)

extern const float screenRatio;

Vector2 ToVirtualCoords(Vector2 coords);

#endif //MY_RAYLIB_GAME_GLOBAL_H
