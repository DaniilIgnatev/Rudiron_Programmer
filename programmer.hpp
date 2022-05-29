#ifndef PROGRAMMER_HPP
#define PROGRAMMER_HPP

#include <QObject>
#include "uart.hpp"
#include <QList>
#include <QSerialPortInfo>
#include <QThread>

#include "programmerarguments.h"

#include "hexramparser.hpp"
#include "hexflashparser.hpp"


class Programmer : public QObject
{
    Q_OBJECT
private:
    const QString id_str = "1986BOOTUART";

    bool initialized = false;
    ProgrammerArguments arguments;

    UART uart;
    QByteArray txdbuf;

    HEXRAMParser ramParser;
    HexFlashParser flashParser;

public:
    explicit Programmer(ProgrammerArguments arguments, QObject *parent = nullptr);

    bool start();

private:

    int getSpeed();

    void sync();

    bool flashBootloader_sync();

    bool flashBootloader_switchSpeed();

    bool flashBootloader_load();

    bool flashBootloader_verify();

    bool flashBootloader_run();

    bool flashBootloader_identify();

    bool flashProgram_erase();

    bool flashProgram_load();

    bool flashProgram_verify();

    bool flashProgram_run();

public:

    bool getInitialized() const;

signals:

};

#endif // PROGRAMMER_HPP
