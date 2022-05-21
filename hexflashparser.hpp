#ifndef HEXFLASHPARSER_HPP
#define HEXFLASHPARSER_HPP

#include <QObject>
#include "hexparser.hpp"

class HexFlashParser : public HEXParser
{
    Q_OBJECT
private:

    ///Размер загружаемого массива байт
    size_t ilcod;

    ///Адресное пространство ПЗУ
    BYTE bufcod[0x20000];

public:
    explicit HexFlashParser(QObject *parent = nullptr);

    virtual BOOL initialize(void);

signals:

};

#endif // HEXFLASHPARSER_HPP
