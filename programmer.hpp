#ifndef PROGRAMMER_HPP
#define PROGRAMMER_HPP

#include <QObject>
#include "uart.hpp"
#include <QList>
#include <QSerialPortInfo>

#include "hexramparser.hpp"
#include "hexflashparser.hpp"


enum ProgrammerOptions{
    None = 0,
    Erase = 1,
    Programm = 2,
    Verify = 4,
    Run = 8
};


class Programmer : public QObject
{
    Q_OBJECT
private:
    UART uart;

    const QString id_str = "1986BOOTUART";
    QString bootloader_path = "/Users/daniilignatev/GitHub/RudironProgrammer/1986_BOOT_UART.hex";
    QString programm_path = "/Users/daniilignatev/GitHub/RudironProgrammer/RudironDiagnostics.hex";

    HEXRAMParser ramParser;
    HexFlashParser flashParser;

    QByteArray txdbuf;

    ProgrammerOptions options;

public:
    explicit Programmer(QObject *parent = nullptr);

    void start();

private:

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
    bool optionChecked(ProgrammerOptions option);
    void setOptions(ProgrammerOptions newOptions);
    void checkOption(ProgrammerOptions option);

signals:

};

#endif // PROGRAMMER_HPP
