#ifndef BALL_GAME_SCORE_HANDLER_H
#define BALL_GAME_SCORE_HANDLER_H

#include <stdbool.h>
#include <stdint.h>
#include "raylib.h"

#define COMBO_BUFFER_SIZE (4)
#define MIN_MULTIPLIER (1)
#define STR_MAX UINT8_MAX

extern char scoreText[STR_MAX];
extern char comboText[STR_MAX];
extern char comboMultiplierText[STR_MAX];
extern char hiScoreText[STR_MAX];
extern char bonusScorePoolText[STR_MAX];
extern const char gameOverText[];
extern const char restartText[];

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
void ScoreHandlerUpdateHiScore();
int ScoreHandlerGetScore();
int ScoreHandlerGetComboScore();
int ScoreHandlerGetComboMultiplier();
int ScoreHandlerGetComboScoreBuffer();
int ScoreHandlerGetBonusScorePool();
int ScoreHandlerGetHiScore();
void ScoreHandlerSetHiScore(int newHiScore);
void ScoreHandlerAddToScoreFromBonusPool(bool instant);

//
//  MEDAL
//

#define MEDAL_COUNT (4)

enum MedalScores
{
    M_BRONZE = 100,
    M_SILVER = 400,
    M_GOLD = 1000,
    M_PLATINUM = 2000
};

extern const int medalScores[MEDAL_COUNT];

void ScoreHandlerUnlockMedals(int score);
void ScoreHandlerRevealMedals();
void ScoreHandlerDrawMedals();
bool ScoreHandlerMedalRevealFinished();

#endif //BALL_GAME_SCORE_HANDLER_H
