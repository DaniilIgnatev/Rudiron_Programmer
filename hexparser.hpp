#ifndef HEXPARSER_HPP
#define HEXPARSER_HPP

#include <QObject>
#include "TypeDefs.hpp"

class HEXParser : public QObject
{
    Q_OBJECT
public:
    explicit HEXParser(int buf_programm_size, QObject *parent = nullptr);

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
    void setHexPath(const QString &newHexFilePath);

public:

    virtual BOOL initialize(void) = 0;

    BOOL GetDataHex(void);

    BYTE GetHexByte(size_t ibuf);

protected:
    ///Адрес смещения в буффере загружаемого массива байт
    int programm_dwadr;

    ///Размер загружаемого массива байт
    int programm_il;

    ///Размер адресного пространства
    int programm_buffer_size;

    ///Адресное пространство
    QByteArray programm_buffer;

public:

    ///Выделяет область используемых программой в буфере байт
    QByteArray getOccupiedProgrammBuffer();

    int getProgramm_dwadr() const;

    int getProgramm_il() const;

    int getProgramm_buffer_size() const;

    const QByteArray &getProgramm_buffer() const;

signals:

};

#endif // HEXPARSER_HPP
