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

    QString bootloader_path = "C:/Users/daniil/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/1986_BOOT_UART.hex";
    QString programm_path = "C:/Users/daniil/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/RudironDiagnostics.hex";

    HEXRAMParser ramParser;
    HexFlashParser flashParser;

    QByteArray txdbuf;

public:
    explicit Programmer(QObject *parent = nullptr);

    void start();

private:

    void sync();

    void flashBootloader();

    void flashBootloader_sync();

    void flashBootloader_load();

    void flashBootloader_verify();

    BOOL Program(void);




public:

signals:

};

#endif // PROGRAMMER_HPP
