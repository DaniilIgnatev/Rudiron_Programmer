#include "hexramparser.hpp"

HEXRAMParser::HEXRAMParser(QObject *parent)
    : HEXParser{parent}
{

}

BOOL HEXRAMParser::GetBootCod(void)
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
