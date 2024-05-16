#ifndef BALL_GAME_GLOBAL_H
#define BALL_GAME_GLOBAL_H

#include "raylib.h"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (1280)
#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)

extern const float screenRatio;
extern const Color shadowColor;
extern bool freezeBalls;

Vector2 to_virtual_coords_vector2(Vector2 coords);
Vector2 round_vector2(Vector2 vector);

#endif //BALL_GAME_GLOBAL_H
