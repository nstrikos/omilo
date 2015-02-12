#include "progressdialog.h"
#include "ui_progressdialog.h"

progressDialog::progressDialog(QProcess *process, QString filename, QWidget *parent) :QDialog(parent)
{
    setupUi(this);
    this->downloadProcess = process;
    this->downloadFile = filename;
    progressBar->setValue(0);
    updateMaxSize();
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkDownloadFileSize()));
    timer.start(5000);
}

void progressDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void progressDialog::updateProgressDialog(QProcess *process, QString filename)
{
    this->downloadProcess = process;
    this->downloadFile = filename;
    this->progressBar->setValue(0);
    timer.start(5000);
    updateMaxSize();
}

void progressDialog::on_cancelButton_clicked()
{
    QFile::remove(this->downloadFile);
    downloadProcess->close();
    timer.stop();
}

void progressDialog::updateMaxSize()
{
    if (downloadFile == "/tmp/cmu-us-awb-arctic-voice_0.95-1_all.deb")
        maxSize = 148;
    else if (downloadFile == "/tmp/cmu-us-bdl-arctic-voice_0.95-1_all.deb")
        maxSize = 106;
    else if (downloadFile == "/tmp/cmu-us-clb-arctic-voice_0.95-1_all.deb")
        maxSize = 145;
    else if (downloadFile == "/tmp/cmu-us-jmk-arctic-voice_0.95-1_all.deb")
        maxSize = 109;
    else if (downloadFile == "/tmp/cmu-us-ksp-arctic-voice_0.95-1_all.deb")
        maxSize = 136;
    else if (downloadFile == "/tmp/cmu-us-rms-arctic-voice_0.95-1_all.deb")
        maxSize = 134;
    else if (downloadFile == "/tmp/cmu-us-slt-arctic-voice_0.95-1_all.deb")
        maxSize = 132;
}

void progressDialog::checkDownloadFileSize()
{
    int size = 0;
    QFile myFile(downloadFile);
    if (myFile.open(QIODevice::ReadOnly)){
        size = myFile.size();  //when file does open.
        myFile.close();
    }
    float sizeInMegabytes = size / 1000000;
    int percent = (sizeInMegabytes / maxSize) * 100;
    progressBar->setValue(percent);
}

void progressDialog::stopTimer()
{
    timer.stop();
}
