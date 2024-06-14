#include <math.h>
#include <stdio.h>
#include "score_handler.h"
#include "raylib.h"
#include "game.h"
#include "asset_loader.h"

int score = 0;
int hiScore = 0;
int comboScore = 0;
float comboMultiplier = 1.f;
// A lost combo gets added to here then it slowly adds to the main score
int bonusScorePool = 0;

char scoreText[128] = "Score: 0";
char comboText[128] = "+";
char hiScoreText[128] = "High Score: 0";
char bonusScorePoolText[128] = "<-";

char gameOverText[32] = "- Game Over -";
char restartText[32] = "Clean up to restart!";

int scoreTextWidth;
int comboTextWidth;
int hiScoreTextWidth;
int bonusScorePoolTextWidth;
const int gameOverTextWidth = 71;
const int restartTextWidth = 104;


void UpdateText()
{
    sprintf(scoreText, "Score: %d", score);
    sprintf(comboText, "+%d", comboScore);
    if (comboMultiplier > 1) sprintf(comboText, "%s x%d", comboText, (int)comboMultiplier);
    sprintf(hiScoreText, "High Score: %d", hiScore);
    sprintf(bonusScorePoolText, "<-%d", bonusScorePool);

    scoreTextWidth = MeasureText(scoreText, 8);
    comboTextWidth = MeasureText(comboText, 8);
    hiScoreTextWidth = MeasureText(hiScoreText, 8);
    bonusScorePoolTextWidth = MeasureText(bonusScorePoolText, 8);

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
    if (comboScore > 1) bonusScorePool += (int)((float)comboScore * comboMultiplier);
    comboScore = 0;
    comboMultiplier = 1.f;

    UpdateText();
}

void ScoreHandlerAddToScoreFromBonusPool()
{
    if (bonusScorePool <= 0) return;

    int updateSpeed = (bonusScorePool < 50) ? 2 : 1;
    if (frameCounter % updateSpeed != 0) return;

    bonusScorePool--;
    score++;
    hiScore = (int)fmax(score, hiScore);

    PlaySound(gameAudio.bonus_score);

    UpdateText();
}

int ScoreHandlerGetComboScore()
{
    return comboScore;
}

int ScoreHandlerGetBonusScorePool()
{
    return bonusScorePool;
}