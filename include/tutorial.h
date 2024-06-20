#ifndef KATANA_KLEAVER_TUTORIAL_H
#define KATANA_KLEAVER_TUTORIAL_H

#include <stdbool.h>

typedef enum
{
    TUTORIAL_MOVING,
    TUTORIAL_DASHING,
    TUTORIAL_SLASHING,
    TUTORIAL_COMBOS,
    TUTORIAL_ORANGES,
    TUTORIAL_END,
    TUTORIAL_LENGTH
} TutorialStates;

void TutorialBegin();
void TutorialUpdate();
void TutorialDraw();

extern int tutorialStateIndex;
extern void (*tutorialState)();
extern double stateEndTime;
extern char *tutorialText;
extern int tutorialTextWidth;

#endif //KATANA_KLEAVER_TUTORIAL_H