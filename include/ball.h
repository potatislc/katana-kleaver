#ifndef BALL_GAME_BALL_H
#define BALL_GAME_BALL_H

#include "raylib.h"
#include "linked_list.h"

#define MIN_BALL_RADIUS (4)
#define BALL_TOO_SMALL_FOR_CLEAN_SPLIT (8)

extern ListNode *ballHead;
extern ListNode *ballSpawnPointHead;
extern float ballSpeed;
extern bool freezeBalls;
extern const double ballSpawnTime;

enum BallType
{
    TYPE_MELON,
    TYPE_ORANGE,
    TYPE_ARMOR
};

typedef struct Ball
{
    int type;
    void (*stateExecute)(struct Ball*);
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    bool colliding;
    Rectangle collisionBox;
    void (*onDestroyFunction)(struct Ball*);
    Texture2D *texture;
    float textureScale;
    Vector2 textureOffset;
    Vector2 shadowOffset;
    float shadowScale;
} Ball;

void BallsUpdate();

Ball *BallInit(Vector2 position, float radius, int type);
void BallUpdate(Ball *ball);
void BallSetPosition(Ball *ball, Vector2 pos);
void BallCollisionScreen(Ball *ball);
void BallCollisionBall(Ball *ball);
void BallSplit(Ball *ball, Vector2 splitDir);

void BallStateMove(Ball *ball);

#define STATE_EXEC_BALL_MOVE (BallStateMove)

void BallDraw(Ball ball);
void BallDrawShadow(Ball ball);

void BallSpawn(Ball *ballToSpawn);
void BallDestroy(Ball *ballT);
void BallDeSpawnAll();

// BallSpawnPoint ---------------------------
typedef struct BallSpawnPoint
{
    Ball *mockBall;
    double initTime;
    double spawnTime;
} BallSpawnPoint;

typedef struct
{
    int spawned;
    int destroyed;
} BallNbrCount;

int NbrOfBallsOnScreen(BallNbrCount ballNbrCount);
void BallNbrCountReset(BallNbrCount *ballNbrCount);

extern BallNbrCount ballNbrCount_All;
extern BallNbrCount ballNbrCount_Small; // This determines when to spawn an orange

int ValueOfBall(float radius);

BallSpawnPoint *BallSpawnPointInit(Ball *mockBall, double spawnTime);
void BallSpawnPointUpdate(BallSpawnPoint *ballSpawnPoint);
void BallSpawnPointDraw(BallSpawnPoint ballSpawnPoint);

#endif //BALL_GAME_BALL_H
