#include "hexparser.hpp"

HEXParser::HEXParser(size_t buf_programm_size, QObject *parent)
    : QObject{parent}
{
    this->programm_buffer_size = buf_programm_size;
    this->programm_buffer.resize(this->programm_buffer_size);
}

const QString &HEXParser::getHexFilePath() const
{
    return hexFilePath;
}

void HEXParser::setHexFilePath(const QString &newHexFilePath)
{
    hexFilePath = newHexFilePath;
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

const QByteArray &HEXParser::getProgramm_buffer() const
{
    return programm_buffer;
}

size_t HEXParser::getProgramm_buffer_size() const
{
    return programm_buffer_size;
}

size_t HEXParser::getProgramm_il() const
{
    return programm_il;
}

size_t HEXParser::getProgramm_dwadr() const
{
    return programm_dwadr;
}
