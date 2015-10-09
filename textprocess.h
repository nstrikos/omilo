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

class MyItem
{
public:
    unsigned int id;
    QString text;
    unsigned int begin;
    unsigned int end;
    QString filename;
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
    QQueue<MyItem> list;

private:
    QString text;
    TextContainer textContainer;    
};

#endif // TEXTPROCESS_H
