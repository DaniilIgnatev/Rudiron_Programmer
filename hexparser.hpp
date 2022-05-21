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

    virtual BOOL initialize(void);

    BOOL GetDataHex(void);

    BYTE GetHexByte(size_t ibuf);

protected:
    ///Адрес начала загружаемого массива байт
    size_t programm_dwadr;

    ///Размер загружаемого массива байт
    size_t programm_il;

    ///Размер адресного пространства
    size_t programm_buffer_size;

    ///Адресное пространство
    QByteArray programm_buffer;

public:

    size_t getProgramm_dwadr() const;

    size_t getProgramm_il() const;

    size_t getProgramm_buffer_size() const;

    const QByteArray &getProgramm_buffer() const;

signals:

};

#endif // HEXPARSER_HPP
