#include <QCoreApplication>
#include "argumentsparser.h"
#include "programmer.hpp"


int main(int argc, char *argv[])
{
    int argc2 = 10;
    char** argv2 = new char*[argc2];
    argv2[0] = (char*)"/Users/daniilignatev/GitHub/build-RudironProgrammer-Desktop_Qt_5_12_12_clang_64bit-Debug/RudironProgrammer";
    argv2[1] = (char*)"Hello";
    argv2[2] = (char*)"World";
    argv2[3] = (char*)"-P";
    argv2[4] = (char*)"-E";
    argv2[5] = (char*)"-L";
    argv2[6] = (char*)"-V";
    argv2[7] = (char*)"-R";
    argv2[8] = (char*)"-S";
    argv2[9] = (char*)"2";

    QCoreApplication a(argc2, argv2);
    QCoreApplication::setApplicationName("RudironProgrammer");
    QCoreApplication::setApplicationVersion("1.0");

    ArgumentsParser parser;
    ProgrammerArguments arguments = parser.processProgrammerArguments(a);

    Programmer *programmer = new Programmer(arguments);
    programmer->start();

    return a.exec();
}
