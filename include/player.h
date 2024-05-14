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

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    int state;
    Texture2D texture;
    Texture2D shadowTexture;
    bool colliding;
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

void init_player(Player *player, Vector2 initPos);
void update_player(Player *player, Ball balls[], int nbrOfBalls);
void move_to_point_player(Player *player, Vector2 point);
void begin_dash_player(Player *player, Vector2 point);
bool lerp_until_point_player(Player *player, Vector2 point);
void dash_player(Player *player);
void begin_slice_player(Player *player);
void slice_player(Player *player);
bool is_inside_screen(Player player);
void screen_collision_player(Player *player);
void ball_collision_player(Player *player, Ball balls[], int nbrOfBalls);
void draw_player(Player player);
void draw_slice_player(Player player);
void draw_player_shadow(Player player);

#endif
