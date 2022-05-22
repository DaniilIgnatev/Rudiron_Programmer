#ifndef UART_HPP
#define UART_HPP

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QtDebug>
#include <QSerialPortInfo>

class UART : public QObject
{
    Q_OBJECT
private:
    QSerialPort* serial = nullptr;

    static const unsigned char rx_buffer_size = 255;

    unsigned char rx_buffer[rx_buffer_size] = {0};

    ///Индекс последнего свободного байта в буфере приема
    unsigned char rx_buffer_index = 0;

public:
    explicit UART(QObject *parent = nullptr);

    int getByte(int at);

    int count();

    bool begin(QSerialPortInfo port);

    void end();

    void write(char byte);

    void write(QByteArray buffer);

    void write(char* buffer, int size);

    int readByte();

    int popByte();

    void waitRead(int timeout = 100);

    ///Очищает буфер приема
    void clearRXBuffer();

signals:

public slots:
    void errorSlot(QSerialPort::SerialPortError error);

    void readyReadSlot();
};

#endif // UART_HPP
