#ifndef MY_RAYLIB_GAME_PLAYER_H
#define MY_RAYLIB_GAME_PLAYER_H

#include "raylib.h"
#include "ball.h"

enum PlayerStates
{
    PLAYER_IDLE = 0,
    PLAYER_MOVING = 1,
    PLAYER_DASHING = 2,
    PLAYER_DEAD = 3
};

struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    int state;
    Texture2D texture;
    bool colliding;
    struct Dash *dash;
};

struct Dash
{
    Vector2 targetPos;
    float speed;
    float distance;
    float startLag;
    float endLag;
    float reloadTime;
};

void initPlayer(struct Player *player, Vector2 initPos);
void updatePlayer(struct Player *player, struct Ball balls[], int nbrOfBalls);
void moveToPointPlayer(struct Player *player, Vector2 point);
void beginDashPlayer(struct Player *player, Vector2 direction);
void dashPlayer(struct Player *player);
void screenCollisionPlayer(struct Player *player);
void ballCollisionPlayer(struct Player *player, struct Ball balls[], int nbrOfBalls);
void drawPlayer(struct Player player);
void drawPlayerShadow(struct Player player);

#endif
