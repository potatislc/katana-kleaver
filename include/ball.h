#ifndef BALL_GAME_BALL_H
#define BALL_GAME_BALL_H

#include "raylib.h"
#include "linked_list.h"

extern ListNode *ballHead;
extern ListNode *ballSpawnPointHead;
extern float ballSpeed;
extern bool freezeBalls;
extern const double ballSpawnTime;

typedef struct Ball
{
    void (*stateExecute)(struct Ball*);
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
    float shadowScale;
} Ball;

Ball *BallInit(Vector2 minInitPos,Vector2 maxInitPos, float radius);
void BallUpdate(Ball *ball);
void BallSetPosition(Ball *ball, Vector2 pos);
void BallCollisionScreen(Ball *ball);
void BallCollisionBall(Ball *ball);
void BallSplit(Ball *ball, Vector2 splitDir);

void BallStateMove(Ball *ball);

#define STATE_EXEC_BALL_MOVE (BallStateMove)

void BallDraw(Ball ball);
void BallDrawShadow(Ball ball);

// BallSpawnPoint ---------------------------
typedef struct BallSpawnPoint
{
    Ball *mockBall;
    double initTime;
    double spawnTime;
} BallSpawnPoint;

BallSpawnPoint *BallSpawnPointInit(Ball *mockBall, double spawnTime);
void BallSpawnPointUpdate(BallSpawnPoint *ballSpawnPoint);
void BallSpawnPointDraw(BallSpawnPoint ballSpawnPoint);

#endif //BALL_GAME_BALL_H
