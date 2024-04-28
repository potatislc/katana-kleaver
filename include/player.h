#ifndef MY_RAYLIB_GAME_PLAYER_H
#define MY_RAYLIB_GAME_PLAYER_H

#include "raylib.h"
#include "ball.h"

struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    Texture2D texture;
    bool colliding;
};

void initPlayer(struct Player *player, Vector2 initPos);
void updatePlayer(struct Player *player, struct Ball balls[], int nbrOfBalls);
void moveToPointPlayer(struct Player *player, Vector2 point);
void screenCollisionPlayer(struct Player *player);
void ballCollisionPlayer(struct Player *player, struct Ball balls[], int nbrOfBalls);
void drawPlayer(struct Player player);
void drawPlayerShadow(struct Player player);

#endif
