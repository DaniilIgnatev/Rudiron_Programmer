#include "argumentsparser.h"

ArgumentsParser::ArgumentsParser()
{
    parser.setApplicationDescription("Rudiron board programming utility via COM port. Утилита программирования Rudiron плат через COM порт.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("bootloader_path", "Full path to the bootloader. Полный путь до загрузчика.");
    parser.addPositionalArgument("program_path", "Full path to the downloaded user program. Полный путь до загружаемой программы пользователя.");

    QCommandLineOption englishOption(englishOption_name,
                                   "Sets the output language to English. Устанавливает язык вывода - английский.");
    parser.addOption(englishOption);

    QCommandLineOption printArgumentsOption(printArgumentsOption_name,
                                   "Outputs program arguments to the console. Выводит аргументы программы в консоль.");
    parser.addOption(printArgumentsOption);

    QCommandLineOption keepOpenOption(keepOpen_name,
                                   "Waits for the key to be pressed before completing. Ожидает нажатие клавиши перед завершением.");
    parser.addOption(keepOpenOption);

    QCommandLineOption verifyBootloaderOption(verifyBootloaderOption_name,
                                   "Checking the boot loader. Проверка загрузчика.");
    parser.addOption(verifyBootloaderOption);

    QCommandLineOption eraseOption(eraseOption_name,
                                   "Full memory cleanup. Полная очистка памяти.");
    parser.addOption(eraseOption);

    QCommandLineOption loadOption(loadOption_name,
                                  "Loading the user program. Загрузка программы пользователя.");
    parser.addOption(loadOption);

    QCommandLineOption verifyProgramOption(verifyProgramOption_name,
                                  "Checking the user program. Проверка программы пользователя.");
    parser.addOption(verifyProgramOption);

    QCommandLineOption runOption(runOption_name,
                                  "Running a user program. Запуск программы пользователя.");
    parser.addOption(runOption);

    QCommandLineOption portNameOption(portNameOption_name,
                                  "Set serial port name <name>. Установить имя последовательного порта <name>.",
                                  "<name>");
    parser.addOption(portNameOption);

    QCommandLineOption speedMultiplierOption(speedMultiplierOption_name,
                                  "Set the exchange rate multiplier <multiplier>. Possible values: 0(default speed), 1 (14400), 2 (28800), 4 (57600), 8 (115200), 16 (230400). Установить умножитель скорости обмена <multiplier>. Возможные значения: 0(скорость по умолчанию), 1 (14400), 2 (28800), 4 (57600), 8 (115200), 16 (230400).",
                                  "<multiplier>");
    parser.addOption(speedMultiplierOption);
}

ProgrammerArguments ArgumentsParser::processProgrammerArguments(QCoreApplication& a)
{
    ProgrammerArguments arguments;
    parser.process(a);

    arguments.english = parser.isSet(englishOption_name);

    const QStringList pos_args = parser.positionalArguments();
    if (pos_args.count() < 2){
        qDebug() << "Need at least 2 arguments: bootloader and user-program paths! Необходимо указать пути до загрузчика и программы пользователя!";
        return arguments;
    }

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

    if (parser.isSet(speedMultiplierOption_name)){
        arguments.speedMultiplier = parser.value(speedMultiplierOption_name).toInt();
    }

    if (parser.isSet(printArgumentsOption_name)){
        qDebug() << "Bootloader path: " << arguments.bootloaderPath;
        qDebug() << "User-program path: " << arguments.programPath;
        qDebug() << "Serial port name: " << arguments.portName;
        qDebug() << "Baud rate speed multiplier: " << arguments.speedMultiplier;
        qDebug() << "ProgrammerOptionsEnum::VerifyProgram: " << arguments.options.checked(ProgrammerOptionsEnum::VerifyProgram);
        qDebug() << "ProgrammerOptionsEnum::Erase: " << arguments.options.checked(ProgrammerOptionsEnum::Erase);
        qDebug() << "ProgrammerOptionsEnum::Load: " << arguments.options.checked(ProgrammerOptionsEnum::Load);
        qDebug() << "ProgrammerOptionsEnum::VerifyProgram: " << arguments.options.checked(ProgrammerOptionsEnum::VerifyProgram);
        qDebug() << "ProgrammerOptionsEnum::Run: " << arguments.options.checked(ProgrammerOptionsEnum::Run);
    }

    return arguments;
}
