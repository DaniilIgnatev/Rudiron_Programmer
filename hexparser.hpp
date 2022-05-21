#ifndef HEXPARSER_HPP
#define HEXPARSER_HPP

#include <QObject>
#include "TypeDefs.hpp"


class HEXParser : public QObject
{
    Q_OBJECT
private:

    BYTE bufcod[0x20000];

    size_t ilcod;

protected:

    QString hexFilePath;

    BYTE bl_hex;

    WORD wadr_offs_hex;

    BYTE buf_data_hex[256];

    DWORD dwadr_seg_hex, dwadr_lineoffs_hex;

    BYTE buf_hexstr[530];

    BYTE btype_hex;

public:

    explicit HEXParser(QObject *parent = nullptr);

    BOOL GetHexCod(QString strfn);

    BOOL GetDataHex(void);

    BYTE GetHexByte(size_t ibuf);

    const QString &getHexFilePath() const;
    void setHexFilePath(const QString &newHexFilePath);

signals:

};

#endif // HEXPARSER_HPP
