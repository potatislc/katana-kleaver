#include <math.h>
#include <stdio.h>
#include "score_handler.h"
#include "raylib.h"
#include "game.h"

int score = 0;
int hiScore = 0;
int comboScore = 0;
float comboMultiplier = 1.f;

char scoreText[32] = "Score: 0";
char comboText[128];
char hiScoreText[32] = "High Score: 0";

char gameOverText[32] = "- Game Over -";
char restartText[32] = "Clean up to restart!";

int scoreTextWidth;
int comboTextWidth;
int hiScoreTextWidth;
const int gameOverTextWidth = 71;
const int restartTextWidth = 104;


void UpdateText()
{
    sprintf(scoreText, "Score: %d", score);
    sprintf(comboText, "+%d", comboScore);
    if (comboMultiplier > 1) sprintf(comboText, "%s x%d", comboText, (int)comboMultiplier);
    sprintf(hiScoreText, "High Score: %d", hiScore);

    scoreTextWidth = MeasureText(scoreText, 8);
    comboTextWidth = MeasureText(comboText, 8);
    hiScoreTextWidth = MeasureText(hiScoreText, 8);

    // For if I ever want to re-measure the constant
    // printf("%d\n", MeasureText(gameOverText, 8));
    // printf("%d\n", MeasureText(restartText, 8));
}

void ScoreHandlerResetScore()
{
    score = 0;

    UpdateText();
}

void ScoreHandlerAddToScore(int val)
{
    if (gameState != GAME_PLAY) return;

    score += val;
    comboScore += val;
    hiScore = (int)fmax(score, hiScore);

    UpdateText();
}

void ScoreHandlerAddToMultiplier(float val)
{
    comboMultiplier += val;
}

void ScoreHandlerLoseCombo()
{
    if (comboScore > 1) score += (int)((float)comboScore * comboMultiplier);
    hiScore = (int)fmax(score, hiScore);
    comboScore = 0;
    comboMultiplier = 1.f;

    UpdateText();
}

int ScoreHandlerGetComboScore()
{
    return comboScore;
}