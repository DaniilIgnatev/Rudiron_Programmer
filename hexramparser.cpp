#include "hexramparser.hpp"

HEXRAMParser::HEXRAMParser(QObject *parent)
    : HEXParser{0x8000, parent}
{

}

BOOL HEXRAMParser::initialize(void)
{
    DWORD	dwadr;
    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;

    for (int i = 0; i < bufram.size(); i++){
        bufram[i] = 0xff;
    }

    QFile file(hexFilePath);
    file.open(QIODevice::ReadWrite);

    int nb = 1;
    char chd;

    while (nb == 1){
        int i = 0;
        do{
            nb = file.read(&chd, 1);
            buf_hexstr[i] = chd;
            i++;
        }
        while (chd != '\n');

        if (nb != 1){
            file.close();

            for (i = 0; i < bufram.size(); i++){
                if((unsigned char)bufram.at(i) != 0xff)
                    break;
            }

            dwadrboot = i;
            for (i = (sizeof(bufram) - 1); i >= 0; i--){
                if((unsigned char)bufram.at(i) != 0xff)
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
                bufram[(int)dwadr + i] = buf_data_hex[i];
            }
        }
    }

    return 0;
}
