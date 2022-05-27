#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <QObject>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "programmerarguments.h"


class ArgumentsParser : public QObject
{
    Q_OBJECT
private:
  QCommandLineParser parser;

  const QString showProgressOption_short = "P";

  const QString showProgressOption_long = "progress";

  const QString eraseOption_short = "E";

  const QString eraseOption_long = "erase";

  const QString loadOption_short = "L";

  const QString loadOption_long = "load";

  const QString verifyOption_short = "V";

  const QString verifyOption_long = "verify";

  const QString runOption_short = "R";

  const QString runOption_long = "run";

  const QString speedMultiplierOption_short = "S";

  const QString speedMultiplierOption_long = "speed";


public:
    explicit ArgumentsParser(QObject *parent = nullptr);

    ProgrammerArguments processProgrammerArguments(QCoreApplication& a);

signals:

};

#endif // ARGUMENTSPARSER_H
