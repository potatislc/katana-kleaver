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
    Vector2 startPos;
    float speed;
    float distance;
    int startLag;
    int endLag;
    int initReloadTime;
    int reloadTime;
};

void init_player(struct Player *player, Vector2 initPos);
void update_player(struct Player *player, struct Ball balls[], int nbrOfBalls);
void move_to_point_player(struct Player *player, Vector2 point);
void begin_dash_player(struct Player *player, Vector2 point);
bool lerp_until_point_player(struct Player *player, Vector2 point);
void dash_player(struct Player *player);
void begin_slice_player(struct Player *player);
void slice_player(struct Player *player);
bool is_inside_screen(struct Player player);
void screen_collision_player(struct Player *player);
void ball_collision_player(struct Player *player, struct Ball balls[], int nbrOfBalls);
void draw_player(struct Player player);
void draw_slice_player(struct Player player);
void draw_player_shadow(struct Player player);

#endif
