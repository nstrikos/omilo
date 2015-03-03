#ifndef TEXTPROCESS_H
#define TEXTPROCESS_H

#include <QString>
#include <QStringList>
#include <QQueue>
#include <QDebug>

class TextContainer
{
public:
    TextContainer();
    ~TextContainer();
    void clear();
    QQueue<QString> text;
    QQueue<unsigned int> begin;
    QQueue<unsigned int> end;
    QQueue<unsigned int> googleBegin;
    QQueue<unsigned int> googleEnd;
    QQueue<QString> googleText;
};


class TextProcess
{
public:
    TextProcess();
    ~TextProcess();
    void setText(QString text);
    void processText();
    //QStringList getTextList();
    TextContainer getTextContainer();

private:
    QString text;
//    QStringList textList;
    TextContainer textContainer;
};

#endif // TEXTPROCESS_H
