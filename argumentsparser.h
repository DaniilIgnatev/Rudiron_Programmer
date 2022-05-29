#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <QObject>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "programmerarguments.h"


class ArgumentsParser
{
private:
  QCommandLineParser parser;

public:
  const QString keepOpen_name = "keep_open";

  const QString portNameOption_name = "port";

  const QString speedMultiplierOption_name = "speed";

  const QString verifyBootloaderOption_name = "verify_bootloader";

  const QString eraseOption_name = "erase";

  const QString loadOption_name = "load";

  const QString verifyProgramOption_name = "verify_program";

  const QString runOption_name = "run";

public:
    explicit ArgumentsParser();

    ProgrammerArguments processProgrammerArguments(QCoreApplication& a);

signals:

};

#endif // ARGUMENTSPARSER_H
