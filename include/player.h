#ifndef MY_RAYLIB_GAME_PLAYER_H
#define MY_RAYLIB_GAME_PLAYER_H

#include "raylib.h"
#include "ball.h"

enum PlayerStates
{
    PLAYER_IDLE = 0,
    PLAYER_MOVING = 1,
    PLAYER_DASHING = 2,
    PLAYER_SLICING = 3,
    PLAYER_DEAD = 4
};

struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    int state;
    Texture2D texture;
    Texture2D shadowTexture;
    bool colliding;
    struct Ball *collidingBall; // Pointer to the ball that you want to slice
    struct Dash *dash;
};

struct Dash
{
    Vector2 targetPos;
    float speed;
    float distance;
    int startLag;
    int endLag;
    int initReloadTime;
    int reloadTime;
};

void initPlayer(struct Player *player, Vector2 initPos);
void updatePlayer(struct Player *player, struct Ball balls[], int nbrOfBalls);
void moveToPointPlayer(struct Player *player, Vector2 point);
void beginDashPlayer(struct Player *player, Vector2 direction);
void dashPlayer(struct Player *player);
void beginSlicePlayer(struct Player *player);
void slicePlayer(struct Player *player);
bool isInsideScreen(struct Player player);
void screenCollisionPlayer(struct Player *player);
void ballCollisionPlayer(struct Player *player, struct Ball balls[], int nbrOfBalls);
void drawPlayer(struct Player player);
void drawPlayerShadow(struct Player player);

#endif
