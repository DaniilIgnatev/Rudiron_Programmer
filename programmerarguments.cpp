#include "programmerarguments.h"


bool ProgrammerOptions::checked(ProgrammerOptionsEnum option)
{
    return (bool)((int)value & (int)option);
}

void ProgrammerOptions::check(ProgrammerOptionsEnum option)
{
    this->value = (ProgrammerOptionsEnum)((int)value | (int)option);
}
