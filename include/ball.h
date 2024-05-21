#ifndef BALL_GAME_BALL_H
#define BALL_GAME_BALL_H

#include "raylib.h"
#include "linked_list.h"

extern float ballSpeed;

typedef struct Ball
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    bool colliding;
    Rectangle collisionBox;
    Texture2D *texture;
    float textureScale;
    Vector2 textureOffset;
    Vector2 shadowOffset;
    float shadowRadius;
} Ball;

void BallInit(Ball *ball, Vector2 initPos, float radius);
void BallSetPosition(Ball *ball, Vector2 pos);
void BallMove(Ball *ball);
void BallCollisionScreen(Ball *ball);
void BallCollisionBall(Ball *ball, ListNode *ballHead);
void BallSplit(Ball *ball, ListNode **ballHeadRef, Vector2 splitDir);

void BallDraw(Ball ball);
void BallDrawShadow(Ball ball);

#endif //BALL_GAME_BALL_H
