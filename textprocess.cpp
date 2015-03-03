#include "textprocess.h"

TextContainer::TextContainer()
{

}

TextContainer::~TextContainer()
{
    text.clear();
    begin.clear();
    end.clear();
    googleText.clear();
    googleBegin.clear();
    googleEnd.clear();
}

void TextContainer::clear()
{
    text.clear();
    begin.clear();
    end.clear();
    googleText.clear();
    googleBegin.clear();
    googleEnd.clear();
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
    QQueue<unsigned int> queue;
    int size = text.size();
    int i = 0;
    int table[3];
    while ( i < size)
    {
        QString ch = text.at(i);
        if ( ch == "." )//|| ch == ",")
        {
            if ( i < size - 1)
            {
                QString nextChar = text.at(i+1);
                if (nextChar == " " || nextChar == "\t" || nextChar == "\n")
                {
                    queue.append(i);
                    //textContainer.end.append(i);
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
    while(queue.size() > 0)
    {
        int n = queue.dequeue();
        QString line = text.mid(m, n - m + 1);
        textContainer.begin.append(m);
        textContainer.end.append(n);
        //line = line.trimmed();
        textContainer.text.append(line);
        m = n + 1;
    }

//    for ( int i = 0; i < textContainer.text.size(); i++)
//    {
//        QString string = textContainer.text.at(i);
//        QString segment;
//        if (string.size() <= 100)
//            textContainer.googleText.append( textContainer.text.at(i) );
//        else
//        {
//            bool done = false;
//            while (!done)
//            {
//                int n = string.indexOf(" ");
//                if ( n >= 0 )
//                {
//                    segment = segment + string.left(n + 1);
//                    string = string.right(string.size() - n - 1);
//                }
//                else
//                {
//                    segment  = string.left(100);
//                    if (string.size() >= 100)
//                        string = string.right(string.size() - 100);
//                    else
//                        string = "";
//                    //continue;
//                }
//                int m = string.indexOf(" ");
//                QString testString = string.left(m);
//                if (segment.size() + testString.size() > 100)
//                {
//                    textContainer.googleText.append(segment);
//                    segment = "";
//                }
//                if (string.size() == 0)
//                {
//                    textContainer.googleText.append(segment);
//                    done = true;
//                }
//            }
//        }
//    }
    textContainer.googleText.clear();
    textContainer.googleEnd.clear();
    int position = 0;
    //while (!tempList.isEmpty())
    for (int i = 0; i < textContainer.text.size(); i++)
    {
        //QString text2 = tempList.takeFirst();
        QString text2 = textContainer.text.at(i);
        if (text2.size() > 100)
        {
            bool done = false;
            while (!done)
            {
                QString segment = text2.left(100);
                text2 = text2.right(text2.size() - 100);
                int space = segment.lastIndexOf(" ");
                QString remain = segment.right(segment.size() - space);
                segment = segment.left(space);
                text2 = remain + text2;
                //stringList << segment;
                textContainer.googleText.append(segment);
                textContainer.googleBegin.append(position);
                position = position + segment.size();
                textContainer.googleEnd.append(position);
                //position++;
                if (text2.size() < 100)
                {
                    //stringList << text2;
                    textContainer.googleText.append(text2);
                    //textContainer.begin.insert(position, 0);
                    textContainer.googleBegin.append(position);
                    position = position + text2.size();
                    textContainer.googleEnd.append(position);
                    done = true;
                }
            }
        }
        else
        {
            //stringList << text2;
            textContainer.googleBegin.append(position);
            textContainer.googleText.append(text2);
            position = position + text2.size();
            textContainer.googleEnd.append(position);
//            textContainer.begin.insert(i + 1, 0);
//            textContainer.end.insert(i + 1, 0);
        }
    }

    for (int i = 0; i < textContainer.text.size(); i++)
    {
        qDebug() << "Text:" << textContainer.text.at(i);
    }
    for (int i = 0; i < textContainer.begin.size(); i++)
    {
        qDebug() << "Begin:" << textContainer.begin.at(i);
    }
    for (int i = 0; i < textContainer.googleEnd.size(); i++)
    {
        qDebug() << "End:" << textContainer.googleEnd.at(i);
    }
    for (int i = 0; i < textContainer.googleText.size(); i++)
    {
        qDebug() << "GoogleText:" << textContainer.googleText.at(i);
    }

}

