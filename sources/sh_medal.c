#include "score_handler.h"
#include "game.h"
#include "asset_loader.h"
#include "renderer.h"
#include "camera.h"
#include "raymath.h"

int medalsUnlocked = MEDAL_COUNT;
int medalsDisplayed = 0;
double medalGotTime[MEDAL_COUNT];
Vector2 medalTexSize = {15, 32};
Vector2 medalOffset = {4, 4};

void ScoreHandlerUnlockMedals()
{
    if (medalsDisplayed >= MEDAL_COUNT || frameCounter % 15 != 0) return;

    medalsDisplayed++;
    medalGotTime[medalsDisplayed] = GetTime();
    CameraSetShake(2, 1.f, .2f);
}

void ScoreHandlerDrawMedals()
{

}

void ScoreHandlerDrawMedalsStationary()
{
    for (int i = 1; i <= medalsDisplayed; i++)
    {
        Rectangle source = {(float)gameTextures.medals.width - (float)i * medalTexSize.x,
                            0,
                            medalTexSize.x,
                            medalTexSize.y};

        float halfTexW = roundf(medalTexSize.x / 2);
        Rectangle dest = {
                VIRTUAL_SCREEN_WIDTH - (halfTexW + medalOffset.x + (float)(i - 1) * (medalTexSize.x + medalOffset.x)),
                medalOffset.y,
                medalTexSize.x,
                medalTexSize.y};

        Vector2 origin = {halfTexW, 0};

        // Chad version
        // float rotation = (float)sin((medalGotTime[i] / GetTime()) * 20) * (float)((medalGotTime[i] / (GetTime() * 10)) * 100);

        // Nerd version
        float elapsedTime = GetTime() - medalGotTime[i];
        float decayRate = 0.2f;
        float initialAmplitude = 40.0f;
        float frequency = 4.0f;

        float damping = exp(-decayRate * elapsedTime);
        float rotation = initialAmplitude * damping * sin(frequency * elapsedTime);

        DrawTexturePro(gameTextures.medals, source, dest, origin, rotation, WHITE);
    }
}