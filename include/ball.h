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

void BallInit(Ball *ball, Vector2 initPos, float radius);
void BallSetPosition(Ball *ball, Vector2 pos);
void BallMove(Ball *ball);
void BallCollisionScreen(Ball *ball);
void BallCollisionBall(Ball *ball, Ball balls[], int nbrOfBalls);
void BallDraw(Ball ball);
void BallDrawShadow(Ball ball);

#endif //MY_RAYLIB_GAME_BALL_H
