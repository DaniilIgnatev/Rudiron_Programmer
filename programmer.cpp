#include "programmer.hpp"

Programmer::Programmer(QObject *parent)
    : QObject{parent}
{
    txdbuf.resize(txdbuf_size);
}

void Programmer::start()
{
    ramParser.setHexFilePath(QDir::currentPath() + "\\1986_BOOT_UART.HEX");

    static const QString EXCHANGE_ERROR = "Ошибка обмена";

    if (!uart.begin("COM8")){
        qDebug() << "Ошибка открытия COM порта...";
        return;
    }

    qDebug() << "Синхронизация...";

    for(int i = 0; i < 512; i++){
        uart.write(0x0);
        uart.readByte();

        if (uart.getByte(0) == 0x0D && uart.getByte(1) == 0x0A && uart.getByte(2) == 0x3E) {
            break;
        }
    }

    if	(!(uart.getByte(0) == 0x0D && uart.getByte(1) == 0x0A && uart.getByte(2) == 0x3E)){
        qDebug() << "Ошибка синхронизации!";
        uart.end();
        return;
    }

    qDebug() << "Синхронизация...ОК!";

    qDebug() << "Начал загрузку бутлодера...";
    txdbuf[0] = 'L';
    txdbuf[1] = (dwadrboot & 0xff);
    txdbuf[2] = (dwadrboot>>8) & 0xff;
    txdbuf[3] = 0;
    txdbuf[4] = 0x20;
    txdbuf[5] = ilboot & 0xff;
    txdbuf[6] = (ilboot>>8) & 0xff;
    txdbuf[7] = 0;
    txdbuf[8] = 0;
    uart.write(txdbuf,9);
    clearTXDBuf();

    if	(uart.getByte(0) != 'L')
    {
        qDebug() << EXCHANGE_ERROR;
        uart.end();
        return;
    }

    uart.write((LPSTR)(bufram+dwadrboot),ilboot);
    if	((!com.ReadBlock(rxdbuf,1, true))||(rxdbuf[0]!='K'))
    {
        str = "ошибка обмена";
        InsertStrToList();
        com.Close();
        return;
    }

//	for(i=0;i<(ilboot>>3);i++)
//	{
//		txdbuf[0] = 'Y';
//		txdbuf[1] = (dwadrboot+8*i) & 0xff;
//		txdbuf[2] = ((dwadrboot+8*i)>>8) & 0xff;
//		txdbuf[3] = 0;
//		txdbuf[4] = 0x20;
//		txdbuf[5] = 8;
//		txdbuf[6] = 0;
//		txdbuf[7] = 0;
//		txdbuf[8] = 0;
//		com.WriteBlock(txdbuf,9);
//		f = TRUE;
//		Sleep(1);

//		if((com.ReadBlock(rxdbuf,10, true))&&(rxdbuf[0]=='Y')&&(rxdbuf[9]=='K'))
//		{
//			for(j=0;j<8;j++)
//			{
//				if(rxdbuf[j+1] != (char)bufram[dwadrboot+8*i+j])
//					f= FALSE;
//			}
//		}
//		else
//			f= FALSE;
//		if(!f)
//		{
//			str = "ошибка обмена";
//			InsertStrToList();
//			com.Close();
//			return;
//		}
//	}
//	txdbuf[0] = 'R';
//	txdbuf[1] = dwadrboot & 0xff;
//	txdbuf[2] = (dwadrboot>>8) & 0xff;
//	txdbuf[3] = 0;
//	txdbuf[4] = 0x20;
//	com.WriteBlock(txdbuf,5);
//	Sleep(1);

//	if	((!com.ReadBlock(rxdbuf,1, true))||(rxdbuf[0]!='R'))
//	{
//		str = "ошибка обмена";
//		InsertStrToList();
//		com.Close();
//		return;
//	}
//	txdbuf[0] = 'I';
//	com.WriteBlock(txdbuf,1);
//	f = TRUE;
//	Sleep(1);

//	if(com.ReadBlock(rxdbuf,12, true))
//	{
//		for(j=0;j<12;j++)
//			{
//				if(rxdbuf[j] != id_str[j])
//					f= FALSE;
//			}
//	}
//	else
//		f = FALSE;
//	if(!f)
//	{
//		str = "ошибка идентификации загрузчика!";
//		InsertStrToList();
//		com.Close();
//		return;
//	}
//	m_list.DeleteString(m_list.GetCount()-1);
//	str = "boot load...ОК!";
//	InsertStrToList();

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

void Programmer::clearTXDBuf()
{
    for (int i = 0; i < txdbuf_size; i++){
        if (txdbuf[i] == 0){
            break;
        }

        txdbuf[i] = 0;
    }
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

