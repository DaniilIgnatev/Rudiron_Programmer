#ifndef HEXPARSER_HPP
#define HEXPARSER_HPP

#include <QObject>
#include "TypeDefs.hpp"

class HEXParser : public QObject
{
    Q_OBJECT
public:
    explicit HEXParser(size_t buf_programm_size, QObject *parent = nullptr);

protected:

    BYTE buf_hexstr[530];

    BYTE btype_hex;

    BYTE bl_hex;

    WORD wadr_offs_hex;

    BYTE buf_data_hex[256];

    DWORD dwadr_seg_hex, dwadr_lineoffs_hex;

protected:
    QString hexFilePath;
public:
    const QString &getHexFilePath() const;
    void setHexFilePath(const QString &newHexFilePath);

public:

    BOOL GetDataHex(void);

    BYTE GetHexByte(size_t ibuf);

protected:
    ///Адрес начала загружаемого массива байт
    size_t dwadrboot;

    ///Размер загружаемого массива байт
    size_t ilboot;

    ///Размер адресного пространства
    size_t buf_programm_size;

    ///Адресное пространство
    QByteArray bufram;

public:
    virtual BOOL initialize(void);

signals:

};

#endif // HEXPARSER_HPP
