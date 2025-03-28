#include "argumentsparser.h"

ArgumentsParser::ArgumentsParser()
{
    parser.setApplicationDescription("(EN) Rudiron COM port flash tool.\n(RU) Утилита программирования Rudiron через COM порт.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("bootloader_path", "(EN) Full path to the bootloader hex file.\n(RU) Полный путь до hex файла загрузчика.");
    parser.addPositionalArgument("program_path", "(EN) Full path to the flashed program hex file.\n(RU) Полный путь до прошиваемой программы.");

    QCommandLineOption englishOption(englishOption_name,
                                   "(EN) Set the output language to English.\n(RU) Установить язык вывода - английский.");
    parser.addOption(englishOption);

    QCommandLineOption printArgumentsOption(printArgumentsOption_name,
                                   "(EN) Print program's start arguments to the terminal.\n(RU) Печать аргументов запуска программы в терминал.");
    parser.addOption(printArgumentsOption);

    QCommandLineOption keepOpenOption(keepOpen_name,
                                   "(EN) Wait for the Ctrl+C before the exit.\n(RU) Ожидать нажатия сочетания клавиш Ctrl+C для завершения работы.");
    parser.addOption(keepOpenOption);

    QCommandLineOption verifyBootloaderOption(verifyBootloaderOption_name,
                                   "(EN) Verify the correctness of flashed boot loader.\n(RU) Проверка корректности записи загрузчика в оперативную память.");
    parser.addOption(verifyBootloaderOption);

    QCommandLineOption eraseOption(eraseOption_name,
                                   "(EN) Clean the whole flash memory.\n(RU) Произвести полную очистку флеш-памяти.");
    parser.addOption(eraseOption);

    QCommandLineOption loadOption(loadOption_name,
                                  "(EN) Flash the user program.\n(RU) Прошить пользовательскую программу в флеш-память.");
    parser.addOption(loadOption);

    QCommandLineOption verifyProgramOption(verifyProgramOption_name,
                                  "(EN) Verify the correctness of flashed user program.\n(RU) Проверка корректности записи пользовательской программы во флеш-память.");
    parser.addOption(verifyProgramOption);

    QCommandLineOption runOption(runOption_name,
                                  "(EN) Run the flashed user program.\n(RU) Запуск прошитой во флеш-память программы.");
    parser.addOption(runOption);

    QCommandLineOption portNameOption(portNameOption_name,
                                  "(EN) Set serial port name <name>.\n(RU) Задать имя последовательного порта <name>.",
                                  "<name>");
    parser.addOption(portNameOption);

    QCommandLineOption speedMultiplierOption(speedMultiplierOption_name,
                                  "(EN) Set the exchange rate multiplier <multiplier>. Possible values: 0 (default value), 1 (14400 baud/s), 2 (28800 baud/s), 4 (57600 baud/s), 8 (115200 baud/s), 16 (230400 baud/s).\n(RU) Установить умножитель скорости обмена <multiplier>. Возможные значения: 0 (по умолчанию), 1 (14400 baud/s), 2 (28800 baud/s), 4 (57600 baud/s), 8 (115200 baud/s), 16 (230400 baud/s).",
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
        qDebug() << "(EN) This tool requires 2 arguments to run: a full path to bootloader hex file and a full path to flashed program hex file. For example: ./\"Rudiron Programmer\" \"1986_BOOT_UART.hex\" \"RudironDiagnostics.hex\".\n(RU) Для запуска утилиты обязательно нужно передать 2 аргумента: полный путь до hex файла загрузчика и полный путь до hex файла прошиваемой пользовательской программы. Например: For example: ./\"Rudiron Programmer\" \"1986_BOOT_UART.hex\" \"RudironDiagnostics.hex\".";
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
