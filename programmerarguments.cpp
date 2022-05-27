#include "programmerarguments.h"


bool ProgrammerOptions::optionChecked(ProgrammerOptionsEnum option)
{
    return (bool)((int)options & (int)option);
}

void ProgrammerOptions::checkOption(ProgrammerOptionsEnum option)
{
    this->options = (ProgrammerOptionsEnum)((int)options | (int)option);
}
