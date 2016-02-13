#ifndef CUSTOMFESTIVALDIALOG_H
#define CUSTOMFESTIVALDIALOG_H

#include <QDialog>

namespace Ui {
class CustomFestivalDialog;
}

class CustomFestivalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomFestivalDialog(QString *command, QString *arguments, QWidget *parent = 0);
    ~CustomFestivalDialog();
    void reject();
    void showDialog();
//    void setCustomFestivalCommand(QString command);
//    void setCustomFestivalCommandArguments(QString arguments);

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::CustomFestivalDialog *ui;
    QString *customFestivalCommand;
    QString *customFestivalCommandArguments;
    QString tmpCommand;
    QString tmpArguments;
};

#endif // CUSTOMFESTIVALDIALOG_H
