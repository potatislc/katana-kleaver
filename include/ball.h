#ifndef MY_RAYLIB_GAME_BALL_H
#define MY_RAYLIB_GAME_BALL_H

#include "raylib.h"

struct Ball
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
};

void init_ball(struct Ball *ball, Vector2 initPos, float radius);
void set_pos_ball(struct Ball *ball, Vector2 pos);
void move_ball(struct Ball *ball);
void screen_collision_ball(struct Ball *ball);
void ball_collision_ball(struct Ball *ball, struct Ball balls[], int nbrOfBalls);
void draw_ball(struct Ball ball);
void draw_shadow_ball(struct Ball ball);

#endif //MY_RAYLIB_GAME_BALL_H
