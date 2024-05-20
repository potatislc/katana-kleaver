#include "ball.h"
#include <math.h>
#include "global.h"
#include "raymath.h"
#include "asset_loader.h"

// #define sign(a) ((a > 0) ? 1 : ((a < 0) ? -1 : 0))
#define sign(a) ((a > 0) ? 1 : -1)

void BallInit(Ball *ball, Vector2 initPos, float radius)
{
    ball->speed = (Vector2){ .5f, .5f };
    ball->radius = radius;
    ball->shadowRadius = radius;
    ball->shadowOffset = (Vector2){ 4.0f, 4.0f };

    ball->colliding = false;
    float colliderScale = .8f;
    ball->collisionBox = (Rectangle){ 0, 0, ball->radius * 2 * colliderScale, ball->radius * 2 * colliderScale};

    BallSetPosition(ball, initPos);

    ball->texture = (radius > BALL_TOO_SMALL_FOR_CLEAN_SPLIT) ? &melonBig : &melonSmall;
    ball->textureScale = (ball->radius * 2) / (float)ball->texture->width;
    ball->textureOffset = (Vector2){ ((float)ball->texture->width / 2.0f) * ball->textureScale,
                                     ((float)ball->texture->height / 2.0f) * ball->textureScale };
}

void BallSetPosition(Ball *ball, Vector2 pos)
{
    ball->position = pos;
    ball->collisionBox.x = pos.x - ball->radius;
    ball->collisionBox.y = pos.y - ball->radius;
}

void BallMove(Ball *ball)
{
    BallSetPosition(ball, (Vector2) {ball->position.x + ball->speed.x, ball->position.y + ball->speed.y});
}

void BallCollisionScreen(Ball *ball)
{
    if ((ball->position.x >= (VIRTUAL_SCREEN_WIDTH - ball->radius)) || (ball->position.x <= ball->radius)) ball->speed.x *= -1.0f;
    if ((ball->position.y >= (VIRTUAL_SCREEN_HEIGHT - ball->radius)) || (ball->position.y <= ball->radius)) ball->speed.y *= -1.0f;
    ball->position.x = fmaxf(ball->radius, fminf(ball->position.x, VIRTUAL_SCREEN_WIDTH - ball->radius));
    ball->position.y = fmaxf(ball->radius, fminf(ball->position.y, VIRTUAL_SCREEN_HEIGHT - ball->radius));
}

void BallCollisionBall(Ball *ball, ListNode *ballHead)
{
    ListNode *currentBallNode = ballHead;

    while(currentBallNode->next != NULL)
    {
        Ball *currentBall = (Ball*)currentBallNode->data;
        currentBallNode = currentBallNode->next;

        if (ball == (Ball*)currentBall)
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
                ball->speed.x = signDist.x * fabsf(ball->speed.x);
                currentBall->speed.x = -signDist.x * fabsf(ball->speed.x);
            }
            else
            {
                ball->speed.y = signDist.y * fabsf(ball->speed.y);
                currentBall->speed.y = -signDist.y * fabsf(ball->speed.y);
            }

            ball->colliding = true;
            return;
        }
    }

    ball->colliding = false;
}

void BallSplit(Ball *ball, ListNode **ballHeadRef, Vector2 splitDir)
{
    float newRadius = ball->radius/2;
    if (newRadius > MIN_BALL_RADIUS)
    {
        float spawnDirRad = atan2f(splitDir.y, splitDir.x) + PI / 2;
        if (spawnDirRad > PI * 2) spawnDirRad -= PI * 2;

        Vector2 spawnPos = {cosf(spawnDirRad) * newRadius / 2, sinf(spawnDirRad) * newRadius / 2};

        Ball *ballRight = (Ball *) malloc(sizeof(Ball));
        BallInit(ballRight, Vector2Add(ball->position, spawnPos), newRadius);
        ListNodePush(ballHeadRef, ballRight);

        Ball *ballLeft = (Ball *) malloc(sizeof(Ball));
        BallInit(ballLeft, Vector2Subtract(ball->position, spawnPos), newRadius);
        ListNodePush(ballHeadRef, ballLeft);
    }

    AddToScore(1); // Temp
    ListNodeRemove(ballHeadRef, ball);
}

void BallDraw(Ball ball)
{
    Vector2 drawPos = {ball.position.x - ball.textureOffset.x, ball.position.y - ball.textureOffset.y};
    DrawTextureEx(*ball.texture, Vector2Round(drawPos), 0, ball.textureScale, WHITE);
}

void BallDrawShadow(Ball ball)
{
    Vector2 ballShadowPos = {ball.position.x + ball.shadowOffset.x, ball.position.y + ball.shadowOffset.y };
    DrawCircleV(Vector2Round(ballShadowPos), ball.shadowRadius, shadowColor);
}