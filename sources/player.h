#ifndef MY_RAYLIB_GAME_PLAYER_H
#define MY_RAYLIB_GAME_PLAYER_H

#include <raylib.h>
#include <ball.h>

struct Player
{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float radius;
    Texture2D texture;
};

void initPlayer(struct Player *player, Vector2 initPos);
void updatePlayer(struct Player *player);
void moveToPointPlayer(struct Player *player, Vector2 point);
void ballCollisionPlayer(struct Player *player, struct Ball balls[]);
void drawPlayer(struct Player player);

#endif
