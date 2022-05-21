#ifndef HEXRAMPARSER_HPP
#define HEXRAMPARSER_HPP

#include <QObject>
#include "hexparser.hpp"


class HEXRAMParser : public HEXParser
{
    Q_OBJECT
private:

    size_t dwadrboot;

    size_t ilboot;

    BYTE bufram[0x8000];

public:
    explicit HEXRAMParser(QObject *parent = nullptr);

    BOOL GetBootCod(void);

signals:

};

#endif // HEXRAMPARSER_HPP
