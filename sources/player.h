#ifndef MY_RAYLIB_GAME_PLAYER_H
#define MY_RAYLIB_GAME_PLAYER_H

#include <raylib.h>
#include <ball.h>

struct Player
{
    Vector2 position;
    float speed;
    float radius;
};

void moveToPointPlayer(struct Player *player, float x, float y);
void ballCollisionPlayer(struct Player *player, struct Ball balls[]);
void drawPlayer(struct Player player);

#endif
