#include <stdio.h>
#include "renderer.h"
#include "asset_loader.h"
#include "player.h"
#include "window_handler.h"
#include "global.h"
#include "score_handler.h"
#include "particle.h"
#include "raymath.h"

Vector2 virtualScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

RenderTexture2D virtualRenderTarget;
RenderTexture2D backgroundPaintTarget;
TextureTransformRect virtualRenderRect;
TextureTransformRect backgroundPaintRect;

Camera2D worldSpaceCamera = { 0 };

Vector2 screenOffset;
Vector2 screenRatio;

UiProgressBar spawnProgressBar = {VIRTUAL_SCREEN_WIDTH, 0, 0};

void RendererInit()
{
    virtualRenderTarget = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);
    virtualRenderRect.source = (Rectangle){ 0.f, 0.f, (float)virtualRenderTarget.texture.width, -(float)virtualRenderTarget.texture.height };
    virtualRenderRect.dest = (Rectangle){0.f, 0.f ,0.f, 0.f};
    virtualRenderRect.origin = Vector2Zero();
    worldSpaceCamera.zoom = 1.0f;
    RendererFitVirtualRectToScreen();

    backgroundPaintTarget = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    backgroundPaintRect.source = (Rectangle){ 0.f, 0.f, (float)backgroundPaintTarget.texture.width, -(float)backgroundPaintTarget.texture.height };
    backgroundPaintRect.dest = (Rectangle){0.f, 0.f ,0.f, 0.f};
    backgroundPaintRect.origin = Vector2Zero();

}

void RendererFitVirtualRectToScreen()
{
    int windowHeight = GetScreenHeight();
    float aspectRatio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;
    int newWidth = (int)((float)windowHeight * aspectRatio); // Calculate the new width based on the aspect ratio

    screenOffset = (Vector2){(float)(GetScreenWidth() - newWidth) / 2.f, 0.f};

    screenRatio = (Vector2){
            ((float)newWidth) / (float)VIRTUAL_SCREEN_WIDTH,
            (float)GetScreenHeight() / (float)(VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y)};

    virtualRenderRect.dest = (Rectangle){
            screenOffset.x,
            screenOffset.y,
            (float)newWidth,
            (float)GetScreenHeight()};
}

void DrawBackground()
{
    DrawTexture(gameTextures.floorStandard, 0, 0, WHITE);
    Color paintBgAlphaColor = WHITE;
    paintBgAlphaColor.a = 100;
    DrawTextureRec(backgroundPaintTarget.texture, backgroundPaintRect.source, backgroundPaintRect.origin, paintBgAlphaColor);
}

void DrawShadows()
{
    PlayerDrawShadow(*playerRef);

    ListNode* currentBallNode = ballHead;
    while (currentBallNode != NULL)
    {
        BallDrawShadow(*(Ball*)currentBallNode->data);
        currentBallNode = currentBallNode->next;
    }
}

void DrawEntities()
{
    ListNode* currentBallNode = ballHead;

    while (currentBallNode != NULL)
    {
        BallDraw(*(Ball*)currentBallNode->data);
        currentBallNode = currentBallNode->next;
    }

    ListNode* currentBallSpawnPointNode = ballSpawnPointHead;
    while (currentBallSpawnPointNode != NULL)
    {
        BallSpawnPointDraw(*(BallSpawnPoint*)currentBallSpawnPointNode->data);
        currentBallSpawnPointNode = currentBallSpawnPointNode->next;
    }

    PlayerDraw(*playerRef);
}

void DrawParticles()
{
    ListNode* currentParticleNode = particleHead;

    while (currentParticleNode != NULL) {
        ParticleDraw(*(Particle *) currentParticleNode->data);
        currentParticleNode = currentParticleNode->next;
    }
}

void DrawParticlesToBackgroundPaint()
{
    ListNode* currentParticleNode = particleHead;

    BeginTextureMode(backgroundPaintTarget);
    {
        while (currentParticleNode != NULL) {
            ParticleDraw(*(Particle *) currentParticleNode->data);
            currentParticleNode = currentParticleNode->next;
        }
    }
    EndTextureMode();
}

void RendererClearBackgroundPaint()
{
    BeginTextureMode(backgroundPaintTarget);
    {
        ClearBackground(BLANK);
    }
}

void SetUiProgressBarLToR(UiProgressBar *uiProgressBar, double current, double end)
{
    // Left to right
    uiProgressBar->start = 0;
    uiProgressBar->end = (float)(current / end) * uiProgressBar->width;
}

void SetUiProgressBarMidToEnds(UiProgressBar *uiProgressBar, double current, double end)
{
    float progress = (float)(current / end);
    float maxLength = uiProgressBar->width / 2.f;
    uiProgressBar->start = maxLength - progress * maxLength;
    uiProgressBar->end = maxLength + progress * maxLength;
}

void DrawUi(bool gameOver)
{
    if (gameOver)
    {
        DrawText(gameOverText, (int)virtualScreenCenter.x - gameOverTextWidth / 2, (int)virtualScreenCenter.y-8, 8, WHITE);
        DrawText(restartText, (int)virtualScreenCenter.x - restartTextWidth / 2, (int)virtualScreenCenter.y+64, 8, WHITE);

        int scoreTextWidth = MeasureText(scoreText, 8);
        DrawText(scoreText, (int)virtualScreenCenter.x - scoreTextWidth / 2, (int)virtualScreenCenter.y+12, 8, WHITE);

        DrawText(hiScoreText, (int)virtualScreenCenter.x - hiScoreTextWidth / 2, (int)virtualScreenCenter.y+24, 8, WHITE);
    }

    // Mouse Icon
    Vector2 mousePos = Vector2ClampInsideScreen(Vector2Round(Vector2ToVirtualCoords(GetMousePosition())), 2);
    DrawCircleV(mousePos, 2, guideColor);

    // Black Box
    DrawRectangle(0, VIRTUAL_SCREEN_HEIGHT, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_OFFSET_Y, BLACK);

    DrawText(scoreText, 5, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorYellow);

    if (ScoreHandlerGetComboScore() > 1)
    {
        DrawText(comboText, 69, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorRed);
    }

    // Draw progressbar until next spawn
    const Vector2 progLineStart = (Vector2){spawnProgressBar.start, VIRTUAL_SCREEN_HEIGHT};
    const Vector2 progLineEnd = (Vector2){spawnProgressBar.end, VIRTUAL_SCREEN_HEIGHT};
    DrawLineV(progLineStart, progLineEnd, uiColorDarkGray);
}

void RenderToTarget(bool gameOver)
{
    DrawParticlesToBackgroundPaint();

    BeginTextureMode(virtualRenderTarget);
    {
        BeginMode2D(worldSpaceCamera);
        {
            DrawBackground();
            DrawShadows();
            DrawEntities();
            DrawParticles();
        }
        EndMode2D();
        DrawUi(gameOver);
    }
    EndTextureMode();
}

void DrawDebug()
{
    DrawFPS(10, 10);

    char windowModeText[32];
    sprintf(windowModeText, "WINDOW MODE %d", WindowHandlerGetWindowMode());
    DrawText(windowModeText, 10, 30, 21, DARKGREEN);

    char ballCountText[32];
    sprintf(ballCountText, "BALLS ON SCREEN %d", NbrOfBallsOnScreen(ballNbrCount_All));
    DrawText(ballCountText, 10, 50, 21, DARKGREEN);
}

void RenderToScreen()
{
    BeginDrawing();
    {
        ClearBackground(BLACK);

        // Draw world camera to screen
        DrawTexturePro(virtualRenderTarget.texture, virtualRenderRect.source, virtualRenderRect.dest, virtualRenderRect.origin, 0.0f, WHITE);

        DrawDebug();
    }
    EndDrawing();
}