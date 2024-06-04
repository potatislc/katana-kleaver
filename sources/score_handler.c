#include <math.h>
#include "score_handler.h"

int score = 0;
int hiScore = 0;
int comboScore = 0;

void AddToScore(int val)
{
    score += val;
    comboScore += val;
    hiScore = (int)fmax(score, hiScore);
}

void LoseCombo()
{
    score += comboScore;
    comboScore = 0;
}