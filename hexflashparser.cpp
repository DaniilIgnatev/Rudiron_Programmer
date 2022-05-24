#include "hexflashparser.hpp"

HexFlashParser::HexFlashParser(QObject *parent)
    : HEXParser{0x20000, parent}
{

}

BOOL HexFlashParser::initialize(void)
{
    int i;
    int nb;
    QFile file(hexFilePath);
    char chd;
    DWORD	dwadr;

    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;

    for (int i = 0; i < program_buffer.size(); i++){
        program_buffer[i] = BYTE_MAX;
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

            for (i = 0; i < program_buffer.size(); i++){
                if((unsigned char)program_buffer.at(i) != BYTE_MAX)
                    break;
            }
            program_dwadr = i;

            for (i = (program_buffer.size() - 1); i >= 0; i--){
                if ((unsigned char)program_buffer.at(i) != BYTE_MAX){
                    break;
                }
            }

            i = ((i + 0x100) & 0xffffff00);
            program_il = i;
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
                program_buffer[(int)dwadr + i] = buf_data_hex[i];
            }
        }
    }

    return 0;
}
