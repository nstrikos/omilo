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
    timer.start(1000);
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
    if (downloadFile == "/tmp/cmu_us_awb_arctic-0.95-release.tar.bz2")
        maxSize = 123;
    else if (downloadFile == "/tmp/cmu_us_bdl_arctic-0.95-release.tar.bz2")
        maxSize = 90;
    else if (downloadFile == "/tmp/cmu_us_clb_arctic-0.95-release.tar.bz2")
        maxSize = 124;
    else if (downloadFile == "/tmp/cmu_us_jmk_arctic-0.95-release.tar.bz2")
        maxSize = 87;
    else if (downloadFile == "/tmp/cmu_us_ksp_arctic-0.95-release.tar.bz2")
        maxSize = 113;
    else if (downloadFile == "/tmp/cmu_us_rms_arctic-0.95-release.tar.bz2")
        maxSize = 110;
    else if (downloadFile == "/tmp/cmu_us_slt_arctic-0.95-release.tar.bz2")
        maxSize = 114;
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
