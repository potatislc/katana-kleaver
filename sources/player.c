#include "player.h"
#include <math.h>
#include "global.h"

#define sign(a) ((a > 0) ? 1 : -1)

void initPlayer(struct Player *player, Vector2 initPos)
{
    Texture2D playerTexture = LoadTexture("../assets/samurai.png");
    player->texture = playerTexture;
    player->position = initPos;
    player->speed = 2;
    player->radius = 6;
}

void updatePlayer(struct Player *player, struct Ball balls[], int nbrOfBalls)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        moveToPointPlayer(player, toVirtualCoordsVector2(GetMousePosition()));
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {

    }

    screenCollisionPlayer(player);
    ballCollisionPlayer(player, balls, nbrOfBalls);
}

void moveToPointPlayer(struct Player *player, Vector2 point)
{
    Vector2 dist = {point.x - player->position.x, point.y - player->position.y};
    float distLength = sqrtf(dist.x * dist.x + dist.y * dist.y);
    Vector2 distNormalized = {dist.x /distLength, dist.y / distLength};

    if (distLength <= player->speed)
    {
        // player->position = point;
        return;
    }

    player->velocity = (Vector2){ player->speed * distNormalized.x, player->speed * distNormalized.y };

    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y;
}

void screenCollisionPlayer(struct Player *player)
{
    player->position.x = fmaxf(player->radius, fminf(player->position.x, VIRTUAL_SCREEN_WIDTH - player->radius));
    player->position.y = fmaxf(player->radius, fminf(player->position.y, VIRTUAL_SCREEN_HEIGHT - player->radius));
}

void ballCollisionPlayer(struct Player *player, struct Ball balls[], int nbrOfBalls)
{
    for (int i = 0; i < nbrOfBalls; i++)
    {
        Vector2 ballDistVec = { balls[i].position.x - player->position.x, balls[i].position.y - player->position.y };
        float ballDist = ballDistVec.x * ballDistVec.x + ballDistVec.y * ballDistVec.y;

        if (ballDist <= player->radius * player->radius + balls[i].radius * balls[i].radius)
        {
            player->colliding = true;
            return;
        }
        else player->colliding = false;
    }
}

void drawPlayer(struct Player player)
{
    drawPlayerShadow(player);

    Vector2 textureOffset = { (float)player.texture.width / 2.0f, (float)player.texture.height / 2.0f };

    Rectangle playerRect =
            {
                0,
                0,
                sign(player.velocity.x) * player.texture.width,
                (float)player.texture.height
            };

    DrawTextureRec
    (
        player.texture,
        playerRect,
        (Vector2){ player.position.x - textureOffset.x,player.position.y - textureOffset.y },
        WHITE
    );
}

void drawPlayerShadow(struct Player player)
{
    //Vector2 shadowPos = {player.position, }
    if (player.colliding) DrawCircleV(roundVector2(player.position), (float)player.texture.width / 2, BLACK);
}