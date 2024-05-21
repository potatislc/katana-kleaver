#include "player.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "ball.h"
#include "asset_loader.h"

#define sign(a) ((a > 0) ? 1 : -1)

void PlayerInit(Player *player, Vector2 initPos, ListNode **ballHeadRef)
{
    player->state = PLAYER_MOVING;
    player->texture = &samurai;
    player->shadowTexture = &samuraiShadow;
    player->position = initPos;
    player->speed = 2;
    player->radius = 6;
    player->ballHeadRef = ballHeadRef;
    player->colliding = false;

    player->dash = (Dash *)malloc(sizeof(Dash));
    if (player->dash != NULL) {
        // Initialize the allocated memory
        *player->dash = ( Dash){0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 48.0f, 10, 30, 30, 0};
    } else {
        // Handle failure to allocate memory IDK :/
    }

    // Allocate memory for whatever ball that goes here
    player->collidingBall = (Ball *)malloc(sizeof(Ball));
    player->collidingBallCopy = (Ball *)malloc(sizeof(Ball));
}

void PlayerUpdate(Player *player, ListNode *ballHead)
{
    switch(player->state)
    {
        case PLAYER_IDLE:
            break;
        case PLAYER_MOVING:
            PlayerMove(player);
            break;
        case PLAYER_DASHING:
            PlayerDash(player);
            break;
        case PLAYER_SLICING:
            PlayerSlice(player);
            break;
    }

    PlayerCollisionScreen(player);
    if (player->dash->reloadTime > 0) player->dash->reloadTime--;
    if (player->state != PLAYER_SLICING) PlayerCollisionBall(player, ballHead);
}

void PlayerMove(Player *player)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        PlayerMoveToPoint(player, Vector2ToVirtualCoords(GetMousePosition()));
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player->dash->reloadTime == 0)
    {
        PlayerBeginDash(player, Vector2ToVirtualCoords(GetMousePosition()));
    }

    if (player->colliding)
    {
        player->state = PLAYER_DEAD;
    }
}

void PlayerMoveToPoint(Player *player, Vector2 point)
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

void PlayerBeginDash(Player *player, Vector2 point)
{
    player->state = PLAYER_DASHING;
    player->dash->reloadTime = player->dash->initReloadTime;

    Vector2 direction = Vector2Normalize((Vector2){player->position.x - point.x, player->position.y - point.y});
    player->dash->targetPos = direction;
    player->dash->targetPos = (Vector2){
            player->position.x - player->dash->targetPos.x * player->dash->distance,
            player->position.y - player->dash->targetPos.y * player->dash->distance };

    player->velocity = (Vector2){-direction.x, -direction.y};
}

bool PlayerLerpUntilPoint(Player *player, Vector2 point)
{
    player->position = (Vector2){
            Lerp(player->position.x, point.x, player->dash->speed),
            Lerp(player->position.y, point.y, player->dash->speed) };

    if (Vector2Distance(player->position, point) <= 2 || !IsInsideScreen(*player))
    {
        return true;
    }

    return false;
}

void PlayerDash(Player *player)
{
    if (player->colliding)
    {
        PlayerBeginSlice(player);
    }

    if (PlayerLerpUntilPoint(player, player->dash->targetPos))
    {
        player->state = PLAYER_MOVING;
        comboScore = 0; // Dash without successful slice means loss of combo
    }
}

void PlayerBeginSlice(Player *player)
{
    if (player->collidingBall == NULL) return; // Error Handling

    player->state = PLAYER_SLICING;
    freezeBalls = true;
    player->dash->startPos = player->position;

    Vector2 ballPos = player->collidingBall->position;
    float ballRadius = player->collidingBall->radius;

    Vector2 sliceTargetPoint;

    if (ballRadius <= BALL_TOO_SMALL_FOR_CLEAN_SPLIT)
    {
        float length = ballRadius*2 + player->radius;
        sliceTargetPoint = (Vector2){player->velocity.x*length, player->velocity.y*length};
    }
    else
    {
        Vector2 distance = (Vector2){ballPos.x - player->position.x, ballPos.y - player->position.y};
        sliceTargetPoint = Vector2Normalize(distance);
        sliceTargetPoint = (Vector2){sliceTargetPoint.x * ballRadius + sliceTargetPoint.x * player->radius, sliceTargetPoint.y * ballRadius + sliceTargetPoint.y * player->radius};
    }

    player->velocity = sliceTargetPoint;

    player->dash->targetPos = (Vector2){ballPos.x + sliceTargetPoint.x, ballPos.y + sliceTargetPoint.y};

    BallSplit(player->collidingBall, player->ballHeadRef, Vector2Normalize(sliceTargetPoint));
}

void PlayerSlice(Player *player)
{
    if (PlayerLerpUntilPoint(player, player->dash->targetPos))
    {
        player->state = PLAYER_MOVING;
        freezeBalls = false;
        player->dash->reloadTime = 0;
    }
}

bool IsInsideScreen(Player player)
{
    Vector2 clampedPos = {
            Clamp(player.position.x, player.radius, VIRTUAL_SCREEN_WIDTH - player.radius),
            Clamp(player.position.y, player.radius, VIRTUAL_SCREEN_HEIGHT - player.radius)};
    return player.position.x == clampedPos.x && player.position.y == clampedPos.y;
}

void PlayerCollisionScreen(Player *player)
{
    player->position.x = fmaxf(player->radius, fminf(player->position.x, VIRTUAL_SCREEN_WIDTH - player->radius));
    player->position.y = fmaxf(player->radius, fminf(player->position.y, VIRTUAL_SCREEN_HEIGHT - player->radius));
}

void PlayerCollisionBall(Player *player, ListNode *ballHead)
{
    ListNode *currentBallNode = ballHead;
    while (currentBallNode != NULL)
    {
        Ball currentBall = *(Ball*)currentBallNode->data;

        Vector2 ballDistVec = { currentBall.position.x - player->position.x, currentBall.position.y - player->position.y };
        float ballDist = ballDistVec.x * ballDistVec.x + ballDistVec.y * ballDistVec.y;

        if (ballDist <= player->radius * player->radius + currentBall.radius * currentBall.radius)
        {
            player->colliding = true;
            player->collidingBall = currentBallNode->data;
            *player->collidingBallCopy = currentBall;
            return;
        }

        currentBallNode = currentBallNode->next;
    }

    player->colliding = false;
    player->collidingBall = NULL;
}

void PlayerDraw(Player player)
{
    if (player.state == PLAYER_DEAD) return;

    Vector2 textureOffset = { (float)player.texture->width / 2.0f, (float)player.texture->height / 2.0f };

    Rectangle playerRect =
            {
                0,
                0,
                sign(player.velocity.x) * player.texture->width,
                (float)player.texture->height
            };

    DrawTextureRec
    (
        *player.texture,
        playerRect,
        (Vector2){ player.position.x - textureOffset.x,player.position.y - textureOffset.y },
        WHITE
    );

    if (player.state == PLAYER_SLICING) PlayerDrawSlice(player);
}

void PlayerDrawSlice(Player player)
{
    // Draw a shape from start pos to target pos, width is dependent on distance until slice is finished
    float distanceTotal = Vector2Distance(player.dash->targetPos, player.dash->startPos);
    float distanceLeft = Vector2Distance(player.dash->targetPos, player.position);
    float sliceRatio = distanceLeft / distanceTotal;

    Vector2 dashDir = {player.dash->targetPos.x - player.dash->startPos.x, player.dash->targetPos.y - player.dash->startPos.y};
    dashDir = Vector2Normalize(dashDir);
    float dashDirDegrees = atan2f(dashDir.y, dashDir.x);
    float tipAngle = PI * .3f;
    Vector2 orthogonalDir = {cosf(dashDirDegrees + tipAngle), sinf(dashDirDegrees + tipAngle)};

    float triangleWidth = sliceRatio * 16;
    Vector2 point1 = player.dash->startPos;
    Vector2 point2 = Vector2Add(player.dash->targetPos, (Vector2){orthogonalDir.x * triangleWidth, orthogonalDir.y * triangleWidth});
    Vector2 point3 = Vector2Add(player.dash->targetPos, (Vector2){-orthogonalDir.x * triangleWidth, -orthogonalDir.y * triangleWidth});

    // Sword slice triangle
    DrawTriangle(point1, point2, point3, WHITE);

    // Melon exploding
    Color circleColor = {255, 255, 255, sliceRatio * 255};
    DrawCircleV(player.collidingBallCopy->position, player.collidingBallCopy->radius, circleColor);
}

void PlayerDrawShadow(Player player)
{
    DrawTexture(*player.shadowTexture, (int)roundf(player.position.x) - player.texture->width / 2, (int)roundf(player.position.y) - player.texture->height / 2, shadowColor);
}