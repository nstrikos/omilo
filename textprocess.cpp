#include "textprocess.h"

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
    this->text = text; //+ ". .";
}

QStringList TextProcess::getTextList()
{
    return textList;
}

TextContainer TextProcess::getTextContainer()
{
    return textContainer;
}

//void TextProcess::processText()
//{
//   bool done = false;

//   if (textList.size() > 0)
//       textList.clear();

//   while (!done)
//   {
//       int m = text.indexOf(".");
//       if (m > 0)
//       {
//           QString line = text.left(m + 1);
//           line = line.trimmed();
//           textList << line;
//           qDebug() << line;
//           text = text.right(text.size() - m - 1);
//       }
//       else
//           done = true;
//   }

//}

//void TextProcess::processText()
//{
//    qDebug() << text;
//    if (textList.size() > 0)
//        textList.clear();

//    int size = text.size();
//    int m = 0;
//    //for (int i = 0; i < size ; i++)
//    int i = 0;
//    while (i < size)
//    {
//        QString ch = text.at(i);
//        if (ch == ".")
//        {
//            if ( i < size - 2)
//            {
//                QString nextChar = text.at(i + 1);
//                if (nextChar == " ")
//                {
//                    QString line = text.mid(m, i - m + 1);
//                    m = i + 1;
//                    line = line.trimmed();
//                    textList << line;
//                    qDebug() << "Line:" << line;
//                    i++;
//                }
//                else
//                {
//                    int n = text.indexOf(" ", i);
//                    QString line = text.mid(m, n - m + 1);
//                    textList << line;
//                    qDebug() << "Line:" << line;
//                    m = n + 1;
//                    i = n + 1;
//                }
//            }
//            else
//                i++;
//        }
//        else if (  i == size - 1)
//        {
//            QString line = text.mid(m, i - m + 1);
//            m = i + 1;
//            textList << line;
//            qDebug() << "Line:" << line;
//            i++;
//        }
//        else
//            i++;
//    }
//}

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

//    textContainer.begin.append(0);
    for (int n = 0; n < textContainer.end.size() - 1; n++)
    {
//        qDebug() << textContainer.end.at(n);
        int begin = textContainer.end.at(n) + 1;
//        textContainer.begin.append(begin);
    }

//    for (int n = 0; n < textContainer.begin.size(); n++)
//    {
//        qDebug() << textContainer.begin.at(n);
//        qDebug() << textContainer.end.at(n);
//    }
//    qDebug() << textContainer.begin.size();
//    qDebug() << textContainer.end.size();

    int m = 0;
    if (textList.size() > 0)
        textList.clear();
    while(queue.size() > 0)
    {
        int n = queue.dequeue();
        QString line = text.mid(m, n - m + 1);
        textContainer.begin.append(m);
        textContainer.end.append(n);
        line = line.trimmed();
        textList << line;
        textContainer.text.append(line);
        //qDebug() << line;
        m = n + 1;
    }
}

