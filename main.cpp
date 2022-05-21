#include <QCoreApplication>
#include "programmer.hpp"
#include <QtSerialPort>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Programmer programmer;
    programmer.start();

    return a.exec();
}
