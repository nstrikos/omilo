#ifndef INSTALLVOICESDIALOG_H
#define INSTALLVOICESDIALOG_H

#include <QDialog>
#include <QProcess>
#include "ui_installvoicesdialog.h"
#include "progressdialog.h"
#include "speechengineinfo.h"

class InstallVoicesDialog : public QDialog, public Ui::InstallVoicesDialog
{
    Q_OBJECT
    
public:
    InstallVoicesDialog(QWidget *parent = 0);
    ~InstallVoicesDialog();
    void showWindow(QString currentVoice);

public slots:
    void isSelectedVoiceInstalled(QString voice);

private slots:
    void downloadFinished();
    void installationFinished();
    void removeFinished();
    void on_closeButton_clicked();
    void on_awbRemoveButton_clicked();
    void on_awbInstallButton_clicked();
    void on_bdlInstallButton_clicked();
    void on_clbInstallButton_clicked();
    void on_jmkInstallButton_clicked();
    void on_kspInstallButton_clicked();
    void on_rmsInstallButton_clicked();
    void on_sltInstallButton_clicked();
    void on_bdlRemoveButton_clicked();
    void on_clbRemoveButton_clicked();
    void on_jmkRemoveButton_clicked();
    void on_kspRemoveButton_clicked();
    void on_rmsRemoveButton_clicked();
    void on_sltRemoveButton_clicked();

    void on_aewFliteInstallButton_clicked();

    void on_ahwFliteInstallButton_clicked();

    void on_aupFliteInstallButton_clicked();

    void on_awb2FliteInstallButton_clicked();

    void on_axbFliteInstallButton_clicked();

    void on_bdlFliteInstallButton_clicked();

    void on_clbFliteInstallButton_clicked();

    void on_eeyFliteInstallButton_clicked();

    void on_femFliteInstallButton_clicked();

    void on_gkaFliteInstallButton_clicked();

    void on_jmkFliteInstallButton_clicked();

    void on_kspFliteInstallButton_clicked();

    void on_ljmFliteInstallButton_clicked();

    void on_rms2FliteInstallButton_clicked();

    void on_rxrFliteInstallButton_clicked();

    void on_slt2FliteInstallButton_clicked();

    void on_indicAupMrFliteInstallButton_clicked();

    void on_IndicAxbGuFliteInstallButton_clicked();

    void on_IndicAxbHiFliteInstallButton_clicked();

    void on_IndicKnrTeFliteInstallButton_clicked();

    void on_IndicSksTaFliteInstallButton_clicked();

    void on_IndicSlpMrFliteInstallButton_clicked();

    void on_IndicSxsHiFliteInstallButton_clicked();

    void on_IndicSxvTaFliteInstallButton_clicked();

private:
    void updateAvailableVoices();
    SpeechEngineInfo *engineInfo;
    QString downloadCommand;
    QString installationCommandPart2;
    QString sudoCommand;
    QString removeCommandPart2;
    QProcess *downloadProcess;
    QProcess *installationProcess;
    QProcess *removeProcess;
    progressDialog *myProgressDialog;
    QString downloadFilename;
    void startDownload();
    QString currentVoice;

signals:
    void updateVoice();
};

#endif // INSTALLVOICESDIALOG_H
