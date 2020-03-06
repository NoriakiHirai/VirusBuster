#include "CommonData.h"

int CommonData::currentStage = 1;

void CommonData::SetCurrentStage(int num)
{
    currentStage = num;
}

const int CommonData::GetCurrentStage()
{
    return currentStage;
}
