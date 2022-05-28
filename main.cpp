#include <QCoreApplication>
#include "argumentsparser.h"
#include "programmer.hpp"


int main(int argc, char *argv[])
{
    int argc2 = 10;
    char** argv2 = new char*[argc2];
    argv2[0] = (char*)"C:/Users/daniil/Documents/GitHub/build-RudironProgrammer-Qt_5_12_10_msvc2017_64-Debug/debug/RudironProgrammer.exe";
//    argv2[1] = (char*)"/Users/daniilignatev/Yandex.Disk.localized/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/1986_BOOT_UART.hex";
    argv2[1] = "C:/Users/user/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/1986_BOOT_UART.hex";
//    argv2[2] = (char*)"/Users/daniilignatev/Yandex.Disk.localized/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/RudironDiagnostics.hex";
    argv2[2] = "C:/Users/user/YandexDisk/Documents/Rudiron/Distributive/components/win_x64/RudironDistributive/tools/UART programmer/1986UARTWSD/RudironDiagnostics.hex";
    argv2[3] = (char*)"-P";
    argv2[4] = (char*)"-E";
    argv2[5] = (char*)"-L";
    argv2[6] = (char*)"-V";
    argv2[7] = (char*)"-R";
    argv2[8] = (char*)"-S";
    argv2[9] = (char*)"16";

    QCoreApplication a(argc2, argv2);
    QCoreApplication::setApplicationName("RudironProgrammer");
    QCoreApplication::setApplicationVersion("1.0");

    ArgumentsParser parser;
    ProgrammerArguments arguments = parser.processProgrammerArguments(a);

    Programmer *programmer = new Programmer(arguments);
    programmer->start();

    return a.exec();
}
