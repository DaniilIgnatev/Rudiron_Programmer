#include "argumentsparser.h"

ArgumentsParser::ArgumentsParser(QObject *parent) : QObject(parent)
{
    parser.setApplicationDescription("UART programming tool for Rudiron boards.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("bootloader", "MCU bootloader full path");
    parser.addPositionalArgument("program", "User-written program full path");

    ///Опция показа прогресса загрузки и проверки программы в ПЗУ
    QCommandLineOption showProgressOption("p", "Show progress during copy");
    parser.addOption(showProgressOption);

    ///Опция полной очистки ПЗУ
    QCommandLineOption eraseOption(QStringList() << "e" << "erase",
            QCoreApplication::translate("main", "Erase SRAM memory."));
    parser.addOption(eraseOption);

    ///Опция загрузки программы пользователя в ПЗУ
    QCommandLineOption loadOption(QStringList() << "l" << "load",
            QCoreApplication::translate("main", "Load user-program in SRAM memory."));
    parser.addOption(loadOption);

    ///Опция полной проверки загруженной в ПЗУ программы пользователя
    QCommandLineOption verifyOption(QStringList() << "v" << "verify",
            QCoreApplication::translate("main", "Verify user-program in SRAM memory."));
    parser.addOption(verifyOption);

    ///Опция запуска программы пользователя из ПЗУ
    QCommandLineOption runOption(QStringList() << "r" << "run",
            QCoreApplication::translate("main", "Run user-program in SRAM memory."));
    parser.addOption(runOption);

    ///Опция установки скорости обмена с бутлодером (baud rate)
    QCommandLineOption targetDirectoryOption(QStringList() << "bm" << "baud-rate multiplier",
            QCoreApplication::translate("main", "Set programmer <baud rate multiplier>."),
            QCoreApplication::translate("main", "multiplier: 1 (14400), 2 (28800), 4 (57600), 8 (115200), 16 (230400)"));
    parser.addOption(targetDirectoryOption);
}

ProgrammerArguments ArgumentsParser::processProgrammerArguments(QCoreApplication& a)
{
    parser.process(a);
    const QStringList pos_args = parser.positionalArguments();

    ProgrammerArguments argumetns;
    argumetns.bootloaderPath = pos_args[0];
    argumetns.programPath = pos_args[1];

    ProgrammerOptions options;
    options.

    bool showProgress = parser.isSet(showProgressOption);
    bool force = parser.isSet(forceOption);
    QString targetDir = parser.value(targetDirectoryOption);
}
