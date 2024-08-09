#include "ball.h"
#include <string.h>
#include <math.h>
#include "global.h"
#include "raymath.h"
#include "asset_loader.h"
#include "renderer.h"
#include "score_handler.h"
#include "particle.h"
#include "game.h"
#include "camera.h"

#define sign(a) ((a > 0) ? 1 : -1)

ListNode *ballHead = NULL;
ListNode *ballSpawnPointHead = NULL;
float ballSpeed = .5f;
bool freezeBalls = false;
const double gameplaySpawnTime = 1.f;
const double tutorialSpawnTime = 1.6f;

BallNbrCount ballNbrCount_All;

BallClearer ballClearer = {NULL, 0, 0, true};

static Vector2 RandomDirection()
{
    // num = (rand() % (upper – lower + 1)) + lower
    // srand(time(0));

    int randDir = rand() % 4;

    Vector2 directionVector = Vector2Zero();
    switch (randDir)
    {
        case 0:
            directionVector = (Vector2){1.0f, 1.0f};
            break;
        case 1:
            directionVector = (Vector2){-1.0f, 1.0f};
            break;
        case 2:
            directionVector = (Vector2){-1.0f, -1.0f};
            break;
        case 3:
            directionVector = (Vector2){1.0f, -1.0f};
            break;
    }

    return directionVector;
}

void OnSplitMelon(Ball *ball, Vector2 splitDir)
{
    float newRadius = ball->radius/2;
    if (newRadius > MIN_BALL_RADIUS && ball->type != TYPE_ORANGE)
    {
        float spawnDirRad = atan2f(splitDir.y, splitDir.x) + PI / 2;
        if (spawnDirRad > PI * 2) spawnDirRad -= PI * 2;

        Vector2 spawnPos = {cosf(spawnDirRad) * newRadius / 2, sinf(spawnDirRad) * newRadius / 2};

        Vector2 spawnPosRight = Vector2Add(ball->position, spawnPos);
        Ball *ballRight = BallInit(spawnPosRight, newRadius, TYPE_MELON);
        BallSpawn(ballRight);

        Vector2 spawnPosLeft = Vector2Subtract(ball->position, spawnPos);
        Ball *ballLeft = BallInit(spawnPosLeft, newRadius, TYPE_MELON);
        BallSpawn(ballLeft);
    }
}

void OnDestroyMelon(Ball *ball)
{
    CREATE_PARTICLES(&particleHead, ParticlePresetJuice(ball->position, uiColorRed), (int)(ball->radius / 2.f) + 1);

    ScoreHandlerAddToScore(1);
}

void BallClearerBegin(Ball *ball, int slowDownFps)
{
    if (!ballClearer.clearingFinished) return;

    ballClearer.currentNode = ballHead;
    ballClearer.nodeIndex = 0;
    ballClearer.listLength = ListLength(&ballHead);
    ballClearer.ballToAvoid = ball;
    ballClearer.clearingFinished = false;
    targetFps = slowDownFps;
    // GameFreezeAllEntities(true);
}

void OnSplitOrange(Ball *ball, Vector2 splitDir)
{
    CREATE_PARTICLES(&particleHead, ParticlePresetJuice(ball->position, orangeColor), (int)(ball->radius / 2.f) + 1);

    if (ball->health > 2)
    {
        Sound hitSound = gameAudio.melonSplats[0];
        SetSoundPitch(hitSound, 2.f - (float)(ball->health) / 10.f);
        SoundPanToWorld(hitSound, ball->position, DEFAULT_SOUND_PAN_INTENSITY);
        PlaySound(hitSound);
        return;
    }

    // Screen wipe
    if (ball->health <= 1 && IS_GAME_STATE_PLAYABLE) BallClearerBegin(ball, 20);
    /*
    ListNode *currentNode = ballHead;
    int listLength = ListLength(&ballHead);
    for (int i = 0; i < listLength; i++)
    {
        if (currentNode == NULL) return;
        ListNode *nextNode = currentNode->next;
        if (currentNode->data != ball) BallSplit(currentNode->data, RandomDirection());
        currentNode = nextNode;
    }
    */
}

void OnDestroyOrange(Ball *ball)
{
    CREATE_PARTICLES(&particleHead, ParticlePresetJuice(ball->position, orangeColor), (int)(ball->radius / 2.f) + 1);

    ScoreHandlerAddToMultiplier(1);
}

void DrawDefault(Ball ball, Vector2 drawPos)
{
    DrawTextureEx(*ball.texture, Vector2Round(drawPos), 0, ball.textureScale, WHITE);
}

// This looks like garbage when it gets drawn
void DrawOrange(Ball ball, Vector2 drawPos)
{
    Texture2D haloTexture = gameTextures.orangeHalo;
    Vector2 haloPos = {ball.position.x - (float)haloTexture.width / 2.f, ball.position.y - (float)haloTexture.height / 2.f};
    Color haloColor = WHITE;
    haloColor.a = 100;
    DrawTextureEx(haloTexture, haloPos, 0, 1, haloColor);

    DrawDefault(ball, drawPos);
}

Ball *BallInit(Vector2 position, float radius, BallType type)
{
    Ball *ball = (Ball *)malloc(sizeof(Ball));

    ball->type = type;

    switch (type)
    {
        case TYPE_MELON:
            ball->health = 1;
            ball->texture = (radius > BALL_TOO_SMALL_FOR_CLEAN_SPLIT) ? &gameTextures.melonBig : &gameTextures.melonSmall;
            ball->drawFunction = DrawDefault;
            ball->onSplitFunction = OnSplitMelon;
            ball->onDestroyFunction = OnDestroyMelon;
            break;
        case TYPE_ORANGE:
            ball->health = 7;
            ball->texture = &gameTextures.orange;
            ball->drawFunction = DrawDefault;
            ball->onSplitFunction = OnSplitOrange;
            ball->onDestroyFunction = OnDestroyOrange;
            break;
        case TYPE_ARMOR:
            break;
    }

    ball->stateExecute = STATE_EXEC_BALL_MOVE;
    ball->speed = ballSpeed;
    ball->velocity = RandomDirection();
    ball->velocity = (Vector2){ball->velocity.x * ball->speed, ball->velocity.y * ball->speed};

    ball->radius = radius;

    ball->colliding = false;
    float colliderScale = .8f;
    ball->collisionBox = (Rectangle){ 0, 0, ball->radius * 2 * colliderScale, ball->radius * 2 * colliderScale};

    BallSetPosition(ball, position);

    ball->textureScale = (ball->radius * 2) / (float)ball->texture->width;
    ball->textureOffset = (Vector2){ ((float)ball->texture->width / 2.0f) * ball->textureScale,
                                     ((float)ball->texture->height / 2.0f) * ball->textureScale };

    ball->shadowScale = radius / ((float)gameTextures.melonShadow.width / 2.f);
    ball->shadowOffset = (Vector2){ 4.0f, 4.0f };

    return ball;
}

void BallUpdate(Ball *ball)
{
    BallCollisionBall(ball);
    ball->stateExecute(ball);
    BallCollisionScreen(ball);
}

void BallSetPosition(Ball *ball, Vector2 pos)
{
    ball->position = pos;
    ball->collisionBox.x = pos.x - ball->radius;
    ball->collisionBox.y = pos.y - ball->radius;
}

void BallStateMove(Ball *ball)
{
    BallSetPosition(ball, (Vector2) {ball->position.x + ball->velocity.x, ball->position.y + ball->velocity.y});
}

bool IsBallClearingFinished()
{
    return ballClearer.clearingFinished;
}

void BallClearerUpdate()
{
    // Slice all balls
    /*
    if (ListLength(&ballHead) > 1)
    {
        ballClearer.nodeIndex++;
        ListNode *nextNode = ballClearer.currentNode->next;
        if (ballClearer.currentNode->data != ballClearer.ballToAvoid) BallSplit(ballClearer.currentNode->data, RandomDirection());
        ballClearer.currentNode = (nextNode != NULL) ? nextNode : ballHead;

        return;
    }
    */

    // Slice all balls only once
    if (ballClearer.nodeIndex < ballClearer.listLength && ballClearer.currentNode != NULL)
    {
        ballClearer.nodeIndex++;
        ListNode *nextNode = ballClearer.currentNode->next;
        if (ballClearer.currentNode->data != NULL && ballClearer.currentNode->data != ballClearer.ballToAvoid) BallSplit(ballClearer.currentNode->data, RandomDirection());
        ballClearer.currentNode = nextNode;

        return;
    }

    if (ballClearer.ballToAvoid != NULL && ballClearer.ballToAvoid->type == TYPE_ORANGE) targetFps = 20;
    ballClearer.clearingFinished = true;
    if (IS_GAME_STATE_PLAYABLE) PlaySound(gameAudio.postBallClarity);
}

void BallClearerForceFinish()
{
    ballClearer.clearingFinished = true;
}

Ball *BallClearerTarget()
{
    if (ballClearer.currentNode == NULL) return NULL;
    return ballClearer.currentNode->data;
}

void BallCollisionScreen(Ball *ball)
{
    if ((ball->position.x >= (VIRTUAL_SCREEN_WIDTH - ball->radius)) || (ball->position.x <= ball->radius)) ball->velocity.x *= -1.0f;
    if ((ball->position.y >= (VIRTUAL_SCREEN_HEIGHT - ball->radius)) || (ball->position.y <= ball->radius)) ball->velocity.y *= -1.0f;
    ball->position.x = fmaxf(ball->radius, fminf(ball->position.x, VIRTUAL_SCREEN_WIDTH - ball->radius));
    ball->position.y = fmaxf(ball->radius, fminf(ball->position.y, VIRTUAL_SCREEN_HEIGHT - ball->radius));
}

void BallCollisionBall(Ball *ball)
{
    ListNode *currentBallNode = ballHead;
    if (currentBallNode == NULL) return;

    while(currentBallNode->next != NULL)
    {
        Ball *currentBall = (Ball*)currentBallNode->data;
        currentBallNode = currentBallNode->next;

        if (currentBall != NULL && ball == (Ball*)currentBall)
        {
            continue;
        }

        Vector2 dist =
                {
                        ball->position.x - currentBall->position.x,
                        ball->position.y - currentBall->position.y
                };
        Vector2 signDist =
                {
                        sign(dist.x),
                        sign(dist.y)
                };

        if (CheckCollisionRecs(ball->collisionBox, currentBall->collisionBox))
        {
            BallSetPosition(ball, (Vector2) {ball->position.x + signDist.x, ball->position.y + signDist.y});
            BallSetPosition(currentBall, (Vector2) {currentBall->position.x - signDist.x, currentBall->position.y - signDist.y});

            if (fabsf(dist.x) > fabsf(dist.y))
            {
                ball->velocity.x = signDist.x * fabsf(ball->velocity.x);
                currentBall->velocity.x = -signDist.x * fabsf(ball->velocity.x);
            }
            else
            {
                ball->velocity.y = signDist.y * fabsf(ball->velocity.y);
                currentBall->velocity.y = -signDist.y * fabsf(ball->velocity.y);
            }

            ball->colliding = true;
            return;
        }
    }

    ball->colliding = false;
}

float RadiusToSplatPitch(float radius)
{
    switch ((int)radius) {
        case RADIUS_LARGE:
            return .9f;
        case RADIUS_MEDIUM:
            return 1.2f;
        case RADIUS_SMALL:
            return 1.5f;
    }
}

void BallSplit(Ball *ball, Vector2 splitDir)
{
    if (ball == NULL) return;
    CameraSetShake(fminf(1.5f, ball->radius / 8), 1.f, .2f);

    ball->onSplitFunction(ball, splitDir);
    ball->health--;
    if (ball->health <= 0) BallDestroy(ball);
}

void BallDraw(Ball ball)
{
    Vector2 drawPos = {ball.position.x - ball.textureOffset.x, ball.position.y - ball.textureOffset.y};
    ball.drawFunction(ball, drawPos);
}

void BallDrawShadow(Ball ball)
{
    Vector2 ballShadowPos = {ball.position.x - ball.radius + ball.shadowOffset.x, ball.position.y - ball.radius + ball.shadowOffset.y };
    DrawTextureEx(gameTextures.melonShadow, Vector2Round(ballShadowPos), 0, ball.shadowScale, shadowColor);
}

// Ball Spawning ----------
void BallSpawn(Ball *ballToSpawn)
{
    ListNodePush(&ballHead, ballToSpawn);
    ballNbrCount_All.spawned++;
}

void BallDestroy(Ball *ball)
{
    if (ball == NULL) return;
    Sound splatSound = SoundPickRandom(gameAudio.melonSplats, MELON_SOUNDS_LENGTH);
    SoundPanToWorld(splatSound, ball->position, DEFAULT_SOUND_PAN_INTENSITY);
    SetSoundPitch(splatSound, RadiusToSplatPitch(ball->radius));
    PlaySound(splatSound);

    ball->onDestroyFunction(ball);

    ListNodeRemove(&ballHead, ball);
    ballNbrCount_All.destroyed++;
}

void BallDeSpawnAll()
{
    ballNbrCount_All.destroyed += ListLength(&ballHead);
    ListRemoveAllNodes(&ballHead);
}

void BallNbrCountReset(BallNbrCount *ballNbrCount)
{
    memset(ballNbrCount, 0, sizeof(BallNbrCount));
}

int NbrOfBallsOnScreen(BallNbrCount ballNbrCount)
{
    return ballNbrCount.spawned - ballNbrCount.destroyed;
}

int NbrOfRadiusBallsOnScreen(float radius)
{
    ListNode *currentBallNode = ballHead;

    int count = 0;
    while(currentBallNode != NULL)
    {
        Ball* currentBall = (Ball*)currentBallNode->data;
        if (currentBall->radius == radius) count++;

        currentBallNode = currentBallNode->next;
    }

    return count;
}

BallSpawnPoint *BallSpawnPointInit(Ball *mockBall, double spawnTime)
{
    BallSpawnPoint *instance = (BallSpawnPoint*)malloc(sizeof(BallSpawnPoint));
    instance->mockBall = mockBall;
    instance->initTime = GetTime();
    instance->spawnTime = spawnTime;

    return instance;
}

static double TimeSinceInit(double initTime)
{
    return GetTime() - initTime;
}

void BallSpawnPointUpdate(BallSpawnPoint *ballSpawnPoint)
{
    if (TimeSinceInit(ballSpawnPoint->initTime) < ballSpawnPoint->spawnTime) return;

    // Spawn a ball
    BallSpawn(ballSpawnPoint->mockBall);
    ListNodeRemove(&ballSpawnPointHead, ballSpawnPoint);
}

void BallSpawnPointDraw(BallSpawnPoint ballSpawnPoint)
{
    double timeSinceInit = TimeSinceInit(ballSpawnPoint.initTime);

    // Outer Circle
    double outerAnimTime = ballSpawnPoint.spawnTime * 1.f; // Maybe change this value?
    float outerRadius = (float)(fmin(timeSinceInit, outerAnimTime) / outerAnimTime);
    Color outerColor = {255, 255, 255, (unsigned char)(outerRadius * 255.f)};
    DrawCircleLinesV(ballSpawnPoint.mockBall->position, (1.f / outerRadius) * ballSpawnPoint.mockBall->radius, outerColor);

    // Inner Circle
    float innerRadius = (float)(timeSinceInit / ballSpawnPoint.spawnTime) * ballSpawnPoint.mockBall->radius;
    DrawCircleLinesV(ballSpawnPoint.mockBall->position, innerRadius, uiColorYellow);
}