#ifndef BALL_GAME_SCORE_HANDLER_H
#define BALL_GAME_SCORE_HANDLER_H

extern int score;
extern int hiScore;
extern int comboScore;

void AddToScore(int val);
void LoseCombo();

#endif //BALL_GAME_SCORE_HANDLER_H
