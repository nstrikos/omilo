#include "textprocess.h"
#include <QDebug>

TextContainer::TextContainer()
{

}

TextContainer::~TextContainer()
{
    text.clear();
    begin.clear();
    end.clear();
}

void TextContainer::clear()
{
    text.clear();
    begin.clear();
    end.clear();
}

TextProcess::TextProcess()
{

}

TextProcess::~TextProcess()
{

}

void TextProcess::setText(QString text)
{
    this->text = text + ".";
}

TextContainer TextProcess::getTextContainer()
{
    return textContainer;
}

void TextProcess::processText()
{
    textContainer.clear();
    list.clear();
    QQueue<unsigned int> queue;
    int size = text.size();
    int i = 0;
    int table[3];
    while ( i < size)
    {
        QString ch = text.at(i);
        if  ( ( ch == "." )   ||  (ch == "\n") )
        {
            if ( i < size - 1)
            {
                QString nextChar = text.at(i+1);
                if  ( (ch == ".") && (nextChar == " " || nextChar == "\t" || nextChar == "\n") )
                {
                    queue.append(i);
                    //textContainer.end.append(i);
                    i++;
                }
                else if ( ch == "\n")
                {
                    queue.append(i);
                    i++;
                }
                else
                {
                    int min = 0;
                    table[0] = text.indexOf(" ", i);
                    table[1] = text.indexOf("\t", i);
                    table[2] = text.indexOf("\n", i);
                    for (int ii = 0; ii < 3; ii++)
                    {
                        if (table[ii] > 0)
                            min = table[ii];
                    }
                    for (int ii = 0; ii < 3; ii++)
                    {
                        if (table[ii] > 0 )
                        {
                            if (table[ii] < min)
                                min = table[ii];
                        }
                    }
                    if ( min > 0)
                    {
                        queue.append(min);
                        //textContainer.end.append(i);
                        i = min;
                    }
                    else
                        i++;
                }
            }
            else if ( i == size - 1)
            {
                queue.append(i);
                //textContainer.end.append(i);
                i++;
            }
            else
                i++;
        }
        else
            i++;
    }

    int m = 0;
//    if (textList.size() > 0)
//        textList.clear();
    int counter = 0;
    while(queue.size() > 0)
    {
        int n = queue.dequeue();
        QString line = text.mid(m, n - m + 1);
        textContainer.begin.append(m);
        textContainer.end.append(n);
        //line = line.trimmed();
        textContainer.text.append(line);
        MyItem item;
        item.id = counter;
        item.text = line;
        item.begin = m;
        item.end = n;
        QString number = QString("%1").arg(counter, 6, 10, QLatin1Char('0'));
        item.filename = "/tmp/omilo-" + number + ".wav";
        qDebug() << item.id;
        qDebug() << item.text;
        qDebug() << item.filename;
        list.append(item);
        counter++;
        m = n + 1;
    }
}
