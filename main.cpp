#include <QCoreApplication>
#include "argumentsparser.h"
#include "programmer.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("RudironProgrammer");
    QCoreApplication::setApplicationVersion("1.0");

    ArgumentsParser parser;
    ProgrammerArguments arguments = parser.processProgrammerArguments(a);

    Programmer *programmer = new Programmer(arguments);
    programmer->start();

    return a.exec();
}
