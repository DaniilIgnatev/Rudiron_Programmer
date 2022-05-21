#ifndef HEXFLASHPARSER_HPP
#define HEXFLASHPARSER_HPP

#include <QObject>
#include "hexparser.hpp"

class HexFlashParser : public HEXParser
{
    Q_OBJECT

public:
    explicit HexFlashParser(QObject *parent = nullptr);

    virtual BOOL initialize(void);

signals:

};

#endif // HEXFLASHPARSER_HPP
