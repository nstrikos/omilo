#ifndef TEXTPROCESS_H
#define TEXTPROCESS_H

#include <QString>
#include <QQueue>
#include "definitions.h"

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



//class MyList
//{
//public:
//    QQueue<MyItem> items;
//};


class TextProcess
{
public:
    TextProcess();
    ~TextProcess();
    void setText(QString text);
    void processText();
    TextContainer getTextContainer();
    QQueue<TextProcessItem> list;

private:
    QString text;
    TextContainer textContainer;    
};

#endif // TEXTPROCESS_H
