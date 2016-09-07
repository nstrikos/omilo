#include "progressdialog.h"
#include "ui_progressdialog.h"

progressDialog::progressDialog(QProcess *process, QString filename, QWidget *parent) :QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::Dialog
                   | Qt::FramelessWindowHint
                   | Qt::CustomizeWindowHint);
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
    timer.start(1000);
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
    else if (downloadFile == "/tmp/cmu_us_aew.flitevox")
        maxSize = 27;
    else if (downloadFile == "/tmp/cmu_us_ahw.flitevox")
        maxSize = 14;
    else if (downloadFile == "/tmp/cmu_us_aup.flitevox")
        maxSize = 11;
    else if (downloadFile == "/tmp/cmu_us_awb.flitevox")
        maxSize = 23;
    else if (downloadFile == "/tmp/cmu_us_axb.flitevox")
        maxSize = 15;
    else if (downloadFile == "/tmp/cmu_us_bdl.flitevox")
        maxSize = 21;
    else if (downloadFile == "/tmp/cmu_us_clb.flitevox")
        maxSize = 26;
    else if (downloadFile == "/tmp/cmu_us_eey.flitevox")
        maxSize = 12;
    else if (downloadFile == "/tmp/cmu_us_fem.flitevox")
        maxSize = 12;
    else if (downloadFile == "/tmp/cmu_us_gka.flitevox")
        maxSize = 14;
    else if (downloadFile == "/tmp/cmu_us_jmk.flitevox")
        maxSize = 21;
    else if (downloadFile == "/tmp/cmu_us_ksp.flitevox")
        maxSize = 25;
    else if (downloadFile == "/tmp/cmu_us_ljm.flitevox")
        maxSize = 11;
    else if (downloadFile == "/tmp/cmu_us_rms.flitevox")
        maxSize = 27;
    else if (downloadFile == "/tmp/cmu_us_rxr.flitevox")
        maxSize = 16;
    else if (downloadFile == "/tmp/cmu_us_slt.flitevox")
        maxSize = 24;
    else if (downloadFile == "/tmp/cmu_indic_aup_mr.flitevox")
        maxSize = 12;
    else if (downloadFile == "/tmp/cmu_indic_axb_gu.flitevox")
        maxSize = 56;
    else if (downloadFile == "/tmp/cmu_indic_axb_hi.flitevox")
        maxSize = 56;
    else if (downloadFile == "/tmp/cmu_indic_knr_te.flitevox")
        maxSize = 39;
    else if (downloadFile == "/tmp/cmu_indic_sks_ta.flitevox")
        maxSize = 32;
    else if (downloadFile == "/tmp/cmu_indic_slp_mr.flitevox")
        maxSize = 15;
    else if (downloadFile == "/tmp/cmu_indic_sxs_hi.flitevox")
        maxSize = 50;
    else if (downloadFile == "/tmp/cmu_indic_sxv_ta.flitevox")
        maxSize = 9;
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
