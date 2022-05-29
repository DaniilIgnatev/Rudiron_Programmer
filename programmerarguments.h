#ifndef PROGRAMMERARGUMENTS_H
#define PROGRAMMERARGUMENTS_H

#include "TypeDefs.hpp"


enum ProgrammerOptionsEnum{
    None = 0,
    VerifyBootloader = 1,
    Erase = 2,
    Load = 4,
    VerifyProgram = 8,
    Run = 16
};


struct ProgrammerOptions{
private:
    ProgrammerOptionsEnum value = ::None;

public:
    bool checked(ProgrammerOptionsEnum option);
    void check(ProgrammerOptionsEnum option);
};


struct ProgrammerArguments{
    QString bootloaderPath;

    QString programPath;

    int speedMultiplier = 1;

    ProgrammerOptions options;
};

#endif // PROGRAMMERARGUMENTS_H
