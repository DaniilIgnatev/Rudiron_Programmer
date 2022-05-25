#include "programmer.hpp"

ProgrammerOptions Programmer::getOptions() const
{
    return options;
}

bool Programmer::optionChecked(ProgrammerOptions option)
{
    return (bool)((int)options & (int)option);
}

void Programmer::setOptions(ProgrammerOptions newOptions)
{
    options = newOptions;
}

void Programmer::checkOption(ProgrammerOptions option)
{
    this->options = (ProgrammerOptions)((int)options | (int)option);
}

Programmer::Programmer(QObject *parent)
    : QObject{parent}
{
    ramParser.setHexPath(this->bootloader_path);
    ramParser.initialize();

    flashParser.setHexPath(this->programm_path);
    flashParser.initialize();

//    this->checkOption(ProgrammerOptions::Erase);
    this->checkOption(ProgrammerOptions::Programm);
    this->checkOption(ProgrammerOptions::Verify);
    this->checkOption(ProgrammerOptions::Run);
}

void Programmer::start()
{
    auto ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;

    for (auto &p: ports){
        if (p.productIdentifier() == 60000){
            port = p;
            break;
        }
    }

    if (port.isNull() || !uart.begin(port)){
        qDebug() << "Ошибка открытия COM порта...";
        return;
    }

    if (!flashBootloader_sync()){
        return;
    }
    if (!flashBootloader_switchSpeed()){
        return;
    }
    if (!flashBootloader_load()){
        return;
    }
    if (!flashBootloader_verify()){
        return;
    }
    if (!flashBootloader_run()){
        return;
    }
    if (!flashBootloader_identify()){
        return;
    }

    if (optionChecked(ProgrammerOptions::Erase)){
        if (!flashProgram_erase()){
            return;
        }
    }
    if (optionChecked(ProgrammerOptions::Programm)){
        if (!flashProgram_load()){
            return;
        }
    }
    if (optionChecked(ProgrammerOptions::Verify)){
        if (!flashProgram_verify()){
            return;
        }
    }
    if (optionChecked(ProgrammerOptions::Run)){
        if (!flashProgram_run()){
            return;
        }
    }

    uart.end();
}

bool Programmer::flashBootloader_sync()
{
    qDebug() << "Начал синхронизацию.";
    uart.clearRXBuffer();

    for(int i = 0; i < 512; i++){
        uart.write(0x0);

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
    qDebug() << "Начал установку скорости обмена 115200 бод.";

    txdbuf.resize(5);
    txdbuf[0] = 'B';
    txdbuf[1] = 0x00;
    txdbuf[2] = (char)0xc2;
    txdbuf[3] = 0x01;
    txdbuf[4] = 0x00;
//    txdbuf[1] = 0x00;
//    txdbuf[2] = (char)0x84;
//    txdbuf[3] = 0x03;
//    txdbuf[4] = 0x00;

    uart.clearRXBuffer();
    uart.writeAndReceive(txdbuf, 1);

    uart.setBaudRate(115200 * 1);

    txdbuf.resize(1);
    txdbuf[0] = 0xd;
    uart.clearRXBuffer();
    uart.writeAndReceive(txdbuf, 3);

    if	(uart.getByte(0) != 0xd || uart.getByte(1) != 0xa || uart.getByte(2) != 0x3e){
        qDebug() << "Ошибка установки скорости обмена 115200 бод!";
        uart.end();
        return false;
    }

    qDebug() << "Закончил установку скорости обмена 115200 бод.";
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
    bool received = uart.writeAndReceive(txdbuf, 1);

    if	(received && uart.getByte(0) != 'L'){
        qDebug() << "Ошибка загрузки бутлодера в начале!";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    received = uart.writeAndReceive(ramParser.getProgramBuffer_notEmpty(), 1);
    if	(received && uart.getByte(0) !='K'){
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
        bool received = uart.writeAndReceive(txdbuf, 10);

        if (received && (uart.getByte(0) == 'Y') && (uart.getByte(9) == 'K')){
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
    bool received = uart.writeAndReceive(txdbuf, 1);

    if	(received && uart.getByte(0) != 'R'){
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
    bool received = uart.writeAndReceive(txdbuf, 12);

    if (!received){
        qDebug() << "Ошибка идентификации загрузчика!";
        uart.end();
        return false;
    }

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
    bool received = uart.writeAndReceive(txdbuf, 9);

    if	(!received || uart.getByte(0) != 'E'){
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
    bool received = uart.writeAndReceive(txdbuf, 1);

    if (!received || uart.getByte(0) != 0x08){
        qDebug() << "Ошибка загрузки основной программы!";
        uart.end();
        return false;
    }

    // шагаем по памяти каждые 256 байт
    for (int i = 0; i < (flashParser.getProgram_il() >> 8); i++){
        uart.write('P');

        QByteArray block = flashParser.getProgram_buffer().mid(i << 8, 256);

        BYTE ks = 0;
        for (int j = 0;j < 256; j++){
            ks += block.at(j);
        }

        uart.clearRXBuffer();
        received = uart.writeAndReceive(block, 1);

        if (!received || (BYTE)uart.getByte(0) != ks){
            qDebug() << "Ошибка загрузки основной программы!";
            uart.end();
            return false;
        }

        qDebug() << "Прогресс загрузки: " << (int)(((double)(i + 1) / (double)(flashParser.getProgram_il() >> 8)) * 100.0) << "%.";
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
    bool received = uart.writeAndReceive(txdbuf, 1);

    if (!received || uart.getByte(0) != 0x08){
        qDebug() << "Ошибка при проверке основной программы!";
        uart.end();
        return false;
    }

    txdbuf.resize(1);
    txdbuf[0] = 'V';
    for (int i = 0;i < (flashParser.getProgram_il() >> 8); i++){
        for (int j = 0; j < 32; j++){
            uart.clearRXBuffer();
            received = uart.writeAndReceive(txdbuf, 8);
            if (!received){
                qDebug() << "Ошибка при проверке основной программы!";
                uart.end();
                return false;
            }

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
        qDebug() << "Прогресс проверки: " << (int)(((double)(i + 1) / (double)(flashParser.getProgram_il() >> 8)) * 100.0) << "%.";
    }

    qDebug() << "Завершил проверку основной программы.";
    return true;
}

bool Programmer::flashProgram_run()
{
    qDebug() << "Начал запуск основной программы.";

    uart.write('R');

    if	(!uart.getByte('R')){
        qDebug() << "Ошибка при запуске основной программы!";
        uart.end();
        return false;
    }

    qDebug() << "Завершил запуск основной программы.";
    return true;
}

