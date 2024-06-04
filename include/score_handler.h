#ifndef BALL_GAME_SCORE_HANDLER_H
#define BALL_GAME_SCORE_HANDLER_H

extern char scoreText[32];
extern char comboText[32];
extern char hiScoreText[32];
extern char gameOverText[32];
extern char restartText[32];

extern int scoreTextWidth;
extern int comboTextWidth;
extern int hiScoreTextWidth;
extern const int gameOverTextWidth;
extern const int restartTextWidth;

void ScoreHandlerAddToScore(int val);
void ScoreHandlerLoseCombo();
void ScoreHandlerResetScore();
int ScoreHandlerGetComboScore();

#endif //BALL_GAME_SCORE_HANDLER_H
