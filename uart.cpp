#include "uart.hpp"
#include <QThread>
#include <QMetaEnum>


UART::UART(QObject *parent)
    : QObject{parent}
{

}

int UART::getByte(int at)
{
    if (at >= this->rx_buffer_index){
        return -1;
    }
    else{
        return this->rx_buffer[at];
    }
}

int UART::count()
{
    return this->rx_buffer_index;
}

bool UART::begin(QString name)
{
    serial = new QSerialPort(name, this);
    connect(serial, &QSerialPort::errorOccurred, this, &UART::errorSlot);
    connect(serial, &QSerialPort::readyRead, this, &UART::readyReadSlot);

    if (!serial->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections)){
        return false;
    }
    if (!serial->setDataBits(QSerialPort::Data8)){
        return false;
    }
    if (!serial->setStopBits(QSerialPort::OneStop)){
        return false;
    }
    if (!serial->setParity(QSerialPort::NoParity)){
        return false;
    }
    if (!serial->setFlowControl(QSerialPort::FlowControl::NoFlowControl)){
        return false;
    }

    if (!serial->open(QIODevice::ReadWrite)) {
        return false;
    }

    return true;
}

void UART::end()
{
    serial->close();
    serial->deleteLater();
}

void UART::errorSlot(QSerialPort::SerialPortError error){
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();
    QString error_str = metaEnum.valueToKey(error);
    qDebug() << "Serial error: " << error_str;
}

void UART::write(char byte)
{
    serial->write(QByteArray(1, byte));
    serial->waitForBytesWritten();
    QThread::currentThread()->msleep(1);
}

void UART::write(QByteArray buffer)
{
    serial->write(buffer);
    serial->waitForBytesWritten();
    QThread::currentThread()->msleep(1);
}

int UART::readByte()
{
    if (serial->atEnd()){
        return -1;
    }
    else{
        if (this->rx_buffer_index < this->rx_buffer_size){
            char byte = serial->read(1).at(0);
            this->rx_buffer[this->rx_buffer_index] = byte;
            this->rx_buffer_index++;
            return byte;
        }
        else{
            return -1;
        }
    }
}

void UART::readyReadSlot()
{
    while (!serial->atEnd()) {
        readByte();
    }
}

int UART::popByte()
{
    if (rx_buffer_index == 0){
        return -1;
    }

    char byte = rx_buffer[0];

    for (int i = 1; i < rx_buffer_size; i++){
        rx_buffer[i - 1] = rx_buffer[i];
    }

    rx_buffer[rx_buffer_size - 1] = 0;
    rx_buffer_index -= 1;

    return byte;
}
