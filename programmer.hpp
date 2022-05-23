#ifndef PROGRAMMER_HPP
#define PROGRAMMER_HPP

#include <QObject>
#include "uart.hpp"
#include <QList>
#include <QSerialPortInfo>

#include "hexramparser.hpp"
#include "hexflashparser.hpp"


class Programmer : public QObject
{
    Q_OBJECT
private:
    UART uart;

    QString bootloader_path = "/Users/daniilignatev/GitHub/RudironProgrammer/1986_BOOT_UART.hex";
    QString programm_path = "/Users/daniilignatev/GitHub/RudironProgrammer/RudironDiagnostics.hex";

    HEXRAMParser ramParser;
    HexFlashParser flashParser;

    QByteArray txdbuf;

public:
    explicit Programmer(QObject *parent = nullptr);

    void start();

private:

    void sync();

    bool flashBootloader();

    bool flashBootloader_sync();

    bool flashBootloader_load();

    bool flashBootloader_verify();

    bool flashBootloader_run();

    bool flashBootloader_identify();

    BOOL Program(void);




public:

signals:

};

#endif // PROGRAMMER_HPP
