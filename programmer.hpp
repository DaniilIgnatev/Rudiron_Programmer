#ifndef PROGRAMMER_HPP
#define PROGRAMMER_HPP

#include <QObject>
#include "uart.hpp"
#include <QList>
#include <QSerialPortInfo>
#include "hexparser.hpp"


class Programmer : public QObject
{
    Q_OBJECT
private:
    UART uart;

    QString bootloader_path = "C:/Users/daniil/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/1986_BOOT_UART.hex";
    QString programm_path = "C:/Users/daniil/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/RudironDiagnostics.hex";

    DWORD dwadr_seg_hex,dwadr_lineoffs_hex;
    DWORD dwadrboot;
    int ilboot,ilcod;

    static const int txdbuf_size = 512;
    char txdbuf[txdbuf_size] = {0};
    void clearTXDBuf();

public:
    explicit Programmer(QObject *parent = nullptr);

    void start();

private:

    BOOL Program(void);


public:

signals:

};

#endif // PROGRAMMER_HPP
