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
    ProgrammerOptionsEnum value = ::None;

public:
    bool checked(ProgrammerOptionsEnum option);
    void check(ProgrammerOptionsEnum option);
};


struct ProgrammerArguments{
    bool showProgress = false;

    QString bootloaderPath;

    QString programPath;

    int speedMultiplier = 1;

    ProgrammerOptions options;
};

#endif // PROGRAMMERARGUMENTS_H
