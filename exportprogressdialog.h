#ifndef EXPORTPROGRESSDIALOG_H
#define EXPORTPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ExportProgressDialog;
}

class ExportProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportProgressDialog(QWidget *parent = 0);
    ~ExportProgressDialog();
    void setLabelText(QString text);
    void setMaxId(int maxId);
    void setCurrentId(int currentId);
    void reset();

private:
    Ui::ExportProgressDialog *ui;
    int maxId;
    int currentId;

private slots:
    void hide();
};

#endif // EXPORTPROGRESSDIALOG_H
