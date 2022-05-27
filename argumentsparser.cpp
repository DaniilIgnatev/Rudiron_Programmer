#include "argumentsparser.h"

ArgumentsParser::ArgumentsParser(QObject *parent) : QObject(parent)
{
    parser.setApplicationDescription("Утилита программирования Rudiron плат через COM порт.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("bootloader", "Полный путь до загрузчика.");
    parser.addPositionalArgument("program", "Полный путь до загружаемой программы пользователя.");

    ///Опция показа прогресса загрузки и проверки программы в ПЗУ
    QCommandLineOption showProgressOption(QStringList() << showProgressOption_short << showProgressOption_long,
                                   "Показывать прогресс загрузки и верификации программы пользователя.");
    parser.addOption(showProgressOption);

    ///Опция полной очистки ПЗУ
    QCommandLineOption eraseOption(QStringList() << eraseOption_short << eraseOption_long,
                                   "Полная очистка памяти.");
    parser.addOption(eraseOption);

    ///Опция загрузки программы пользователя в ПЗУ
    QCommandLineOption loadOption(QStringList() << loadOption_short << loadOption_long,
                                  "Загрузка программы пользователя.");
    parser.addOption(loadOption);

    ///Опция полной проверки загруженной в ПЗУ программы пользователя
    QCommandLineOption verifyOption(QStringList() << verifyOption_short << verifyOption_long,
                                  "Проверка программы пользователя.");
    parser.addOption(verifyOption);

    ///Опция запуска программы пользователя из ПЗУ
    QCommandLineOption runOption(QStringList() << runOption_short << runOption_long,
                                  "Запуск программы пользователя.");
    parser.addOption(runOption);

    ///Опция установки скорости обмена с бутлодером (baud rate)
    QCommandLineOption speedMultiplierOption(QStringList() << speedMultiplierOption_short << speedMultiplierOption_long,
                                  "Установить умножитель частоты обмена.",
                                  "Варианты: 1 (14400), 2 (28800), 4 (57600), 8 (115200), 16 (230400)");
    parser.addOption(speedMultiplierOption);
}

ProgrammerArguments ArgumentsParser::processProgrammerArguments(QCoreApplication& a)
{
    parser.process(a);
    const QStringList pos_args = parser.positionalArguments();

    ProgrammerArguments arguments;
    arguments.bootloaderPath = pos_args[0];
    arguments.programPath = pos_args[1];

    ProgrammerOptions options;
    if (parser.isSet(eraseOption_long)){
        options.check(ProgrammerOptionsEnum::Erase);
    }
    if (parser.isSet(loadOption_long)){
        options.check(ProgrammerOptionsEnum::Load);
    }
    if (parser.isSet(verifyOption_long)){
        options.check(ProgrammerOptionsEnum::Verify);
    }
    if (parser.isSet(runOption_long)){
        options.check(ProgrammerOptionsEnum::Run);
    }

    arguments.options = options;
    arguments.showProgress = parser.isSet(showProgressOption_long);
    arguments.speedMultiplier = parser.value(speedMultiplierOption_long).toInt();

    return arguments;
}
