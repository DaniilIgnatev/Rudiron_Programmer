#ifndef UART_HPP
#define UART_HPP

#include "TypeDefs.hpp"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class UART : public QObject
{
    Q_OBJECT
    public:
    int read_delay = 1;

    QSerialPort* serial = nullptr;

    static const unsigned char rx_buffer_size = 255;

    unsigned char rx_buffer[rx_buffer_size] = {0};

    ///Индекс последнего свободного байта в буфере приема
    unsigned char rx_buffer_index = 0;

    int readByte();

    int popByte();

    int getEventLoopDelay();

public:
    explicit UART(QObject *parent = nullptr);

    int getByte(int at);

    bool begin(QSerialPortInfo port);

    void end();

    void setBaudRate(qint32 rate);

    void writeSync();

    void writeRead(QByteArray buffer, int readBytes);

    void waitRead(int timeout = 100);

    void clearRXBuffer();

signals:

public slots:
    void errorSlot(QSerialPort::SerialPortError error);

    void readyReadSlot();
};

#endif // UART_HPP
