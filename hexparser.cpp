#include "hexparser.hpp"

HEXParser::HEXParser(QString hexFilePath, QObject *parent)
    : QObject{parent}
{
    this->hexFilePath = hexFilePath;
}


BOOL HEXParser::GetBootCod(void)
{
    DWORD	dwadr;
    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;

    for (size_t i = 0; i < sizeof(bufram); i++){
        bufram[i] = 0xff;
    }

    QFile file(hexFilePath);
    file.open(QIODevice::ReadWrite);

    int nb = 1;
    char chd;

    while (nb == 1){
        size_t i = 0;
        do{
            nb = file.read(&chd, 1);
            buf_hexstr[i] = chd;
            i++;
        }
        while (chd != '\n');

        if (nb != 1){
            file.close();

            for (i = 0; i < sizeof(bufram); i++){
                if(bufram[i] != 0xff)
                    break;
            }

            dwadrboot = i;
            for (i = (sizeof(bufram) - 1); i >= 0; i--){
                if(bufram[i] != 0xff)
                    break;
            }

            ilboot = (i + 8 - dwadrboot) & 0xfffffff8;
            return 1;
        }

        if (!GetDataHex()){
            file.close();
            return 0;
        }

        if (btype_hex == 0){
            dwadr = dwadr_lineoffs_hex + dwadr_seg_hex + wadr_offs_hex;
            if ((dwadr < 0x20000000) || ((dwadr + bl_hex) > 0x20008000)){
                file.close();
                return 0;
            }

            dwadr -= 0x20000000;
            for (i = 0; i < bl_hex; i++){
                bufram[dwadr + i] = buf_data_hex[i];
            }
        }
    }

    return 0;
}

BOOL HEXParser::GetHexCod(QString strfn)
{
    size_t i;
    int nb;
    QFile file(strfn);
    char chd;
    DWORD	dwadr;
    /*
    for(i=0;i<sizeof(bufcod);i++)
    {
        bufcod[i] = i;
    }
    ilcod = 512*0x100;
    return 1;
*/
    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;
    for (i = 0; i < sizeof(bufcod); i++){
        bufcod[i] = 0xff;
    }

    if (!file.open(QIODevice::ReadWrite)){
        return 0;
    }

    nb = 1;
    while (nb == 1){
        i = 0;
        do{
            nb = file.read(&chd, 1);
            buf_hexstr[i] = chd;
            i++;
        }
        while (chd != '\n');

        if (nb != 1){
            file.close();

            for (i = (sizeof(bufcod) - 1); i >= 0; i--){
                if (bufcod[i] != 0xff){
                    break;
                }
            }

            i = ((i + 0x100) & 0xffffff00);
            ilcod = i;
            return 1;
        }

        if (!GetDataHex()){
            file.close();
            return 0;
        }

        if (btype_hex == 0){
            dwadr = dwadr_lineoffs_hex + dwadr_seg_hex + wadr_offs_hex;
            if ((dwadr < 0x08000000) || ((dwadr + bl_hex) > 0x08020000)){
                file.close();
                return 0;
            }

            dwadr -= 0x08000000;
            for (i = 0;i < bl_hex; i++){
                bufcod[dwadr+i] = buf_data_hex[i];
            }
        }
    }

    return 0;
}

BOOL HEXParser::GetDataHex(void)
{
    size_t i;
    BYTE ks;

    if (buf_hexstr[0] != 0x3a){
        return	0;
    }

    bl_hex = GetHexByte(1);
    wadr_offs_hex = ((WORD)GetHexByte(3) << 8) + GetHexByte(5);
    btype_hex = GetHexByte(7);
    ks = bl_hex + btype_hex + (wadr_offs_hex >> 8) + wadr_offs_hex;

    for (i = 0; i < bl_hex + 1; i++)
    {
        buf_data_hex[i] = GetHexByte(2 * i + 9);
        ks += buf_data_hex[i];
    }

    if (ks != 0){
        return 0;
    }

    if (btype_hex == 2){
        dwadr_seg_hex =	(((WORD)GetHexByte(9) << 8) + GetHexByte(11)) << 4;
    }

    if (btype_hex == 4){
        dwadr_lineoffs_hex = (((WORD)GetHexByte(9) << 8) + GetHexByte(11)) << 16;
    }

    return 1;
}

BYTE HEXParser::GetHexByte(size_t ibuf)
{
    BYTE bh, bl;
    bh = buf_hexstr[ibuf] - 0x30;

    if (bh > 9){
        bh -= 7;
    }

    bh <<= 4;
    bl = buf_hexstr[ibuf + 1] - 0x30;

    if (bl > 9){
        bl -= 7;
    }

    return bh + bl;
}
