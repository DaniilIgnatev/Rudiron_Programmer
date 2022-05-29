#include <QCoreApplication>
#include "argumentsparser.h"
#include "programmer.hpp"


int main(int argc, char *argv[])
{
    int argc2 = 13;
    char** argv2 = new char*[argc2];
    argv2[0] = (char*)"C:/Users/daniil/Documents/GitHub/build-RudironProgrammer-Qt_5_12_10_msvc2017_64-Debug/debug/RudironProgrammer.exe";
//    argv2[1] = (char*)"/Users/daniilignatev/Yandex.Disk.localized/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/1986_BOOT_UART.hex";
    argv2[1] = (char*)"C:/Users/user/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/1986_BOOT_UART.hex";
//    argv2[2] = (char*)"/Users/daniilignatev/Yandex.Disk.localized/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/RudironDiagnostics.hex";
    argv2[2] = (char*)"C:/Users/user/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/RudironDiagnostics.hex";
    argv2[3] = (char*)"keep_open";
    argv2[4] = (char*)"verify_bootloader";
    argv2[5] = (char*)"erase";
    argv2[6] = (char*)"--load";
    argv2[7] = (char*)"verify_program";
    argv2[8] = (char*)"--run";
    argv2[9] = (char*)"--port";
    argv2[10] = (char*)"COM4";
    argv2[11] = (char*)"--speed";
    argv2[12] = (char*)"16";

    QCoreApplication a(argc2, argv2);
    QCoreApplication::setApplicationName("Rudiron UART programmer");
    QCoreApplication::setApplicationVersion("1.0");

    ArgumentsParser parser;
    ProgrammerArguments arguments = parser.processProgrammerArguments(a);

    Programmer *programmer = new Programmer(arguments);
    int start_code = programmer->start() ? 0 : 1;

    if (arguments.keepOpen){
        return a.exec();
    }
    else{
        return start_code;
    }
}
