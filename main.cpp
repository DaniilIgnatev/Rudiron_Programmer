#include <QCoreApplication>
#include "argumentsparser.h"
#include "programmer.hpp"


int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    int argc2 = 11;
    char** argv2 = new char*[argc2];
    argv2[0] = (char*)"C:/Users/daniil/Documents/GitHub/build-RudironProgrammer-Qt_5_12_10_msvc2017_64-Debug/debug/RudironProgrammer.exe";
    argv2[1] = (char*)"/Users/daniilignatev/GitHub/Rudiron_Diagnostics/1986_BOOT_UART.hex";
    argv2[2] = (char*)"/Users/daniilignatev/GitHub/Rudiron_Diagnostics/RudironDiagnostics.hex";
    argv2[3] = (char*)"keep_open";
    argv2[4] = (char*)"--verify_bootloader";
    argv2[5] = (char*)"--erase";
    argv2[6] = (char*)"--load";
    argv2[7] = (char*)"--verify_program";
    argv2[8] = (char*)"--run";
    argv2[9] = (char*)"--speed";
    argv2[10] = (char*)"16";

    QCoreApplication a(argc2, argv2);
#else
    QCoreApplication a(argc, argv);
#endif

    QCoreApplication::setApplicationName("Rudiron programmer");
    QCoreApplication::setApplicationVersion("1.1.0");

    ArgumentsParser parser;
    ProgrammerArguments arguments = parser.processProgrammerArguments(a);

    Programmer *programmer = new Programmer(arguments);
    int start_code = programmer->start() ? 0 : 1;

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
        return start_code;
    }
}
