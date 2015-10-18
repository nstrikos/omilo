#ifndef DISPLAYMESSAGEDIALOG_H
#define DISPLAYMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class DisplayMessageDialog;
}

class DisplayMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayMessageDialog(QWidget *parent = 0);
    ~DisplayMessageDialog();
    void setText(QString text);

private:
    Ui::DisplayMessageDialog *ui;

private slots:
    void hide();
};

#endif // DISPLAYMESSAGEDIALOG_H
