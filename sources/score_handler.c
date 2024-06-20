#include <math.h>
#include <stdio.h>
#include "score_handler.h"
#include "raylib.h"
#include "game.h"
#include "asset_loader.h"

int score = 0;
int hiScore = 0;
int comboScore = 0;
int comboScoreBuffer = 0;
int comboMultiplier = 1;
// A lost combo gets added to here then it slowly adds to the main score
int bonusScorePool = 0;

char scoreText[128] = "Score: 0";
char comboText[128] = "+";
char comboMultiplierText[128] = "x";
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
    //if (comboMultiplier > 1) sprintf(comboText, "%s x%d", comboText, comboMultiplier);
    sprintf(comboMultiplierText, "x%d", comboMultiplier);
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
    comboScore = 0;
    bonusScorePool = 0;
    comboScoreBuffer = 0;
    comboMultiplier = 1;

    UpdateText();
}

void ScoreHandlerAddToScore(int val)
{
    if (!IS_GAME_STATE_PLAYABLE) return;

    score += val;
    comboScoreBuffer += val;
    comboScore = (comboScoreBuffer > COMBO_BUFFER_SIZE) ? comboScoreBuffer - COMBO_BUFFER_SIZE : 0;
    hiScore = (int)fmax(score, hiScore);

    UpdateText();
}

void ScoreHandlerAddToMultiplier(int val)
{
    if (!IS_GAME_STATE_PLAYABLE) return;

    comboMultiplier += val;

    UpdateText();
}

void ScoreHandlerLoseCombo()
{
    if (comboScore > 0)
    {
        bonusScorePool += comboScore * comboMultiplier;
        // PlaySound(gameAudio.loseCombo); It sucked :/
    }

    comboScoreBuffer = 0;
    comboScore = 0;
    comboMultiplier = 1;

    UpdateText();
}

void ScoreHandlerAddToScoreFromBonusPool(bool instant)
{
    if (bonusScorePool <= 0) return;

    int updateSpeed = (bonusScorePool < 50) ? 2 : 1;
    if (frameCounter % updateSpeed != 0) return;

    if (instant)
    {
        score += bonusScorePool;
        bonusScorePool = 0;
    }
    else
    {
        bonusScorePool--;
        score++;
        PlaySound(gameAudio.bonusScore);
    }

    hiScore = (int)fmax(score, hiScore);

    UpdateText();
}

int ScoreHandlerGetComboScore()
{
    return comboScore;
}

int ScoreHandlerGetComboScoreBuffer()
{
    return comboScoreBuffer;
}

int ScoreHandlerGetComboMultiplier()
{
    return (int)comboMultiplier;
}

int ScoreHandlerGetBonusScorePool()
{
    return bonusScorePool;
}

int ScoreHandlerGetHiScore()
{
    return hiScore;
}

int ScoreHandlerSetHiScore(int newHiScore)
{
    hiScore = newHiScore;
    UpdateText();
}