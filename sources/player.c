#include <player.h>
#include <math.h>

void initPlayer(struct Player *player, Vector2 initPos)
{
    Texture2D playerTexture = LoadTexture("../assets/samurai.png");
    player->texture = playerTexture;
    player->position = initPos;
    player->speed = 2;
}

void updatePlayer(struct Player *player)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        moveToPointPlayer(player, GetMousePosition());
}

void moveToPointPlayer(struct Player *player, Vector2 point)
{
    Vector2 dist = {point.x - player->position.x, point.y - player->position.y};
    float distLength = sqrtf(dist.x * dist.x + dist.y * dist.y);
    Vector2 distNormalized = {dist.x /distLength, dist.y / distLength};

    player->velocity = (Vector2){ player->speed * distNormalized.x, player->speed * distNormalized.y };

    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y;
}

void drawPlayer(struct Player player)
{
    DrawTextureV(player.texture, player.position, WHITE);
}