#ifndef BALL_GAME_PLAYER_H
#define BALL_GAME_PLAYER_H

#include "raylib.h"
#include "ball.h"
#include "linked_list.h"
#include "sprite.h"

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    int state;
    void (*stateExecute)(struct Player*);
    Sprite *spriteIdle;
    Sprite *spriteRun;
    Sprite *spriteDie;
    Texture2D *shadowTexture;
    bool colliding;
    ListNode **ballHeadRef;
    Ball *collidingBall; // Pointer to the ball that you want to slice
    Ball collidingBallCopy;
    struct Dash *dash;
} Player;
// typedef renames 'struct Player' to simply 'Player'.

typedef struct Dash
{
    bool bufferedDash;
    Vector2 targetPos;
    Vector2 startPos;
    float speed;
    float distance;
    int startLag;
    int endLag;
    int initReloadTime;
    int reloadTime;
    bool justReloaded;
} Dash;

extern Player *playerRef;
extern bool freezePlayer;

Player *PlayerInit(Vector2 initPos, ListNode **ballHeadRef);
Player *PlayerReset(Player *player, Vector2 initPos, ListNode **ballHeadRef);
void PlayerDie(Player *player);
void PlayerUpdate(Player *player);
void PlayerMoveToPoint(Player *player, Vector2 point);
void PlayerBeginDash(Player *player, Vector2 point);
bool PlayerLerpUntilPoint(Player *player, Vector2 point);
void PlayerBeginSlice(Player *player);
void PlayerCollisionBall(Player *player);

void PlayerStateIdle(Player *player);
void PlayerStateMove(Player *player);
void PlayerStateDash(Player *player);
void PlayerStateSlice(Player *player);
void PlayerStateDead(Player *player);
void PlayerStateRevive(Player *player);

#define STATE_EXEC_PLAYER_IDLE (PlayerStateIdle)
#define STATE_EXEC_PLAYER_MOVE (PlayerStateMove)
#define STATE_EXEC_PLAYER_DASH (PlayerStateDash)
#define STATE_EXEC_PLAYER_SLICE (PlayerStateSlice)
#define STATE_EXEC_PLAYER_DEAD (PlayerStateDead)
#define STATE_EXEC_PLAYER_REVIVE (PlayerStateRevive)

void PlayerDraw(Player player);
void PlayerDrawSlice(Player player);
void PlayerDrawShadow(Player player);

#endif
