#include "tutorial.h"
#include "game.h"
#include "renderer.h"

int tutorialStateIndex = 0;
void (*tutorialState)();
double stateEndTime = 0;
char *tutorialText = "Text";
int tutorialTextWidth;

const double statesTimeDuration[TUTORIAL_LENGTH] =
        {
            1,
            1,
        };
bool statesComplete[TUTORIAL_LENGTH] = {false};

void SetTutorialText(char* text)
{
    tutorialText = text;
    tutorialTextWidth = MeasureText(tutorialText, 8);
}

void StateMoving()
{
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) statesComplete[TUTORIAL_MOVING] = true;
}

void StateDashing()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) statesComplete[TUTORIAL_DASHING] = true;
}

void StateSlashing()
{

}

void StateCombo()
{

}

void SetState(int index)
{
    tutorialStateIndex = index;

    switch (tutorialStateIndex)
    {
        case TUTORIAL_MOVING:
        {
            SetTutorialText("RMB to move.");
            tutorialState = StateMoving;
            break;
        }

        case TUTORIAL_DASHING:
        {
            SetTutorialText("LMB to dash.");
            tutorialState = StateDashing;
            break;
        }

        case TUTORIAL_SLASHING:
        {
            SetTutorialText("Dash though a fruit to slice it.");
            tutorialState = StateSlashing;
            break;
        }

        case TUTORIAL_COMBOS:
        {
            SetTutorialText("Do 5 slashes without missing to gain combo!");
            break;
        }

        case TUTORIAL_ORANGES:
        {
            SetTutorialText("Oranges multiply your combo!");
            break;
        }

        case TUTORIAL_END:
        {
            SetTutorialText("Good luck.");
            break;
        }

        default:
        {
            SetTutorialText("");
            GameStart();
            return;
        }
    }
}

void TutorialBegin()
{
    if (gameState == GAME_TUTORIAL) return;

    gameState = GAME_TUTORIAL;
    stateEndTime = GetTime();
    SetState(tutorialStateIndex);
}

void TutorialUpdate()
{
    tutorialState();

    if (!statesComplete[tutorialStateIndex]) stateEndTime = GetTime() + statesTimeDuration[tutorialStateIndex];
    if (GetTime() > stateEndTime + statesTimeDuration[tutorialStateIndex])
    {
        SetState(tutorialStateIndex+1);
    }
}

void TutorialDraw()
{
    Color textColor = (statesComplete[tutorialStateIndex]) ? GREEN : WHITE;
    DrawText(tutorialText, (int)virtualScreenCenter.x - tutorialTextWidth / 2, (int)virtualScreenCenter.y - 8, 8, textColor);
}

void TutorialEnd()
{

}
