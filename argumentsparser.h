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

public:
    explicit ArgumentsParser(QObject *parent = nullptr);

    ProgrammerArguments processProgrammerArguments(QCoreApplication& a);

signals:

};

#endif // ARGUMENTSPARSER_H
