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
    bool english = false;

    bool keepOpen = false;

    QString bootloaderPath;

    QString programPath;

    int speedMultiplier = 0;

    bool hasSpeedMultiplier(){
        return speedMultiplier != 0;
    }

    QString portName;

    ProgrammerOptions options;
};

#endif // PROGRAMMERARGUMENTS_H
