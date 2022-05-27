#ifndef PROGRAMMERARGUMENTS_H
#define PROGRAMMERARGUMENTS_H

#include "TypeDefs.hpp"

enum ProgrammerOptionsEnum{
    None = 0,
    Erase = 1,
    Load = 2,
    Verify = 4,
    Run = 8
};

struct ProgrammerOptions{
private:
    ProgrammerOptionsEnum value;

public:
    bool checked(ProgrammerOptionsEnum option);
    void check(ProgrammerOptionsEnum option);
};

struct ProgrammerArguments{
    bool showProgress;

    QString bootloaderPath;

    QString programPath;

    int speedMultiplier;

    ProgrammerOptions options;
};

#endif // PROGRAMMERARGUMENTS_H
