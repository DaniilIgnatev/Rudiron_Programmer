#ifndef HEXRAMPARSER_HPP
#define HEXRAMPARSER_HPP

#include <QObject>
#include "hexparser.hpp"

class HEXRAMParser : public HEXParser
{
    Q_OBJECT
public:
    explicit HEXRAMParser(QObject *parent = nullptr);

    virtual BOOL initialize(void);

signals:

};

#endif // HEXRAMPARSER_HPP
