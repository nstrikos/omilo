#ifndef TEXTPROCESS_H
#define TEXTPROCESS_H

#include <QString>
#include <QStringList>
#include <QQueue>

class TextContainer
{
public:
    TextContainer();
    ~TextContainer();
    void clear();
    QQueue<QString> text;
    QQueue<unsigned int> begin;
    QQueue<unsigned int> end;
};


class TextProcess
{
public:
    TextProcess();
    ~TextProcess();
    void setText(QString text);
    void processText();
    TextContainer getTextContainer();

private:
    QString text;
    TextContainer textContainer;
};

#endif // TEXTPROCESS_H
