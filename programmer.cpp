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
}

void Programmer::start()
{
    auto ports = QSerialPortInfo::availablePorts();
    for (auto &p: ports){
        if (p.productIdentifier() == 60000){
            if (!uart.begin(p)){
                qDebug() << "Ошибка открытия COM порта...";
                return;
            }
            break;
        }
    }

    if (!flashBootloader_sync()){
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

//    if (m_erase.GetCheck() == BST_CHECKED)
//        if(!Erase())
//            return;
    if (optionChecked(ProgrammerOptions::Programm))
        if (!flashProgram_load())
            return;
//    if (m_verify.GetCheck() == BST_CHECKED)
//        if(!Verify())
//            return;

    if (optionChecked(ProgrammerOptions::Run)){
//        str = "Run at 0x08000000...";
//        InsertStrToList();
//        txdbuf[0] = 'R';
//        com.WriteBlock(txdbuf,1);
//        Sleep(1);

//        if	((!com.ReadBlock(rxdbuf,1, true))||(rxdbuf[0]!='R'))
//        {
//            str = "ошибка обмена";
//            InsertStrToList();
//        }
//        else
//        {
//            m_list.DeleteString(m_list.GetCount()-1);
//            str = "Run at 0x08000000 OK!";
//            InsertStrToList();
//        }
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

    if	(received && uart.getByte(0) != 'L')
    {
        qDebug() << "Ошибка загрузки бутлодера в начале!";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    received = uart.writeAndReceive(ramParser.getProgramBuffer_notEmpty(), 1);
    if	(received && uart.getByte(0) !='K')
    {
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

    for (int i = 0; i < (ramParser.getProgram_il() >> 3); i++)
    {
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
        bool received = uart.writeAndReceive(txdbuf, 9);

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
        if( uart.getByte(j) != id_str[j]){
            qDebug() << "Ошибка идентификации загрузчика!";
            uart.end();
            return false;
        }
    }

    qDebug() << "Завершил идентификацию загрузчика.";
    return true;
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

    // шаг в памяти каждые 256 байт
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
    }

    qDebug() << "Завершил загрузку основной программы.";
    return true;
}

