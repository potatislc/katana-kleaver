#ifndef BALL_GAME_SCORE_HANDLER_H
#define BALL_GAME_SCORE_HANDLER_H

#define COMBO_BUFFER_SIZE (4)

extern char scoreText[128];
extern char comboText[128];
extern char comboMultiplierText[128];
extern char hiScoreText[128];
extern char bonusScorePoolText[128];
extern char gameOverText[32];
extern char restartText[32];

extern int scoreTextWidth;
extern int comboTextWidth;
extern int comboMultiplierTextWidth;
extern int hiScoreTextWidth;
extern int bonusScorePoolTextWidth;
extern const int gameOverTextWidth;
extern const int restartTextWidth;

void ScoreHandlerAddToScore(int val);
void ScoreHandlerAddToMultiplier(int val);
void ScoreHandlerLoseCombo();
void ScoreHandlerResetScore();
int ScoreHandlerGetComboScore();
int ScoreHandlerGetComboMultiplier();
int ScoreHandlerGetComboScoreBuffer();
int ScoreHandlerGetBonusScorePool();
void ScoreHandlerAddToScoreFromBonusPool();

#endif //BALL_GAME_SCORE_HANDLER_H
