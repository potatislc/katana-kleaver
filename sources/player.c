#include "player.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "ball.h"
#include "asset_loader.h"
#include "score_handler.h"
#include "game.h"
#include "particle.h"
#include "camera.h"
#include "renderer.h"

#define sign(a) ((a > 0) ? 1 : -1)

Player *playerRef;

int deathBuffer; // Some grace frames when colliding, when the buffer exceeds max, the player dies
const int maxDeathBuffer = 4;
bool freezePlayer = false;

Player *PlayerInit(Vector2 initPos, ListNode **ballHeadRef)
{
    Player *player = (Player*) malloc(sizeof(Player));

    player->stateExecute = STATE_EXEC_PLAYER_IDLE;
    player->spriteIdle = SpriteInit(gameTextures.samurai, (Rectangle){0, 0, 16, 16}, 0, true);
    player->spriteRun = SpriteInit(gameTextures.samuraiRunSheet, (Rectangle){0, 0, 16, 16}, 0, true);
    player->spriteDie = SpriteInit(gameTextures.samuraiDieSheet, (Rectangle){0, 0, 48, 48}, 0, true);
    player->shadowTexture = &gameTextures.samuraiShadow;
    player->position = initPos;
    player->speed = 2;
    player->radius = 6;
    player->ballHeadRef = ballHeadRef;
    player->colliding = false;

    player->dash = (Dash *)malloc(sizeof(Dash));
    *player->dash = (Dash){false, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 48.0f, 10, 30, 30, 0};

    player->collidingBall = (Ball *)malloc(sizeof(Ball));
    player->collidingBallCopy = (Ball *)malloc(sizeof(Ball));

    return player;
}

Player *PlayerReset(Player *player, Vector2 initPos, ListNode **ballHeadRef)
{
    free(player->dash);
    free(player->spriteIdle);
    free(player->spriteRun);
    free(player->collidingBallCopy);
    free(player);
    return PlayerInit(initPos, ballHeadRef);
}

void PlayerDie(Player *player)
{
    Sound deathSound = gameAudio.deathScream;
    SoundPanToWorld(deathSound, player->position, DEFAULT_SOUND_PAN_INTENSITY);
    PlaySound(deathSound);

    CameraSetShake(4, 1.f, 2.f);

    CREATE_PARTICLES(ParticlePresetPlayerBlood(player->position), 35);

    player->stateExecute = STATE_EXEC_PLAYER_DEAD;
    deathBuffer = 0;
    GameEnd();
}

void PlayerUpdate(Player *player)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) player->dash->bufferedDash = true;

    player->stateExecute(player);

    player->radius = (player->stateExecute == STATE_EXEC_PLAYER_MOVE) ? 6.f : 8.f;
    player->position = Vector2ClampInsideScreen(player->position, player->radius);
    if (player->stateExecute != STATE_EXEC_PLAYER_SLICE) PlayerCollisionBall(player);

    SoundPanToWorld(gameAudio.footstep, player->position, DEFAULT_SOUND_PAN_INTENSITY);
}

void PlayerStateMove(Player *player)
{
    if (player->dash->reloadTime > 0) player->dash->reloadTime--;

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        PlayerMoveToPoint(player, Vector2ToVirtualCoords(GetMousePosition()));
        if (!IsSoundPlaying(gameAudio.footstep)) PlaySound(gameAudio.footstep);
    }

    if (player->dash->bufferedDash && player->dash->reloadTime == 0)
    {
        player->dash->bufferedDash = false;
        PlayerBeginDash(player, Vector2ToVirtualCoords(GetMousePosition()));
    }

    if (player->colliding && IsBallClearingFinished() && gameState != GAME_TUTORIAL)
    {
        deathBuffer++;
        if (deathBuffer > maxDeathBuffer) PlayerDie(player);
    }
    else
    {
        deathBuffer = 0;
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
    SoundPanToWorld(gameAudio.dash, player->position, DEFAULT_SOUND_PAN_INTENSITY);

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
    Vector2 clampedPoint = Vector2ClampInsideScreen(point, player->radius);

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
        PlaySound(gameAudio.footstep);
        player->stateExecute = STATE_EXEC_PLAYER_MOVE;
        if (IsBallClearingFinished())
        {
            ScoreHandlerLoseCombo(); // Dash without successful slice means loss of combo
        }
        else
        {
            player->dash->reloadTime = 0;
        }
    }
}

void PlaySliceSound()
{
    float initPitch = .95f;
    float comboPitch = (ScoreHandlerGetComboScoreBuffer() >= COMBO_BUFFER_SIZE) ? initPitch + ((float)ScoreHandlerGetComboScore()+1) * .005f : initPitch;
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
    SoundPanToWorld(gameAudio.swordSlash, player->position, DEFAULT_SOUND_PAN_INTENSITY);

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

void PlayerStateRevive(Player *player)
{
    PlayerMoveToPoint(player, virtualScreenCenter);
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

void PlayerDraw(Player player) {
    const float footOffset = (float) player.spriteIdle->frameRect.height / 2.f - 2.f;
    const Vector2 footPos = {player.position.x, player.position.y + footOffset};

    if(player.stateExecute != STATE_EXEC_PLAYER_DEAD)
    {
        DrawArrowTo(footPos, Vector2ToVirtualCoords(GetMousePosition()), 8, 5, 10, .7f, guideColor);
    }

    if (player.stateExecute == STATE_EXEC_PLAYER_MOVE)
    {
        DrawDashRing(player.dash, (Vector2){player.position.x, player.position.y - 12}, 0.f, 4.f, guideColor);
    }

    // This didnt help at all
    /*
    Vector2 footToMouseDir = Vector2Normalize(Vector2Subtract(Vector2ToVirtualCoords(GetMousePosition()), footPos));
    Vector2 linePoint = {footToMouseDir.x * 240, footToMouseDir.y * 240};
    DrawLineV(footPos, Vector2Add(footPos, linePoint), guideColor);
    */

    // This didn't either
    /*
    if (player.stateExecute == STATE_EXEC_PLAYER_DASH)
    {
        const Vector2 footTargetPos = Vector2Add(player.dash->targetPos, (Vector2){0, footOffset});
        DrawCircleLinesV(Vector2Round(footTargetPos), 3.f, guideColor);
    }
    */

    Vector2 spriteFacing = {sign(player.velocity.x), 1};

    if (player.stateExecute == STATE_EXEC_PLAYER_MOVE)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            SpriteAnimate(player.spriteRun, .05f, true);
            SpriteDraw(*player.spriteRun, player.position, spriteFacing, 0);
        }
        else
        {
            SpriteDraw(*player.spriteIdle, player.position, spriteFacing, 0);
        }
    }
    else if (player.stateExecute == STATE_EXEC_PLAYER_DEAD)
    {
        SpriteAnimate(player.spriteDie, .015f, false);
        SpriteDraw(*player.spriteDie, player.position, spriteFacing, 0);
    }
    else if (player.stateExecute == STATE_EXEC_PLAYER_REVIVE)
    {
        SpriteAnimate(player.spriteDie, -.015f, false);
        SpriteDraw(*player.spriteDie, player.position, spriteFacing, 0);
    }
    else if (player.stateExecute == STATE_EXEC_PLAYER_IDLE)
    {
        spriteFacing = (Vector2){sign(Vector2ToVirtualCoords(GetMousePosition()).x - player.position.x), 1};
        SpriteDraw(*player.spriteIdle, player.position, spriteFacing, 0);
    }
    else
    {
        SpriteDraw(*player.spriteIdle, player.position, spriteFacing, 0);
    }

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
    DrawTexture(*player.shadowTexture, (int)roundf(player.position.x) - player.spriteIdle->frameRect.width / 2, (int)roundf(player.position.y) - player.spriteIdle->frameRect.height / 2, shadowColor);
}