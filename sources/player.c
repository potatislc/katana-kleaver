#include "player.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "ball.h"
#include "asset_loader.h"
#include "renderer.h"
#include "score_handler.h"
#include "game.h"

#define sign(a) ((a > 0) ? 1 : -1)

Player *playerRef;

Vector2 ClampInsideScreen(Vector2 position, float radius)
{
    return (Vector2){
            Clamp(position.x, radius, VIRTUAL_SCREEN_WIDTH - radius),
            Clamp(position.y, radius, VIRTUAL_SCREEN_HEIGHT - radius)};
}

Player *PlayerInit(Vector2 initPos, ListNode **ballHeadRef)
{
    Player *player = (Player*) malloc(sizeof(Player));

    player->stateExecute = STATE_EXEC_PLAYER_MOVE;
    player->texture = &gameTextures.samurai;
    player->shadowTexture = &gameTextures.samuraiShadow;
    player->position = initPos;
    player->speed = 2;
    player->radius = 6;
    player->ballHeadRef = ballHeadRef;
    player->colliding = false;

    player->dash = (Dash *)malloc(sizeof(Dash));
    *player->dash = (Dash){false, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 48.0f, 10, 30, 30, 0};

    // Allocate memory for whatever ball that goes here
    player->collidingBall = (Ball *)malloc(sizeof(Ball));
    player->collidingBallCopy = (Ball *)malloc(sizeof(Ball));

    return player;
}

Player *PlayerReset(Player *player, Vector2 initPos, ListNode **ballHeadRef)
{
    free(player);
    return PlayerInit(initPos, ballHeadRef);
}

void PlayerDie(Player *player)
{
    player->stateExecute = STATE_EXEC_PLAYER_DEAD;
    GameEnd(); // Should be a signal to game instead
}

void PlayerUpdate(Player *player)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) player->dash->bufferedDash = true;

    player->stateExecute(player);

    player->position = ClampInsideScreen(player->position, player->radius);
    if (player->stateExecute != STATE_EXEC_PLAYER_SLICE) PlayerCollisionBall(player);
}

void PlayerStateMove(Player *player)
{
    if (player->dash->reloadTime > 0) player->dash->reloadTime--;

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        PlayerMoveToPoint(player, Vector2ToVirtualCoords(GetMousePosition()));
    }

    if (player->dash->bufferedDash && player->dash->reloadTime == 0)
    {
        player->dash->bufferedDash = false;
        PlayerBeginDash(player, Vector2ToVirtualCoords(GetMousePosition()));
    }

    if (player->colliding)
    {
        PlayerDie(player);
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

void PlayDashSound()
{
    float randPitch = .8f  + (float)(rand() % 16) / 16.f * .4f;
    SetSoundPitch(gameAudio.dash, randPitch);
    PlaySound(gameAudio.dash);
}

void PlayerBeginDash(Player *player, Vector2 point)
{
    PlayDashSound();

    player->stateExecute = STATE_EXEC_PLAYER_DASH;

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
    Vector2 clampedPoint = ClampInsideScreen(point, player->radius);

    Vector2 nextPosition = (Vector2){
            Lerp(player->position.x, point.x, player->dash->speed),
            Lerp(player->position.y, point.y, player->dash->speed) };

    if (Vector2Distance(player->position, clampedPoint) <= 2.f)
    {
        return false;
    }

    player->position = nextPosition;

    return true;
}

void PlayerStateDash(Player *player)
{
    if (player->colliding)
    {
        PlayerBeginSlice(player);
    }

    if (!PlayerLerpUntilPoint(player, player->dash->targetPos))
    {
        player->stateExecute = STATE_EXEC_PLAYER_MOVE;
        ScoreHandlerLoseCombo(); // Dash without successful slice means loss of combo
    }
}

void PlaySliceSound()
{
    float initPitch = .95f;
    float comboPitch = (comboScore > 5) ? initPitch + (float)comboScore * .005f : initPitch;
    SetSoundPitch(gameAudio.swordSlash, comboPitch);
    PlaySound(gameAudio.swordSlash);
}

void PlayerBeginSlice(Player *player)
{
    if (player->collidingBall == NULL) return; // Error Handling

    PlaySliceSound();

    player->stateExecute = STATE_EXEC_PLAYER_SLICE;
    freezeBalls = true;
    player->dash->startPos = player->position;

    Vector2 ballPos = player->collidingBall->position;
    float ballRadius = player->collidingBall->radius;

    Vector2 sliceTargetPoint;

    if (ballRadius <= BALL_TOO_SMALL_FOR_CLEAN_SPLIT)
    {
        float length = ballRadius*2 + player->radius;
        Vector2 direction = Vector2Normalize(player->velocity);
        sliceTargetPoint = (Vector2){direction.x*length, direction.y*length};
    }
    else
    {
        Vector2 distance = (Vector2){ballPos.x - player->position.x, ballPos.y - player->position.y};
        sliceTargetPoint = Vector2Normalize(distance);
        sliceTargetPoint = (Vector2){sliceTargetPoint.x * ballRadius + sliceTargetPoint.x * player->radius,
                                     sliceTargetPoint.y * ballRadius + sliceTargetPoint.y * player->radius};
    }

    player->velocity = sliceTargetPoint;

    player->dash->targetPos = (Vector2){ballPos.x + sliceTargetPoint.x, ballPos.y + sliceTargetPoint.y};

    BallSplit(player->collidingBall, Vector2Normalize(sliceTargetPoint));
}

void PlayerStateSlice(Player *player)
{
    if (!PlayerLerpUntilPoint(player, player->dash->targetPos))
    {
        // Perform another slice if it ends inside a ball
        PlayerCollisionBall(player);
        if (player->colliding)
        {
            PlayerBeginSlice(player);
            return;
        }

        // End Dash
        player->stateExecute = STATE_EXEC_PLAYER_MOVE;
        freezeBalls = false;
        player->dash->reloadTime = 0;
    }
}

void PlayerStateIdle(Player *player)
{
    // Chill out
}

void PlayerStateDead(Player *player)
{
    // Chill out
}

void PlayerCollisionBall(Player *player)
{
    ListNode *currentBallNode = *player->ballHeadRef;
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

static void DrawArrowTo(Vector2 from, Vector2 to, float offset, float width, float length, float perspectiveYShift, Color color)
{
    Vector2 dir = Vector2Normalize(Vector2Subtract(to, from));
    float anglePerpendicular = atan2f(dir.y, dir.x) + PI / 2;
    Vector2 dirPerpendicular = {cosf(anglePerpendicular), sinf(anglePerpendicular)};

    float middleLength = offset + length;
    Vector2 pointMiddle = {dir.x * middleLength, dir.y * middleLength * perspectiveYShift};

    Vector2 pointRight = {
            dir.x * offset + dirPerpendicular.x * width,
            (dir.y * offset + dirPerpendicular.y * width) * perspectiveYShift};
    Vector2 pointLeft = {
            dir.x * offset - dirPerpendicular.x * width,
            (dir.y * offset - dirPerpendicular.y * width) * perspectiveYShift};

    DrawTriangle(
            Vector2Add(from, pointLeft),
            Vector2Add(from, pointRight),
            Vector2Add(from, pointMiddle),
            color);
}

static void DrawDashRing(Dash *dash, Vector2 pos, float innerRadius, float outerRadius, Color color)
{
    if (dash->reloadTime <= 0) return;
    float ringPotion = (float)dash->reloadTime / (float)dash->initReloadTime;
    DrawRing(Vector2Round(pos), innerRadius, outerRadius, 0, ringPotion*ringPotion*(360), 12, color);
}

void PlayerDraw(Player player)
{
    if (player.stateExecute == STATE_EXEC_PLAYER_DEAD)
    {
        DrawCircleV(Vector2Round(player.position), 8, uiColorRed);
        return;
    }

    Vector2 footPos = {player.position.x, player.position.y + (float)player.texture->height / 2.f - 2.f};

    DrawArrowTo(footPos, Vector2ToVirtualCoords(GetMousePosition()), 8, 5, 10, .7f, guideColor);
    if (player.stateExecute == STATE_EXEC_PLAYER_MOVE)
        DrawDashRing(player.dash, (Vector2){player.position.x, player.position.y - 12}, 0.f, 4.f, guideColor);

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

    if (player.stateExecute == STATE_EXEC_PLAYER_SLICE) PlayerDrawSlice(player);
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