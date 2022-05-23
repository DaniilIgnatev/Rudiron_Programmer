#include "programmer.hpp"

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

    this->flashBootloader();

    //	if(m_erase.GetCheck()==BST_CHECKED)
    //		if(!Erase())
    //			return;
    //	if(m_program.GetCheck()==BST_CHECKED)
    //		if(!Program())
    //			return;
    //	if(m_verify.GetCheck()==BST_CHECKED)
    //		if(!Verify())
    //			return;
    //	if(fStartRun)
    //	{
    //		str = "Run at 0x08000000...";
    //		InsertStrToList();
    //		txdbuf[0] = 'R';
    //		com.WriteBlock(txdbuf,1);
    //		Sleep(1);

    //		if	((!com.ReadBlock(rxdbuf,1, true))||(rxdbuf[0]!='R'))
    //		{
    //			str = "ошибка обмена";
    //			InsertStrToList();
    //		}
    //		else
    //		{
    //			m_list.DeleteString(m_list.GetCount()-1);
    //			str = "Run at 0x08000000 OK!";
    //			InsertStrToList();
    //		}
    //	}
    //	com.Close();

}

bool Programmer::flashBootloader()
{
    bool success = true;
    success &= flashBootloader_sync();
    success &= flashBootloader_load();
    success &= flashBootloader_verify();
    success &= flashBootloader_run();

    return success;
}

bool Programmer::flashBootloader_sync()
{
    qDebug() << "Начал синхронизацию...";
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
    qDebug() << "Начал загрузку бутлодера...";
    txdbuf.resize(9);
    txdbuf[0] = 'L';
    txdbuf[1] = (ramParser.getProgramm_dwadr() & 0xff);
    txdbuf[2] = (ramParser.getProgramm_dwadr() >> 8) & 0xff;
    txdbuf[3] = 0;
    txdbuf[4] = 0x20;
    txdbuf[5] = ramParser.getProgramm_il() & 0xff;
    txdbuf[6] = (ramParser.getProgramm_il() >> 8) & 0xff;
    txdbuf[7] = 0;
    txdbuf[8] = 0;
    uart.clearRXBuffer();
    uart.write(txdbuf);

    if	(uart.getByte(0) != 'L')
    {
        qDebug() << "Ошибка загрузки бутлодера в начале!";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    uart.write(ramParser.getOccupiedProgrammBuffer());
    if	(uart.getByte(0) !='K')
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
    qDebug() << "Начал верификацию бутлодера...";

    for (int i = 0; i < (ramParser.getProgramm_il() >> 3); i++)
    {
        txdbuf.resize(9);
        txdbuf[0] = 'Y';
        txdbuf[1] = (ramParser.getProgramm_dwadr() + 8 * i) & 0xff;
        txdbuf[2] = ((ramParser.getProgramm_dwadr() + 8 * i) >> 8) & 0xff;
        txdbuf[3] = 0;
        txdbuf[4] = 0x20;
        txdbuf[5] = 8;
        txdbuf[6] = 0;
        txdbuf[7] = 0;
        txdbuf[8] = 0;
        uart.clearRXBuffer();
        uart.write(txdbuf, 30, 9);

        if ((uart.getByte(0) == 'Y') && (uart.getByte(9) == 'K')){
            for (int j = 0; j < 8; j++){
                if ((unsigned char)uart.getByte(j + 1) != (unsigned char)ramParser.getProgramm_buffer().at(ramParser.getProgramm_dwadr() + 8 * i + j)){
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
    qDebug() << "Завершил верификацию бутлодера...";
    return true;
}

bool Programmer::flashBootloader_run()
{
    qDebug() << "Начал запуск бутлодера...";

    txdbuf.resize(5);
    txdbuf[0] = 'R';
    txdbuf[1] = ramParser.getProgramm_dwadr() & 0xff;
    txdbuf[2] = (ramParser.getProgramm_dwadr() >> 8) & 0xff;
    txdbuf[3] = 0;
    txdbuf[4] = 0x20;
    uart.clearRXBuffer();
    uart.write(txdbuf, 30);

    if	(uart.getByte(0) != 'R'){
        qDebug() << "Ошибка запуска бутлодера...";
        uart.end();
        return false;
    }

    uart.clearRXBuffer();
    qDebug() << "Завершил запуск бутлодера...";
    return true;
}

bool Programmer::flashBootloader_identify()
{
//    txdbuf[0] = 'I';
//    com.WriteBlock(txdbuf,1);
//    f = TRUE;

//    if (com.ReadBlock(rxdbuf, 12, true)){
//        for (j = 0; j < 12; j++){
//            if(rxdbuf[j] != id_str[j]){
//                f = FALSE;
//            }
//        }
//    }
//    else{
//        f = FALSE;
//    }

//    if (!f){
//        str = "ошибка идентификации загрузчика!";
//        InsertStrToList();
//        com.Close();
//        return;
//    }

//    m_list.DeleteString(m_list.GetCount() - 1);
//    str = "boot load...ОК!";
//    InsertStrToList();
}

//BOOL Programmer::Program(void)
//{
//int i,j;
//BYTE ks;
//    str.Format("Program %i byte...",ilcod);
//    InsertStrToList();
//    m_progress.SetRange(0,ilcod>>8);
//    m_progress.SetPos(0);
//    txdbuf[0] = 'A';
//    txdbuf[1] = 0x00;
//    txdbuf[2] = 0x00;
//    txdbuf[3] = 0x00;
//    txdbuf[4] = 0x08;
//    com.WriteBlock(txdbuf,5);
//    if((!com.ReadBlock(rxdbuf,1, true))||(rxdbuf[0]!=0x08))
//    {
//        str = "ошибка обмена";
//        InsertStrToList();
//        com.Close();
//        return 0;
//    }
//    txdbuf[0] = 'P';
//    for(i=0;i<(ilcod>>8);i++)
//    {
//        com.WriteBlock(txdbuf,1);
//        com.WriteBlock((LPSTR)(bufcod+(i<<8)),256);
//        ks =0;
//        for(j=0;j<256;j++)
//            ks += bufcod[j+(i<<8)];
//        if((!com.ReadBlock(rxdbuf,1, true))||((BYTE)rxdbuf[0]!=ks))
//        {
//            str = "ошибка обмена";
//            InsertStrToList();
//            com.Close();
//            return 0;
//        }
//        m_progress.SetPos(i+1);
//    }
//    m_list.DeleteString(m_list.GetCount()-1);
//    str.Format("Program %i byte done!",ilcod);
//    InsertStrToList();
//    m_run.EnableWindow(1);
//    return 1;
//}

