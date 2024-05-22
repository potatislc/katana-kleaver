#ifndef BALL_GAME_PLAYER_H
#define BALL_GAME_PLAYER_H

#include "raylib.h"
#include "ball.h"
#include "linked_list.h"

enum PlayerStates
{
    PLAYER_IDLE = 0,
    PLAYER_MOVING = 1,
    PLAYER_DASHING = 2,
    PLAYER_SLICING = 3,
    PLAYER_DEAD = 4
};

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    int state;
    Texture2D *texture;
    Texture2D *shadowTexture;
    bool colliding;
    ListNode **ballHeadRef;
    Ball *collidingBall; // Pointer to the ball that you want to slice
    Ball *collidingBallCopy;
    struct Dash *dash;
} Player;
// typedef renames 'struct Player' to simply 'Player'.

typedef struct Dash
{
    Vector2 targetPos;
    Vector2 startPos;
    float speed;
    float distance;
    int startLag;
    int endLag;
    int initReloadTime;
    int reloadTime;
} Dash;

void PlayerInit(Player *player, Vector2 initPos, ListNode **ballHeadRef);
void PlayerUpdate(Player *player);
void PlayerMove(Player *player);
void PlayerMoveToPoint(Player *player, Vector2 point);
void PlayerBeginDash(Player *player, Vector2 point);
bool PlayerLerpUntilPoint(Player *player, Vector2 point);
void PlayerDash(Player *player);
void PlayerBeginSlice(Player *player);
void PlayerSlice(Player *player);
bool IsInsideScreen(Player player);
void PlayerCollisionScreen(Player *player);
void PlayerCollisionBall(Player *player);

void PlayerDraw(Player player);
void PlayerDrawSlice(Player player);
void PlayerDrawShadow(Player player);

#endif
