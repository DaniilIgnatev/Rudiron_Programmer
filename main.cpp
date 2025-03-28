#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>

#include "argumentsparser.h"
#include "programmer.hpp"

int main(int argc, char *argv[])
{
#ifdef TEST
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    int argc2 = 13;
    char** argv2 = new char*[argc2];
    argv2[0] = (char*)"C:/Users/folke/Documents/git/build-Rudiron Programmer-Desktop_Qt_5_12_10_MSVC2017_64bit-Debug/debug/Rudiron Programmer.exe";
    argv2[1] = (char*)"C:/Users/folke/Documents/git/build-Rudiron Programmer-Desktop_Qt_5_12_10_MSVC2017_64bit-Debug/debug/1986_BOOT_UART.hex";
    argv2[2] = (char*)"C:/Users/folke/Documents/git/build-Rudiron Programmer-Desktop_Qt_5_12_10_MSVC2017_64bit-Debug/debug/RudironDiagnostics.hex";
    argv2[3] = (char*)"--keep_open";
    argv2[4] = (char*)"--english";
    argv2[5] = (char*)"--verify_bootloader";
    argv2[6] = (char*)"--erase";
    argv2[7] = (char*)"--load";
    argv2[8] = (char*)"--verify_program";
    argv2[9] = (char*)"--run";
    argv2[10] = (char*)"--speed";
    argv2[11] = (char*)"16";
    argv2[12] = (char*)"--help";

    QCoreApplication a(argc2, argv2);
#else
    QCoreApplication a(argc, argv);
#endif
    QCoreApplication::setApplicationName("Rudiron Programmer");
    QCoreApplication::setApplicationVersion("1.2.0");

    ArgumentsParser parser;
    ProgrammerArguments arguments = parser.processProgrammerArguments(a);

    int start_code = 1;

    if (arguments.correct()){
       Programmer* programmer = new Programmer(arguments);
       start_code = programmer->start() ? 0 : 2;
       programmer->deleteLater();
    }

    if (arguments.keepOpen){
        if (arguments.english){
            qDebug() << "Press Ctrl+C to close the program...";
        }
        else{
            qDebug() << "Нажмите Ctrl+C, чтобы закрыть программу...";
        }
        return a.exec();
    }
    else{
        a.exit(start_code);
        return start_code;
    }
}
