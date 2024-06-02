#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "ball.h"
#include "player.h"
#include "global.h"
#include "linked_list.h"
#include "asset_loader.h"

#define WINDOW_TITLE "Ball Game"

static void ToggleFullscreenWindow()
{
    if (IsWindowFullscreen())
    {
        ToggleFullscreen();
        SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    }
    else
    {
        int display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
    }
}

Rectangle VirtualRect()
{
    int windowHeight = GetScreenHeight();
    float aspectRatio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;
    int newWidth = (int)((float)windowHeight * aspectRatio); // Calculate the new width based on the aspect ratio

    screenOffset = (Vector2){(float)(GetScreenWidth() - newWidth) / 2.f, 0.f};

    screenRatio = (Vector2){
            ((float)newWidth) / (float)VIRTUAL_SCREEN_WIDTH,
            (float)GetScreenHeight() / (float)(VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y)};

    return (Rectangle){
            screenOffset.x,
            screenOffset.y,
            (float)newWidth,
            (float)GetScreenHeight()};
}

int main(void)
{
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE);

    Vector2 vScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

    RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);

    Camera2D worldSpaceCamera = { 0 };
    worldSpaceCamera.zoom = 1.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };

    Vector2 origin = { 0.0f, 0.0f };

    LoadGameTextures();

    // Set Seed
    srand(time(0));

    SetTargetFPS(60);

    ToggleFullscreenWindow();
    Rectangle destRec = VirtualRect();

    int ballsSpawned = 0;
    char ballsSpawnedText[16];
    sprintf(ballsSpawnedText, "%d", ballsSpawned);

    double spawnDelay = 3.0;
    double timeSinceLastSpawn = GetTime();

    Player *player = PlayerInit(vScreenCenter, &ballHead);
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------

        if (IsKeyPressed(KEY_R))
        {
            Player *newPlayer = PlayerReset(player, vScreenCenter, &ballHead);
            player = newPlayer;

            ListNodeRemoveAll(&ballHead);
            ListNodeRemoveAll(&ballSpawnPointHead);

            score = 0;
            comboScore = 0;

            gameOver = false;
        }

        if (GetTime() > timeSinceLastSpawn+spawnDelay)
        {
            timeSinceLastSpawn = GetTime();

            float testRadius = 32.0f;
            Ball *newBall = BallInit(
                     (Vector2) {testRadius, testRadius},
                     (Vector2) {VIRTUAL_SCREEN_WIDTH - testRadius, VIRTUAL_SCREEN_HEIGHT - testRadius},
                     testRadius);

            BallSpawnPoint *newBallSpawn = BallSpawnPointInit(newBall, ballSpawnTime);
            ListNodePush(&ballSpawnPointHead, newBallSpawn);

            // Only for debug
            ballsSpawned++;
            sprintf(ballsSpawnedText, "%d", ballsSpawned);
        }

        if (!freezeBalls)
        {
            // Updating linked list of balls
            ListNode* currentBallNode = ballHead;
            while (currentBallNode != NULL)
            {
                BallUpdate(currentBallNode->data);
                currentBallNode = currentBallNode->next;
            }

            ListNode* currentBallSpawnPointNode = ballSpawnPointHead;
            while (currentBallSpawnPointNode != NULL)
            {
                BallSpawnPointUpdate(currentBallSpawnPointNode->data);
                currentBallSpawnPointNode = currentBallSpawnPointNode->next;
            }
        }

        PlayerUpdate(player);
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginTextureMode(target);

            // Draw Game World
            BeginMode2D(worldSpaceCamera);
                // Draw floor
                DrawTexture(gameTextures.floorStandard, 0, 0, WHITE);

                // Draw Shadows
                PlayerDrawShadow(*player);

                ListNode* currentBallNode = ballHead;
                while (currentBallNode != NULL)
                {
                    BallDrawShadow(*(Ball*)currentBallNode->data);
                    currentBallNode = currentBallNode->next;
                }

                // Draw characters
                currentBallNode = ballHead;
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

                PlayerDraw(*player);

                // Draw UI
                DrawRectangle(0, VIRTUAL_SCREEN_HEIGHT, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_OFFSET_Y, BLACK);
                char scoreText[32];
                sprintf(scoreText, "Score: %d", score);
                DrawText(scoreText, 5, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorYellow);

                DrawCircleV(Vector2Round(Vector2ToVirtualCoords(GetMousePosition())), 2, guideColor);

                if (comboScore > 1)
                {
                    char comboText[16];
                    sprintf(comboText, "x%d", comboScore);
                    DrawText(comboText, 69, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorRed);
                }

                if (gameOver)
                {
                    char gameOverText[16] = "- Game Over -";
                    int textWidth = MeasureText(gameOverText, 8);
                    DrawText(gameOverText, (int)vScreenCenter.x-textWidth/2, (int)vScreenCenter.y, 8, WHITE);
                }
            EndMode2D();
        EndTextureMode();

        // Draw to Screen
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw world camera to screen
            DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

            // Debug stuff
            DrawFPS(10, 10);
            DrawText(ballsSpawnedText, 10, 30, 21, GREEN);
        EndDrawing();
        //-----------------------------------------------------

        // Toggle Fullscreen
        //-----------------------------------------------------
        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            ToggleFullscreenWindow();
            destRec = VirtualRect();
        }
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
