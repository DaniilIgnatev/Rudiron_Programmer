#include "argumentsparser.h"

ArgumentsParser::ArgumentsParser()
{
    parser.setApplicationDescription("Утилита программирования Rudiron плат через COM порт.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("bootloader", "Полный путь до загрузчика.");
    parser.addPositionalArgument("program", "Полный путь до загружаемой программы пользователя.");

    ///Ожидает нажатия клавиши для закрытия
    QCommandLineOption keepOpeOption(keepOpen_name,
                                   "Ожидает нажатие клавиши перед завершением.");
    parser.addOption(keepOpeOption);

    ///Опция верификации загрузчика
    QCommandLineOption verifyBootloaderOption(verifyBootloaderOption_name,
                                   "Проверка загрузчика.");
    parser.addOption(verifyBootloaderOption);

    ///Опция полной очистки ПЗУ
    QCommandLineOption eraseOption(eraseOption_name,
                                   "Полная очистка памяти.");
    parser.addOption(eraseOption);

    ///Опция загрузки программы пользователя в ПЗУ
    QCommandLineOption loadOption(loadOption_name,
                                  "Загрузка программы пользователя.");
    parser.addOption(loadOption);

    ///Опция полной проверки загруженной в ПЗУ программы пользователя
    QCommandLineOption verifyProgramOption(verifyProgramOption_name,
                                  "Проверка программы пользователя.");
    parser.addOption(verifyProgramOption);

    ///Опция запуска программы пользователя из ПЗУ
    QCommandLineOption runOption(runOption_name,
                                  "Запуск программы пользователя.");
    parser.addOption(runOption);

    ///Принудительно задает имя COM-порта.
    QCommandLineOption portNameOption(portNameOption_name,
                                  "Установить имя последовательного порта <name>.",
                                  "<name>");
    parser.addOption(portNameOption);

    ///Опция установки скорости обмена с бутлодером (baud rate)
    QCommandLineOption speedMultiplierOption(speedMultiplierOption_name,
                                  "Установить умножитель скорости обмена <multiplier>. Возможные значения: 1 (14400), 2 (28800), 4 (57600), 8 (115200), 16 (230400)",
                                  "<multiplier>");
    parser.addOption(speedMultiplierOption);
}

ProgrammerArguments ArgumentsParser::processProgrammerArguments(QCoreApplication& a)
{
    parser.process(a);
    const QStringList pos_args = parser.positionalArguments();

    ProgrammerArguments arguments;
    arguments.bootloaderPath = pos_args[0];
    arguments.programPath = pos_args[1];

    arguments.keepOpen = parser.isSet(keepOpen_name);

    ProgrammerOptions options;
    if (parser.isSet(verifyBootloaderOption_name)){
        options.check(ProgrammerOptionsEnum::VerifyBootloader);
    }
    if (parser.isSet(eraseOption_name)){
        options.check(ProgrammerOptionsEnum::Erase);
    }
    if (parser.isSet(loadOption_name)){
        options.check(ProgrammerOptionsEnum::Load);
    }
    if (parser.isSet(verifyProgramOption_name)){
        options.check(ProgrammerOptionsEnum::VerifyProgram);
    }
    if (parser.isSet(runOption_name)){
        options.check(ProgrammerOptionsEnum::Run);
    }

    arguments.options = options;

    if (parser.isSet(portNameOption_name)){
        arguments.portName = parser.value(portNameOption_name);
    }

    arguments.speedMultiplier = parser.value(speedMultiplierOption_name).toInt();

#ifdef QT_DEBUG
    qDebug() << "Путь до загрузчика: " << arguments.bootloaderPath;
    qDebug() << "Путь до программы пользователя: " << arguments.programPath;
    qDebug() << "Имя последовательного порта: " << arguments.portName;
    qDebug() << "Умножитель скорости: " << arguments.speedMultiplier;
    qDebug() << "ProgrammerOptionsEnum::VerifyProgram: " << arguments.options.checked(ProgrammerOptionsEnum::VerifyProgram);
    qDebug() << "ProgrammerOptionsEnum::Erase: " << arguments.options.checked(ProgrammerOptionsEnum::Erase);
    qDebug() << "ProgrammerOptionsEnum::Load: " << arguments.options.checked(ProgrammerOptionsEnum::Load);
    qDebug() << "ProgrammerOptionsEnum::VerifyProgram: " << arguments.options.checked(ProgrammerOptionsEnum::VerifyProgram);
    qDebug() << "ProgrammerOptionsEnum::Run: " << arguments.options.checked(ProgrammerOptionsEnum::Run);
#endif

    return arguments;
}
