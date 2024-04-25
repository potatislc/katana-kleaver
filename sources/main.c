#include "raylib.h"
#include "ball.h"
#include "player.h"

#define SCREEN_WIDTH (1024)
#define SCREEN_HEIGHT (1024)

#define WINDOW_TITLE "Ball Game"

#define NBR_OF_BALLS (6)

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    struct Player player;
    initPlayer(&player, screenCenter);

    struct Ball balls[NBR_OF_BALLS];

    for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
    {
        float testRadius = 64.0f;
        initBall(&balls[i],(Vector2){ testRadius + testRadius * (float)i * 2.0f, testRadius + testRadius * (float)i * 2.0f },testRadius);
    }

    Vector2 shadowOffset = { 4.0f, 4.0f};
    float shadowRadius = 1;

    bool pause = 0;
    int framesCounter = 0;

    SetTargetFPS(60);
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause)
        {
            for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
            {
                ballCollisionBall(&balls[i], balls, NBR_OF_BALLS);
                moveBall(&balls[i]);
                screenCollisionBall(&balls[i]);
            }

            updatePlayer(&player);
        }
        else framesCounter++;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGRAY);

        for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
        {
            drawBall(balls[i], shadowOffset, shadowRadius);
        }

        drawPlayer(player);
        //DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25, 20, LIGHTGRAY);

        // On pause, we draw a blinking message
        if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", 350, 200, 30, GRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
