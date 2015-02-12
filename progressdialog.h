#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTimer>
#include <QFile>
#include <QKeyEvent>
#include "ui_progressdialog.h"

class progressDialog : public QDialog, public Ui::progressDialog
{
    Q_OBJECT
    
public:
    progressDialog(QProcess *process, QString filename, QWidget *parent = 0);
    void updateProgressDialog(QProcess *process, QString filename);
    void stopTimer();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_cancelButton_clicked();
    void checkDownloadFileSize();

private:
    QProcess *downloadProcess;
    QString downloadFile;
    int maxSize;
    void updateMaxSize();
    QTimer timer;
};

#endif // PROGRESSDIALOG_H
