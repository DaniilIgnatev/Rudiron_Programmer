#ifndef HEXFLASHPARSER_HPP
#define HEXFLASHPARSER_HPP

#include <QObject>

class HexFlashParser : public QObject
{
    Q_OBJECT
public:
    explicit HexFlashParser(QObject *parent = nullptr);

signals:

};

#endif // HEXFLASHPARSER_HPP
