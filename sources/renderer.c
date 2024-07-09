#include <stdio.h>
#include "renderer.h"
#include "asset_loader.h"
#include "player.h"
#include "window_handler.h"
#include "global.h"
#include "score_handler.h"
#include "particle.h"
#include "raymath.h"
#include "game.h"
#include "camera.h"
#include "tutorial.h"
#include "firefly.h"

Vector2 virtualScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

RenderTexture2D virtualRenderTarget;
RenderTexture2D backgroundPaintTarget;
TextureTransformRect virtualRenderRect;
TextureTransformRect backgroundPaintRect;

Vector2 screenOffset;
Vector2 screenRatio;

UiProgressBar spawningProgressBar = {VIRTUAL_SCREEN_WIDTH, 0, 0};

Texture2D *bgTexture;

const char getReadyText[] = "- Get Ready! -";
int getReadyTextWidth = 0;

Vector2 titleScreenOffset;

float eraserRadius = 31.f;

bool debugDrawing = false;

typedef struct
{
    float startAngle;
    float endAngle;
    double duration;
    double timeSinceLastTransition;
    void (*transitionFunction)();
} RingTransition;

RingTransition ringTrans = {360.f, 360.f, .8, 0};

Vector2 mousePos;

typedef struct
{
    Color color;
    float radius;
    int blendMode;
    double sprayDuration;
    double timeSinceLastSpray;
} SprayCan;

SprayCan sprayCanFoam = {WHITE, 16.f, BLEND_SUBTRACT_COLORS, .15, 0};

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

    bgTexture = &gameTextures.bgFloorStandard;

    getReadyTextWidth = MeasureText(getReadyText, 8);
}

void RendererFitVirtualRectToScreen()
{
    int windowHeight = GetScreenHeight();
    float aspectRatio = (float)VIRTUAL_SCREEN_WIDTH / (float)(VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);
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
    if (!IsBallClearingFinished() && IS_GAME_STATE_PLAYABLE)
    {
        if (bgTexture != &gameTextures.bgBallClear)
        {
            bgTexture = &gameTextures.bgBallClear;
            DrawTexture(*bgTexture, 0, 0, WHITE);
        }
    }
    else
    {
        bgTexture = &gameTextures.bgFloorStandard;
        DrawTexture(*bgTexture, 0, 0, WHITE);

        Color paintBgAlphaColor = WHITE;
        paintBgAlphaColor.a = 200;
        DrawTextureRec(backgroundPaintTarget.texture, backgroundPaintRect.source, backgroundPaintRect.origin, paintBgAlphaColor);
    }
}

void DrawShadows()
{
    PlayerDrawShadow(*playerRef);
    LIST_MAP_DATA(ballHead, BallDrawShadow, *(Ball *));
}

void DrawEntities()
{
    LIST_MAP_DATA(ballHead, BallDraw, *(Ball *));
    LIST_MAP_DATA(ballSpawnPointHead, BallSpawnPointDraw, *(BallSpawnPoint *));
    PlayerDraw(*playerRef);
    FirefliesDraw();
}

void DrawParticles()
{
    LIST_MAP_DATA(particleHead, ParticleDraw, *(Particle *));
}

void DrawParticlesToBackgroundPaint()
{
    BeginTextureMode(backgroundPaintTarget);
    {
        LIST_MAP_DATA(particleHead, ParticleDraw, *(Particle *));
    }
    EndTextureMode();
}

void RendererClearBackgroundPaint()
{
    BeginTextureMode(backgroundPaintTarget);
    {
        ClearBackground(BLANK);
    }
    EndTextureMode();
}

void EraseBackgroundPaint(Vector2 position, float radius)
{
    BeginTextureMode(backgroundPaintTarget);
    {
        BeginBlendMode(BLEND_SUBTRACT_COLORS);
        {
            DrawCircleV(position, radius, BLANK);
        }
        EndBlendMode();
    }
    EndTextureMode();

    if (!IsSoundPlaying(gameAudio.wipes[0]) && !IsSoundPlaying(gameAudio.wipes[1]))
    {
        Sound wipeSound = SoundPickRandom(gameAudio.wipes, WIPE_SOUNDS_LENGTH);
        if (!IsSoundPlaying(wipeSound))
        {
            SoundPanToWorld(wipeSound, position, DEFAULT_SOUND_PAN_INTENSITY);
            SetSoundPitch(wipeSound, .9f + ((float)rand() / RAND_MAX) * .5f);
            PlaySound(wipeSound);
        }
    }
}

void SprayBackgroundPaint(Vector2 position, SprayCan sprayCan)
{
    position = (Vector2){position.x + (float)cos(GetTime() * 4) * 4, position.y + (float)sin(GetTime() * 7) * 7};

    BeginTextureMode(backgroundPaintTarget);
    {
        BeginBlendMode(BLEND_SUBTRACT_COLORS);
        {
            DrawCircleV(position, sprayCan.radius, sprayCan.color);
        }
        EndBlendMode();
    }
    EndTextureMode();
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

void DrawUiDeathRing()
{
    Vector2 center = playerRef->position;

    DrawRing(center, (float)(targetFps) * ((float)targetFps / 12.f), 240, 0, 360, 32, BLACK);
}

void DrawUiScore()
{
    int scoreOffset = 5;
    int bonusScoreOffset = scoreOffset + scoreTextWidth + 1;
    int comboScoreOffset = bonusScoreOffset + bonusScorePoolTextWidth + 16;
    int comboMultiplierOffset = comboScoreOffset + comboTextWidth + 1;

    int yOffset = VIRTUAL_SCREEN_HEIGHT + 3;
    int defaultSize = 8;

    DrawText(scoreText, scoreOffset, yOffset, defaultSize, uiColorYellow);

    LIST_MAP_DATA(particleUiHead, ParticleDraw, *(Particle *));

    if (ScoreHandlerGetBonusScorePool() > 0)
    {
        DrawText(bonusScorePoolText, bonusScoreOffset, yOffset, defaultSize, guideColor);
    }

    if (ScoreHandlerGetComboScore() > 0)
    {
        DrawText(comboText, comboScoreOffset, yOffset, defaultSize, uiColorRed);
        Vector2 particlePos = {(float)comboScoreOffset + (float)comboTextWidth / 2, (float)yOffset+8};
        ParticleCreate(&particleUiHead, ParticlePresetAura(particlePos, uiColorRed));
    }

    if (ScoreHandlerGetComboMultiplier() > 1)
    {
        double pulse = sin(GetTime() * 10) * 50;
        Color pulsatingColor = {(int)fmin(orangeColor.r + pulse, 255), (int)fmin(orangeColor.g + pulse, 255), orangeColor.b, 255};
        DrawText(comboMultiplierText, comboMultiplierOffset, yOffset, defaultSize, pulsatingColor);
        Vector2 particlePos = {(float)comboMultiplierOffset + (float)comboMultiplierTextWidth / 2, (float)yOffset+8};
        ParticleCreate(&particleUiHead, ParticlePresetAura(particlePos, orangeColor));
    }
}

void DrawUiProgressBar()
{
    const Vector2 progLineStart = (Vector2){spawningProgressBar.start, VIRTUAL_SCREEN_HEIGHT+1};
    const Vector2 progLineEnd = (Vector2){spawningProgressBar.end, VIRTUAL_SCREEN_HEIGHT+1};
    DrawLineV(progLineStart, progLineEnd, uiColorDarkGray);
}

void DrawUiGameOver()
{
    if (RendererIsRingTransitionActive()) return;

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        DrawText(gameOverText, (int)virtualScreenCenter.x - gameOverTextWidth / 2, (int)virtualScreenCenter.y-8, 8, WHITE);

        scoreTextWidth = MeasureText(scoreText, 8);
        DrawText(scoreText, (int)virtualScreenCenter.x - scoreTextWidth / 2, (int)virtualScreenCenter.y+12, 8, (ScoreHandlerGetScore() == ScoreHandlerGetHiScore()) ? uiColorYellow : WHITE);

        if (tutorialStateIndex >= TUTORIAL_LENGTH)
        {
            DrawText(hiScoreText, (int)virtualScreenCenter.x - hiScoreTextWidth / 2, (int)virtualScreenCenter.y+24, 8, uiColorYellow);
        }

        ScoreHandlerDrawMedals();
    }

    if (frameCounter / 15 % 2 == 0) DrawText(restartText, (int)virtualScreenCenter.x - restartTextWidth / 2, (int)virtualScreenCenter.y+66, 8, WHITE);

    if ((backButton->pressed || !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) && IsPointInsideCircularButton(*backButton, mousePos))
    {
        CircularButtonDraw(*backButton);
    }
}

void DrawUiRingTransition()
{
    double progress = (GetTime() - ringTrans.timeSinceLastTransition) / ringTrans.duration;

    if (progress > 1.f) return;

    ringTrans.endAngle = fminf((float)progress * 2.f * 360.f, 360.f);

    DrawRing(virtualScreenCenter, 0, 240, ringTrans.startAngle, ringTrans.endAngle, 32, BLACK);

    if (ringTrans.endAngle == 360.f)
    {
        if (ringTrans.startAngle == 0.f) ringTrans.transitionFunction();

        ringTrans.startAngle = fmaxf(((float)progress-.5f) * 2.f * 360.f, 0.f);
    }
}

bool RendererIsRingTransitionActive()
{
    return GetTime() <= ringTrans.timeSinceLastTransition + ringTrans.duration;
}

void RendererPlayRingTransition(void (*transitionFunction)())
{
    ringTrans.timeSinceLastTransition = GetTime();
    ringTrans.startAngle = 0;
    ringTrans.endAngle = 0;
    ringTrans.transitionFunction = transitionFunction;

    PlaySound(gameAudio.dash);
}

void DrawUiBallClearerTarget()
{
    if (BallClearerTarget() == NULL) return;

    Ball *targetBall = BallClearerTarget();
    Vector2 center = targetBall->position;
    float radius = targetBall->radius;
    float rotation = (float)GetTime() * 1.5f;
    float rAng = PI / 2;

    Vector2 dirs[4] = {
            {cosf(rotation), sinf(rotation)},
            {cosf(rotation + rAng), sinf(rotation + rAng)},
            {cosf(rotation + PI), sinf(rotation + PI)},
            {cosf(rotation + PI + rAng), sinf(rotation + PI + rAng)}
    };

    Color triColor = orangeColor;
    float triLength = 16;
    float triWidth = 8;

    for (int i = 0; i < 4; i++) {
        Vector2 dir = dirs[i];
        Vector2 dirNext = dirs[(i + 1) % 4];

        Vector2 outerVertPivot = Vector2Add(center, (Vector2){dir.x * (radius + triLength), dir.y * (radius + triLength)});
        Vector2 outerVertCorner = {dirNext.x * triWidth, dirNext.y * triWidth};
        DrawTriangleLines(
                Vector2Add(center, (Vector2){dir.x * radius, dir.y * radius}),
                Vector2Add(outerVertPivot, outerVertCorner),
                Vector2Subtract(outerVertPivot, outerVertCorner),
                triColor);
    }
}

void DrawUiTitleButtons()
{
    CircularButtonDraw(*startButton);
    CircularButtonDraw(*settingsButton);
}

void DrawUiSettingsButtons()
{
    CircularButtonDraw(*backButton);
#ifdef PLATFORM_DESKTOP
    CircularButtonDraw(*windowModeButton);
    DrawText(GetWindowModeAsString(), (int)windowModeButton->position.x + 24, (int)windowModeButton->position.y, 8, WHITE);
#endif
    CircularButtonDraw(*muteBgmButton);
    DrawText((muted) ? "Off" : "On", (int)muteBgmButton->position.x + 24, (int)muteBgmButton->position.y, 8, WHITE);
    CircularButtonDraw(*replayTutButton);
    DrawText("Replay Tutorial", (int)replayTutButton->position.x + 24, (int)replayTutButton->position.y, 8, WHITE);
}

void DrawUiBorders()
{
    DrawRectangle(0, VIRTUAL_SCREEN_HEIGHT, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_OFFSET_Y, BLACK);
    DrawTexture(gameTextures.tvBorder, 0, 0, WHITE);
}

void DrawUi()
{
    DrawUiBorders();

    mousePos = Vector2ClampInsideScreen(Vector2Round(Vector2ToVirtualCoords(GetMousePosition())), 2);

    switch(gameState)
    {
        case GAME_TITLE:
        {
            if (ScoreHandlerGetHiScore() >= M_BRONZE) ScoreHandlerDrawMedals();

            double speed = GetTime() * 2;
            titleScreenOffset = (Vector2) {(float) cos(speed) * 8, (float) sin(speed * 2) * 4};
            DrawTextureV(gameTextures.titleText, titleScreenOffset, WHITE);

            DrawUiTitleButtons();

            DrawTextureV(gameTextures.infoQuit, Vector2Zero(), WHITE);

            if (ScoreHandlerGetHiScore() > 0) DrawText(hiScoreText, (int)virtualScreenCenter.x - hiScoreTextWidth / 2, VIRTUAL_SCREEN_HEIGHT - 52, 8, uiColorYellow);
            break;
        }

        case GAME_SETTINGS:
        {
            DrawUiSettingsButtons();
            break;
        }

        case GAME_PLAY:
        {
            if (ballNbrCount_All.spawned == 0 && frameCounter / 15 % 2 == 0 && ListLength(&ballSpawnPointHead) == 0)
            {
                DrawText(getReadyText, (int) virtualScreenCenter.x- getReadyTextWidth / 2 + 1,
                         (int) virtualScreenCenter.y - 32 + 1, 8, GRAY);
                DrawText(getReadyText, (int) virtualScreenCenter.x - getReadyTextWidth / 2,
                         (int) virtualScreenCenter.y - 32, 8, WHITE);
            }

            DrawUiScore();
            DrawUiProgressBar();

            if (!IsBallClearingFinished()) DrawUiBallClearerTarget();
            break;
        }

        case GAME_OVER:
        {
            if (RendererIsRingTransitionActive()) break;

            if (targetFps != initFps)
            {
                DrawUiDeathRing();
            }
            else
            {
                DrawUiGameOver();
                DrawTextureV(gameTextures.infoQuit, Vector2Zero(), WHITE);
            }

            DrawUiScore();
            DrawUiProgressBar();

            DrawCircleLinesV(mousePos, eraserRadius, guideColor);

            if (targetFps == initFps && !backButton->pressed)
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    EraseBackgroundPaint(mousePos, eraserRadius);
                }
                else if (GetTime() < sprayCanFoam.timeSinceLastSpray + sprayCanFoam.sprayDuration)
                {
                    SprayBackgroundPaint(mousePos, sprayCanFoam);
                }

                if (IsKeyPressed(KEY_S)) sprayCanFoam.timeSinceLastSpray = GetTime();
            }

            break;
        }

        case GAME_TUTORIAL:
        {
            TutorialDraw();
            if (!IsBallClearingFinished()) DrawUiBallClearerTarget();
            if (tutorialStateIndex >= TUTORIAL_SLICING) DrawUiScore();
            if (tutorialStateIndex == TUTORIAL_COMBOS_2) DrawUiProgressBar();
            break;
        }
    }

    DrawUiRingTransition();

    DrawCircleV(mousePos, 2, guideColor);
}


float RendererGetPaintPercentage()
{
    Image image = LoadImageFromTexture(backgroundPaintTarget.texture);

    Color *pixels = LoadImageColors(image);

    int totalPixels = image.width * image.height;

    int blankPixelCount = 0;
    for (int i = 0; i < totalPixels; i++)
    {
        if (pixels[i].a == 0) blankPixelCount++;
    }

    UnloadImageColors(pixels);
    UnloadImage(image);

    float blankPixelPercentage = (float)(totalPixels - blankPixelCount) / (float)totalPixels * 100.0f;
    return blankPixelPercentage;
}

void RenderToTarget()
{
    DrawParticlesToBackgroundPaint();
    CameraShakeUpdate();

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
        DrawUi();
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

    char ballsSpawnedText[32];
    sprintf(ballsSpawnedText, "BALLS SPAWNED %d", ballNbrCount_All.spawned);
    DrawText(ballsSpawnedText, 10, 70, 21, DARKGREEN);
}

void RenderSplashScreenToTarget()
{
    if (IsWindowResized()) RendererFitVirtualRectToScreen();

    BeginTextureMode(virtualRenderTarget);
    {
        DrawTexture(gameTextures.bgFloorStandard, 0, 0, GRAY);
        FirefliesDraw();
        DrawUiBorders();

        switch (splashMessageIndex)
        {
            case SM_THIS_GAME_USES_MOUSE:
            {
                double speed = GetTime() * 2;
                Vector2 spriteOffset = {virtualScreenCenter.x + (float)cos(speed) * 8,
                                        virtualScreenCenter.y + (float)sin(speed * 2) * 4 - 16};
                SpriteDraw(*splashMouseIcon, spriteOffset, Vector2One(), 0);
                SpriteAnimate(splashMouseIcon, .02f, true);

                char *text = "This game only uses the mouse.";
                int textWidth = MeasureText(text, 8);
                Vector2 textPos = {virtualScreenCenter.x - (float)textWidth/2, virtualScreenCenter.y + 32};
                DrawText(text, (int)textPos.x+1, (int)textPos.y+1, 8, GRAY);
                DrawText(text, (int)textPos.x, (int)textPos.y, 8, WHITE);
                break;
            }

            case SM_STUDIO_NAME:
            {
                char *text = "A game by Lökallan";
                int textWidth = MeasureText(text, 8);
                Vector2 textPos = {virtualScreenCenter.x - (float)textWidth/2, virtualScreenCenter.y};
                DrawText(text, (int)textPos.x+1, (int)textPos.y+1, 8, GRAY);
                DrawText(text, (int)textPos.x, (int)textPos.y, 8, WHITE);
                break;
            }

            default:
                return;
        }
    }
    EndTextureMode();
}

void RenderToScreen()
{
    if (IsWindowResized()) RendererFitVirtualRectToScreen();

    BeginDrawing();
    {
        ClearBackground(BLACK);

        // Draw world camera to screen
        DrawTexturePro(virtualRenderTarget.texture, virtualRenderRect.source, virtualRenderRect.dest, virtualRenderRect.origin, 0.0f, WHITE);

        if (debugDrawing) DrawDebug();
    }
    EndDrawing();
}