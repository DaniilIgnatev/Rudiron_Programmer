#include "hexflashparser.hpp"

HexFlashParser::HexFlashParser(QObject *parent)
    : HEXParser{parent}
{

}

BOOL HexFlashParser::initialize(void)
{
    size_t i;
    int nb;
    QFile file(hexFilePath);
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
