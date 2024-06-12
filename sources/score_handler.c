#include <math.h>
#include <stdio.h>
#include "score_handler.h"
#include "raylib.h"

int score = 0;
int hiScore = 0;
int comboScore = 0;
float comboMultiplier = 1.f;

char scoreText[32] = "Score: 0";
char comboText[128];
char hiScoreText[32] = "High Score: 0";

char gameOverText[32] = "- Game Over -";
char restartText[32] = "'Middle Mouse' to restart";

int scoreTextWidth;
int comboTextWidth;
int hiScoreTextWidth;
const int gameOverTextWidth = 71;
const int restartTextWidth = 131;


void UpdateText()
{
    sprintf(scoreText, "Score: %d", score);
    sprintf(comboText, "+%d", comboScore);
    // sprintf(comboText, "%s *%9.1f", comboText, comboMultiplier);
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
    score += val;
    comboScore += val;
    // comboMultiplier += (float)val / 10.f;
    hiScore = (int)fmax(score, hiScore);

    UpdateText();
}

void ScoreHandlerLoseCombo()
{
    if (comboScore > 1) ScoreHandlerAddToScore((int)((float)comboScore * comboMultiplier));
    comboScore = 0;
    comboMultiplier = 1.f;

    UpdateText();
}

int ScoreHandlerGetComboScore()
{
    return comboScore;
}