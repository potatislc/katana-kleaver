#include "player.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>
#include "global.h"

#define sign(a) ((a > 0) ? 1 : -1)

void initPlayer(struct Player *player, Vector2 initPos)
{
    player->state = PLAYER_MOVING;
    player->texture = LoadTexture("../assets/samurai.png");
    player->shadowTexture = LoadTexture("../assets/samurai_shadow.png");
    player->position = initPos;
    player->speed = 2;
    player->radius = 6;

    player->dash = (struct Dash *)malloc(sizeof(struct Dash));
    if (player->dash != NULL) {
        // Initialize the allocated memory
        *player->dash = (struct Dash){0.0f, 0.0f, 0.3f, 32.0f, 10, 30, 30, 0};
    } else {
        // Handle failure to allocate memory
    }

    // Allocate memory for whatever ball that goes here
    player->collidingBall = (struct Ball *)malloc(sizeof(struct Ball));
}

void updatePlayer(struct Player *player, struct Ball balls[], int nbrOfBalls)
{
    switch(player->state)
    {
        case PLAYER_IDLE:
            break;
        case PLAYER_MOVING:
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                moveToPointPlayer(player, toVirtualCoordsVector2(GetMousePosition()));
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player->dash->reloadTime == 0)
            {
                beginDashPlayer(player, toVirtualCoordsVector2(GetMousePosition()));
            }
            break;
        case PLAYER_DASHING:
            dashPlayer(player);
            break;
    }

    screenCollisionPlayer(player);
    ballCollisionPlayer(player, balls, nbrOfBalls);
    if (player->dash->reloadTime > 0) player->dash->reloadTime--;
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

void beginDashPlayer(struct Player *player, Vector2 point)
{
    player->state = PLAYER_DASHING;
    player->dash->reloadTime = player->dash->initReloadTime;

    Vector2 direction = Vector2Normalize((Vector2){ player->position.x - point.x, player->position.y - point.y});
    player->dash->targetPos = direction;
    player->dash->targetPos = (Vector2){
            player->position.x - player->dash->targetPos.x * player->dash->distance,
            player->position.y - player->dash->targetPos.y * player->dash->distance };

    player->velocity.x = -(float)sign(direction.x);
}

void dashPlayer(struct Player *player)
{
    player->position = (Vector2){
        Lerp(player->position.x, player->dash->targetPos.x, player->dash->speed),
        Lerp(player->position.y, player->dash->targetPos.y, player->dash->speed) };

    if (player->colliding)
    {
        beginSlicePlayer(player);
    }

    if (Vector2Distance(player->position, player->dash->targetPos) <= 2 || !isInsideScreen(*player))
    {
        player->state = PLAYER_MOVING;
    }
}

void beginSlicePlayer(struct Player *player)
{
    player->state = PLAYER_SLICING;
}

bool isInsideScreen(struct Player player)
{
    Vector2 clampedPos = {
            Clamp(player.position.x, player.radius, VIRTUAL_SCREEN_WIDTH - player.radius),
            Clamp(player.position.y, player.radius, VIRTUAL_SCREEN_HEIGHT - player.radius)};
    return player.position.x == clampedPos.x && player.position.y == clampedPos.y;
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
            player->collidingBall = &balls[i];
            return;
        }
    }

    player->colliding = false;
    player->collidingBall = NULL;
}

void drawPlayer(struct Player player)
{
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

    if (player.colliding)
    {
        if (player.state == PLAYER_DASHING)
            DrawCircleLinesV(roundVector2(player.position), (float)player.texture.width, BLUE);
        else
            DrawCircleLinesV(roundVector2(player.position), (float)player.texture.width / 2, RED);
    }
}

void drawPlayerShadow(struct Player player)
{
    DrawTexture(player.shadowTexture, (int)roundf(player.position.x) - player.texture.width / 2, (int)roundf(player.position.y) - player.texture.height / 2, shadowColor);
}