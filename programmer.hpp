#ifndef PROGRAMMER_HPP
#define PROGRAMMER_HPP

#include <QObject>
#include "uart.hpp"
#include <QList>
#include <QSerialPortInfo>

#include "programmerarguments.h"

#include "hexramparser.hpp"
#include "hexflashparser.hpp"


class Programmer : public QObject
{
    Q_OBJECT
private:
    UART uart;

    const QString id_str = "1986BOOTUART";
    QString bootloader_path;
    QString programm_path;

    HEXRAMParser ramParser;
    HexFlashParser flashParser;

    QByteArray txdbuf;

    ProgrammerOptions options;

    ///baud rate = 14400 * speedMultiplier. Допускаются значения: 1, 2, 4, 8, 16
    int speedMultiplier = 1;

public:
    explicit Programmer(ProgrammerArguments arguments, QObject *parent = nullptr);

    void start();

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

    ProgrammerOptions getOptions() const;

    void setOptions(ProgrammerOptions newOptions);

signals:

};

#endif // PROGRAMMER_HPP
