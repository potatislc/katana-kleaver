#ifndef KATANA_KLEAVER_TUTORIAL_H
#define KATANA_KLEAVER_TUTORIAL_H

#include "raylib.h"

typedef enum
{
    TUTORIAL_MOVING,
    TUTORIAL_DASHING,
    TUTORIAL_SLICING,
    TUTORIAL_COMBOS_2,
    TUTORIAL_COMBOS,
    TUTORIAL_ORANGES,
    TUTORIAL_ORANGES_2,
    TUTORIAL_END,
    TUTORIAL_LENGTH
} TutorialStates;

void TutorialBegin();
void TutorialUpdate();
void TutorialDraw();
void TutorialSetState(TutorialStates index);
void TutorialReset();

extern int tutorialStateIndex;
extern void (*tutorialState)();
extern double stateEndTime;
extern char *tutorialText;
extern int tutorialTextWidth;

#endif //KATANA_KLEAVER_TUTORIAL_H