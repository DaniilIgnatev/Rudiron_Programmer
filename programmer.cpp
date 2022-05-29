#include "programmer.hpp"


bool Programmer::getInitialized() const
{
    return initialized;
}

Programmer::Programmer(ProgrammerArguments arguments, QObject *parent)
    : QObject{parent}
{
    initialized = true;

    ramParser.setHexPath(arguments.bootloaderPath);
    if (!ramParser.initialize()){
        qDebug() << "Ошибка чтения файла загрузчика " << arguments.bootloaderPath;
        initialized = false;
    }

    flashParser.setHexPath(arguments.programPath);
    if (!flashParser.initialize()){
        qDebug() << "Ошибка чтения файла программы " << arguments.programPath;
        initialized = false;
    }

    if (!((arguments.speedMultiplier % 2 == 0) && (arguments.speedMultiplier >= 1 && arguments.speedMultiplier <= 16))){
        qDebug() << "Некорректное значение умножителя частоты обмена " << arguments.speedMultiplier;
        initialized = false;
    }

    this->arguments = arguments;
}

bool Programmer::start()
{
    if (!initialized){
        qDebug() << "Программа запущена с некорректными аргументами! Завершение работы.";
        return false;
    }

    if (!open()){
        return false;
    }

    if (!flashBootloader_sync()){
        return false;
    }
    if (!flashBootloader_switchSpeed()){
        return false;
    }
    if (!flashBootloader_load()){
        return false;
    }
    if (arguments.options.checked(ProgrammerOptionsEnum::VerifyBootloader)){
        if (!flashBootloader_verify()){
            return false;
        }
    }
    if (!flashBootloader_run()){
        return false;
    }
    if (!flashBootloader_identify()){
        return false;
    }

    if (arguments.options.checked(ProgrammerOptionsEnum::Erase)){
        if (!flashProgram_erase()){
            return false;
        }
    }
    if (arguments.options.checked(ProgrammerOptionsEnum::Load)){
        if (!flashProgram_load()){
            return false;
        }
    }
    if (arguments.options.checked(ProgrammerOptionsEnum::VerifyProgram)){
        if (!flashProgram_verify()){
            return false;
        }
    }
    if (arguments.options.checked(ProgrammerOptionsEnum::Run)){
        if (!flashProgram_run()){
            return false;
        }
    }

    uart.end();
    return true;
}

bool Programmer::open()
{
    QSerialPortInfo port;
    auto ports = QSerialPortInfo::availablePorts();

    if (arguments.portName.isEmpty()){
        for (auto &p: ports){
            if (p.productIdentifier() == 60000){
                port = p;
                break;
            }
        }
    }
    else{
        for (auto &p: ports){
            if (p.portName() == arguments.portName){
                port = p;
                break;
            }
        }
    }

    if (port.isNull() || !uart.begin(port)){
        qDebug() << "Ошибка открытия COM порта.";
        return false;
    }

    return true;
}

int Programmer::getSpeed()
{
    return 14400 * arguments.speedMultiplier;
}

bool Programmer::flashBootloader_sync()
{
    qDebug() << "Начал синхронизацию.";
    uart.clearRXBuffer();

    for(int i = 0; i < 512; i++){
        uart.writeSync();

        if (uart.getByte(0) == 0x0D && uart.getByte(1) == 0x0A && uart.getByte(2) == 0x3E) {
            break;
        }
    }

    if	(!(uart.getByte(0) == 0x0D && uart.getByte(1) == 0x0A && uart.getByte(2) == 0x3E)){
        qDebug() << "Ошибка синхронизации!";
        uart.end();
        return false;
    }

    qDebug() << "Завершил синхронизацию.";
    uart.clearRXBuffer();
    return true;
}

bool Programmer::flashBootloader_switchSpeed()
{
    qDebug() << "Начал установку скорости обмена " << getSpeed() << " бод!";

    int speed = getSpeed();

    txdbuf.resize(5);
    txdbuf[0] = 'B';

    txdbuf[1] = *((BYTE*)&speed + 0);
    txdbuf[2] = *((BYTE*)&speed + 1);
    txdbuf[3] = *((BYTE*)&speed + 2);
    txdbuf[4] = *((BYTE*)&speed + 3);

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 0);

    uart.setBaudRate(getSpeed());

    txdbuf.resize(1);
    txdbuf[0] = 0xd;
    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 3);

    if	(uart.getByte(0) != 0xd || uart.getByte(1) != 0xa || uart.getByte(2) != 0x3e){
        qDebug() << "Ошибка установки скорости обмена " << getSpeed() << " бод!";
        uart.end();
        return false;
    }

    qDebug() << "Закончил установку скорости обмена " << getSpeed() << " бод!";
    return true;
}

bool Programmer::flashBootloader_load()
{
    qDebug() << "Начал загрузку бутлодера.";
    txdbuf.resize(9);
    txdbuf[0] = 'L';
    txdbuf[1] = (ramParser.getProgram_dwadr() & 0xff);
    txdbuf[2] = (ramParser.getProgram_dwadr() >> 8) & 0xff;
    txdbuf[3] = 0;
    txdbuf[4] = 0x20;
    txdbuf[5] = ramParser.getProgram_il() & 0xff;
    txdbuf[6] = (ramParser.getProgram_il() >> 8) & 0xff;
    txdbuf[7] = 0;
    txdbuf[8] = 0;

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 1);

    if	(uart.getByte(0) != 'L'){
        qDebug() << "Ошибка загрузки бутлодера в начале!";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    uart.writeRead(ramParser.getProgramBuffer_notEmpty(), 1);
    if	(uart.getByte(0) !='K'){
        qDebug() << "Ошибка загрузки бутлодера в конце!";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    qDebug() << "Завершил загрузку бутлодера.";
    return true;
}

bool Programmer::flashBootloader_verify()
{
    qDebug() << "Начал верификацию бутлодера.";

    for (int i = 0; i < (ramParser.getProgram_il() >> 3); i++){
        txdbuf.resize(9);
        txdbuf[0] = 'Y';
        txdbuf[1] = (ramParser.getProgram_dwadr() + 8 * i) & 0xff;
        txdbuf[2] = ((ramParser.getProgram_dwadr() + 8 * i) >> 8) & 0xff;
        txdbuf[3] = 0;
        txdbuf[4] = 0x20;
        txdbuf[5] = 8;
        txdbuf[6] = 0;
        txdbuf[7] = 0;
        txdbuf[8] = 0;

        uart.clearRXBuffer();
        uart.writeRead(txdbuf, 10);

        if ((uart.getByte(0) == 'Y') && (uart.getByte(9) == 'K')){
            for (int j = 0; j < 8; j++){
                if ((unsigned char)uart.getByte(j + 1) != (unsigned char)ramParser.getProgram_buffer().at(ramParser.getProgram_dwadr() + 8 * i + j)){
                    qDebug() << "Ошибка верификации бутлодера!";
                    uart.end();
                    return false;
                }
            }
        }
        else {
            qDebug() << "Ошибка верификации бутлодера!";
            uart.end();
            return false;
        }
    }

    uart.clearRXBuffer();
    qDebug() << "Завершил верификацию бутлодера.";
    return true;
}

bool Programmer::flashBootloader_run()
{
    qDebug() << "Начал запуск бутлодера.";

    txdbuf.resize(5);
    txdbuf[0] = 'R';
    txdbuf[1] = ramParser.getProgram_dwadr() & 0xff;
    txdbuf[2] = (ramParser.getProgram_dwadr() >> 8) & 0xff;
    txdbuf[3] = 0;
    txdbuf[4] = 0x20;

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 1);

    if	(uart.getByte(0) != 'R'){
        qDebug() << "Ошибка запуска бутлодера!";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    qDebug() << "Завершил запуск бутлодера.";
    return true;
}

bool Programmer::flashBootloader_identify()
{
    qDebug() << "Начал идентификацию загрузчика.";

    txdbuf.resize(1);
    txdbuf[0] = 'I';

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 12);

    for (int j = 0; j < 12; j++){
        if (uart.getByte(j) != id_str[j]){
            qDebug() << "Ошибка идентификации загрузчика!";
            uart.end();
            return false;
        }
    }

    qDebug() << "Завершил идентификацию загрузчика.";
    return true;
}

bool Programmer::flashProgram_erase()
{
    qDebug() << "Начал полную очистку памяти.";

    txdbuf.resize(1);
    txdbuf[0] = 'E';

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 9, 800000);

    if	(uart.getByte(0) != 'E'){
        qDebug() << "Ошибка очистки памяти!";
        uart.end();
        return false;
    }

    uint32_t adr = (BYTE)uart.getByte(1) + (((BYTE)uart.getByte(2)) << 8) + (((BYTE)uart.getByte(3)) << 16)
            + (((BYTE)uart.getByte(4)) << 24);
    uint32_t data = (BYTE)uart.getByte(5) + (((BYTE)uart.getByte(6)) << 8) + (((BYTE)uart.getByte(7)) << 16)
            + (((BYTE)uart.getByte(8)) << 24);

    if ((adr == 0x08020000) && (data == 0xffffffff)){
        qDebug() << "Завершил полную очистку памяти.";
        return true;
    }
    else{
        qDebug() << "Ошибка очистки памяти!";
        uart.end();
        return false;
    }
}

bool Programmer::flashProgram_load()
{
    qDebug() << "Начал загрузку основной программы.";

    txdbuf.resize(5);
    txdbuf[0] = 'A';
    txdbuf[1] = 0x00;
    txdbuf[2] = 0x00;
    txdbuf[3] = 0x00;
    txdbuf[4] = 0x08;

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 1);

    if (uart.getByte(0) != 0x08){
        qDebug() << "Ошибка загрузки основной программы!";
        uart.end();
        return false;
    }

    // шагаем по памяти каждые 256 байт
    int last_progress = 0;
    for (int i = 0; i < (flashParser.getProgram_il() >> 8); i++){
        QByteArray block = flashParser.getProgram_buffer().mid(i << 8, 256);
        block.prepend('P');

        BYTE ks = 0;
        for (int j = 0 + 1;j < 256 + 1; j++){
            ks += block.at(j);
        }

        uart.clearRXBuffer();
        uart.writeRead(block, 1);

        if ((BYTE)uart.getByte(0) != ks){
            qDebug() << "Ошибка загрузки основной программы!";
            uart.end();
            return false;
        }

        int progress = (int)(((double)(i + 1) / (double)(flashParser.getProgram_il() >> 8)) * 100.0);
        if (last_progress != progress && progress % 10 == 0){
            qDebug() << "Прогресс загрузки: " << progress << "%.";
            last_progress = progress;
        }
    }

    qDebug() << "Завершил загрузку основной программы.";
    return true;
}

bool Programmer::flashProgram_verify()
{
    qDebug() << "Начал проверку основной программы.";

    txdbuf.resize(5);
    txdbuf[0] = 'A';
    txdbuf[1] = 0x00;
    txdbuf[2] = 0x00;
    txdbuf[3] = 0x00;
    txdbuf[4] = 0x08;

    uart.clearRXBuffer();
    uart.writeRead(txdbuf, 1);

    if (uart.getByte(0) != 0x08){
        qDebug() << "Ошибка при проверке основной программы!";
        uart.end();
        return false;
    }

    txdbuf.resize(1);
    txdbuf[0] = 'V';
    int last_progress = 0;
    for (int i = 0;i < (flashParser.getProgram_il() >> 8); i++){
        for (int j = 0; j < 32; j++){
            uart.clearRXBuffer();
            uart.writeRead(txdbuf, 8);

            for (int k = 0; k < 8; k++){
                if ((BYTE)uart.getByte(k) != (BYTE)flashParser.getProgram_buffer().at(k + (j << 3) + (i << 8))){
                    qDebug() << "Ошибка при проверке основной программы!";
                    qDebug() << "Адрес:" << QString("0x").append(QString::number(0x08000000 + k + (j << 3) + (i << 8), 16))
                             << ". Записано:" << QString("0x").append(QString::number((BYTE)flashParser.getProgram_buffer().at(k + (j << 3) + (i << 8)), 16))
                             << ". Прочитано:" << QString("0x").append(QString::number((BYTE)ramParser.getProgram_buffer().at(k), 16))
                             << ".";
                    uart.end();
                    return false;
                }
            }
        }

        int progress = (int)(((double)(i + 1) / (double)(flashParser.getProgram_il() >> 8)) * 100.0);
        if (last_progress != progress && progress % 10 == 0){
            qDebug() << "Прогресс проверки: " << progress << "%.";
            last_progress = progress;
        }
    }

    qDebug() << "Завершил проверку основной программы.";
    return true;
}

bool Programmer::flashProgram_run()
{
    qDebug() << "Начал запуск основной программы.";

    uart.writeRead(QByteArray(1, 'R'), 1);

    if	(!uart.getByte('R')){
        qDebug() << "Ошибка при запуске основной программы!";
        uart.end();
        return false;
    }

    qDebug() << "Завершил запуск основной программы.";
    return true;
}

