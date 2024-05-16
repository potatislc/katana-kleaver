#include "ball.h"
#include <math.h>
#include "global.h"

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

    ball->texture = LoadTexture("../assets/melon_big.png");
    ball->textureScale = (ball->radius * 2) / (float)ball->texture.width;
    ball->textureOffset = (Vector2){ ((float)ball->texture.width / 2.0f) * ball->textureScale,
                                     ((float)ball->texture.height / 2.0f) * ball->textureScale };
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

void BallCollisionBall(Ball *ball, Ball balls[], int nbrOfBalls)
{
    for (int i = 0; i < nbrOfBalls; i++)
    {
        if (&balls[i] == ball)
        {
            continue;
        }

        Vector2 dist =
                {
                    ball->position.x - balls[i].position.x,
                    ball->position.y - balls[i].position.y
                };

        Vector2 signDist =
                {
                        sign(dist.x),
                        sign(dist.y)
                };

        while (CheckCollisionRecs(ball->collisionBox, balls[i].collisionBox))
        {
            BallSetPosition(ball, (Vector2) {ball->position.x + signDist.x, ball->position.y + signDist.y});
            BallSetPosition(&balls[i], (Vector2) {balls[i].position.x - signDist.x, balls[i].position.y - signDist.y});

            if (fabsf(dist.x) > fabsf(dist.y))
            {
                ball->speed.x = signDist.x * fabsf(ball->speed.x);
                balls[i].speed.x = -signDist.x * fabsf(ball->speed.x);
            }
            else
            {
                ball->speed.y = signDist.y * fabsf(ball->speed.y);
                balls[i].speed.y = -signDist.y * fabsf(ball->speed.y);
            }

            ball->colliding = true;
            return;
        }
    }

    ball->colliding = false;
}

void BallDraw(Ball ball)
{
    Vector2 drawPos = {ball.position.x - ball.textureOffset.x, ball.position.y - ball.textureOffset.y};
    DrawTextureEx(ball.texture, round_vector2(drawPos), 0, ball.textureScale, WHITE);
}

void BallDrawShadow(Ball ball)
{
    Vector2 ballShadowPos = {ball.position.x + ball.shadowOffset.x, ball.position.y + ball.shadowOffset.y };
    DrawCircleV(round_vector2(ballShadowPos), ball.shadowRadius, shadowColor);
}