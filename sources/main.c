#include "raylib.h"
#include "ball.h"

#define SCREEN_WIDTH (1024)
#define SCREEN_HEIGHT (1024)

#define WINDOW_TITLE "Ball Game"

#define NBR_OF_BALLS (6)

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    struct Ball balls[NBR_OF_BALLS];

    for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
    {
        balls[i].speed = (Vector2){ 2.0f, 2.0f };
        balls[i].radius = 64.0f;
        balls[i].position = (Vector2){ balls[i].radius + balls[i].radius * (float)i * 2.0f, balls[i].radius + balls[i].radius * (float)i * 2.0f };
        balls[i].colliding = false;
        balls[i].collisionBox = (Rectangle){ 0, 0, balls[i].radius*2, balls[i].radius*2 };
    }

    Vector2 shadowOffset = { 4.0f, 4.0f};
    float shadowRadius = 1;

    bool pause = 0;
    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
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
