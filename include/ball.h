#ifndef MY_RAYLIB_GAME_BALL_H
#define MY_RAYLIB_GAME_BALL_H

#include "raylib.h"

typedef struct Ball
{
    Vector2 position;
    Vector2 speed;
    float radius;
    bool colliding;
    Rectangle collisionBox;
    Texture2D texture;
    float textureScale;
    Vector2 textureOffset;
    Vector2 shadowOffset;
    float shadowRadius;
} Ball;

void init_ball(Ball *ball, Vector2 initPos, float radius);
void set_pos_ball(Ball *ball, Vector2 pos);
void move_ball(Ball *ball);
void screen_collision_ball(Ball *ball);
void ball_collision_ball(Ball *ball, Ball balls[], int nbrOfBalls);
void draw_ball(Ball ball);
void draw_shadow_ball(Ball ball);

#endif //MY_RAYLIB_GAME_BALL_H
