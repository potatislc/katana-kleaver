#ifndef KATANA_KLEAVER_STORAGE_H
#define KATANA_KLEAVER_STORAGE_H

#include "raylib.h"

#include <stdlib.h>

#define STORAGE_DATA_FILE   "save.data"

typedef enum {
    STORAGE_POSITION_HISCORE,
    STORAGE_POSITION_TUTORIAL_STATE_INDEX,
    STORAGE_POSITION_LENGTH
} StorageData;

bool SaveStorageValue(unsigned int position, int value);
int LoadStorageValue(unsigned int position);
bool ResetAllStorageValues();


#endif //KATANA_KLEAVER_STORAGE_H
