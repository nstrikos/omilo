#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QPlainTextEdit>

// Problem: I have several identical widgets, and I want to know
// which one of them send the signal
// Solution 1: Subclass QPlainText. This solution is implemented here
// Solution 2: Use QSignalMapper. See https://artandlogic.com/2013/09/qt-5-and-c11-lambdas-are-your-friend/
// to the problem of sending signals fro identical widgets


class CustomTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = 0);

signals:
    void customTextChanged(QString name, QString text);

private slots:
    void handleTextChanged();

public slots:
};

#endif // CUSTOMTEXTEDIT_H
