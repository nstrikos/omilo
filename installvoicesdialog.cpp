#include "installvoicesdialog.h"
#include "progressdialog.h"

InstallVoicesDialog::InstallVoicesDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    myProgressDialog = NULL;
    installationProcess = new QProcess();
    downloadProcess = new QProcess();
    removeProcess = new QProcess();

    engineInfo = new SpeechEngineInfo();

    connect(downloadProcess, SIGNAL(finished(int)), this, SLOT(downloadFinished()));
    connect(installationProcess, SIGNAL(finished(int)), this, SLOT(installationFinished()));
    connect(removeProcess, SIGNAL(finished(int)), this, SLOT(removeFinished()));

    QFile file1("/usr/bin/pkexec");
    QFile file2("/usr/bin/gksu");
    QFile file3("/usr/bin/kdesu");
    QFile file4("/usr/bin/gnomesu");
    if (file1.exists())
        sudoCommand = "pkexec ";
    else if (file2.exists())
        sudoCommand = "gksu ";
    else if (file3.exists())
        sudoCommand = "kdesu ";
    else if (file4.exists())
        sudoCommand = "gnomesu ";

    qDebug() << "Install voices dialog created...";
}

InstallVoicesDialog::~InstallVoicesDialog()
{
    qDebug() << "Deleting install voices dialog...";
    delete installationProcess;
    delete removeProcess;
    delete downloadProcess;
    delete myProgressDialog;
    delete engineInfo;
}

void InstallVoicesDialog::updateAvailableVoices()
{
    unsigned int i;

    engineInfo->update();

    for (i = 0; i < engineInfo->installedVoices.size(); i++)
    {
        //if (engineInfo->installedVoices[i].mode == festivalCmu)
        //{
            if (engineInfo->installedVoices[i].name == AwbCmuFestival)
            {
                awbInstallButton->setEnabled(false);
                awbRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == BdlCmuFestival)
            {
                bdlInstallButton->setEnabled(false);
                bdlRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == ClbCmuFestival)
            {
                clbInstallButton->setEnabled(false);
                clbRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == JmkCmuFestival)
            {
                jmkInstallButton->setEnabled(false);
                jmkRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == KspCmuFestival)
            {
                kspInstallButton->setEnabled(false);
                kspRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == RmsCmuFestival)
            {
                rmsInstallButton->setEnabled(false);
                rmsRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == SltCmuFestival)
            {
                sltInstallButton->setEnabled(false);
                sltRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == AewFlite)
            {
                aewFliteInstallButton->setEnabled(false);
                aewFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == AhwFlite)
            {
                ahwFliteInstallButton->setEnabled(false);
                ahwFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == AupFlite)
            {
                aupFliteInstallButton->setEnabled(false);
                aupFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == Awb2Flite)
            {
                awb2FliteInstallButton->setEnabled(false);
                awb2FliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == AxbFlite)
            {
                axbFliteInstallButton->setEnabled(false);
                axbFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == BdlFlite)
            {
                bdlFliteInstallButton->setEnabled(false);
                bdlFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == ClbFlite)
            {
                clbFliteInstallButton->setEnabled(false);
                clbFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == EeyFlite)
            {
                eeyFliteInstallButton->setEnabled(false);
                eeyFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == FemFlite)
            {
                femFliteInstallButton->setEnabled(false);
                femFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == GkaFlite)
            {
                gkaFliteInstallButton->setEnabled(false);
                gkaFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == JmkFlite)
            {
                jmkFliteInstallButton->setEnabled(false);
                jmkFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == KspFlite)
            {
                kspFliteInstallButton->setEnabled(false);
                kspFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == LjmFlite)
            {
                ljmFliteInstallButton->setEnabled(false);
                ljmFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == Rms2Flite)
            {
                rms2FliteInstallButton->setEnabled(false);
                rms2FliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == RxrFlite)
            {
                rxrFliteInstallButton->setEnabled(false);
                rxrFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == Slt2Flite)
            {
                slt2FliteInstallButton->setEnabled(false);
                slt2FliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicAupMrFlite)
            {
                indicAupMrFliteInstallButton->setEnabled(false);
                indicAupMrFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicAxbGuFlite)
            {
                IndicAxbGuFliteInstallButton->setEnabled(false);
                IndicAxbGuFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicAxbHiFlite)
            {
                IndicAxbHiFliteInstallButton->setEnabled(false);
                IndicAxbHiFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicKnrTeFlite)
            {
                IndicKnrTeFliteInstallButton->setEnabled(false);
                IndicKnrTeFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicSksTaFlite)
            {
                IndicSksTaFliteInstallButton->setEnabled(false);
                IndicSksTaFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicSlpMrFlite)
            {
                IndicSlpMrFliteInstallButton->setEnabled(false);
                IndicSlpMrFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicSxsHiFlite)
            {
                IndicSxsHiFliteInstallButton->setEnabled(false);
                IndicSxsHiFliteRemoveButton->setEnabled(true);
            }
            else if (engineInfo->installedVoices[i].name == IndicSxvTaFlite)
            {
                IndicSxvTaFliteInstallButton->setEnabled(false);
                IndicSxvTaFliteRemoveButton->setEnabled(true);
            }            
        //}
    }

    for (i = 0; i < engineInfo->availableVoices.size(); i++)
    {
        //if (engineInfo->availableVoices[i].mode == festivalCmu)
        //{
            if (engineInfo->availableVoices[i].name == AwbCmuFestival)
            {
                awbInstallButton->setEnabled(true);
                awbRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == BdlCmuFestival)
            {
                bdlInstallButton->setEnabled(true);
                bdlRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == ClbCmuFestival)
            {
                clbInstallButton->setEnabled(true);
                clbRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == JmkCmuFestival)
            {
                jmkInstallButton->setEnabled(true);
                jmkRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == KspCmuFestival)
            {
                kspInstallButton->setEnabled(true);
                kspRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == RmsCmuFestival)
            {
                rmsInstallButton->setEnabled(true);
                rmsRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == SltCmuFestival)
            {
                sltInstallButton->setEnabled(true);
                sltRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == AewFlite)
            {
                aewFliteInstallButton->setEnabled(true);
                aewFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == AhwFlite)
            {
                ahwFliteInstallButton->setEnabled(true);
                ahwFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == AupFlite)
            {
                aupFliteInstallButton->setEnabled(true);
                aupFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == Awb2Flite)
            {
                awb2FliteInstallButton->setEnabled(true);
                awb2FliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == AxbFlite)
            {
                axbFliteInstallButton->setEnabled(true);
                axbFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == BdlFlite)
            {
                bdlFliteInstallButton->setEnabled(true);
                bdlFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == ClbFlite)
            {
                clbFliteInstallButton->setEnabled(true);
                clbFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == EeyFlite)
            {
                eeyFliteInstallButton->setEnabled(true);
                eeyFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == FemFlite)
            {
                femFliteInstallButton->setEnabled(true);
                femFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == GkaFlite)
            {
                gkaFliteInstallButton->setEnabled(true);
                gkaFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == JmkFlite)
            {
                jmkFliteInstallButton->setEnabled(true);
                jmkFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == KspFlite)
            {
                kspFliteInstallButton->setEnabled(true);
                kspFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == LjmFlite)
            {
                ljmFliteInstallButton->setEnabled(true);
                ljmFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == Rms2Flite)
            {
                rms2FliteInstallButton->setEnabled(true);
                rms2FliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == RxrFlite)
            {
                rxrFliteInstallButton->setEnabled(true);
                rxrFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == Slt2Flite)
            {
                slt2FliteInstallButton->setEnabled(true);
                slt2FliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicAupMrFlite)
            {
                indicAupMrFliteInstallButton->setEnabled(true);
                indicAupMrFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicAxbGuFlite)
            {
                IndicAxbGuFliteInstallButton->setEnabled(true);
                IndicAxbGuFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicAxbHiFlite)
            {
                IndicAxbHiFliteInstallButton->setEnabled(true);
                IndicAxbHiFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicKnrTeFlite)
            {
                IndicKnrTeFliteInstallButton->setEnabled(true);
                IndicKnrTeFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicSksTaFlite)
            {
                IndicSksTaFliteInstallButton->setEnabled(true);
                IndicSksTaFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicSlpMrFlite)
            {
                IndicSlpMrFliteInstallButton->setEnabled(true);
                IndicSlpMrFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicSxsHiFlite)
            {
                IndicSxsHiFliteInstallButton->setEnabled(true);
                IndicSxsHiFliteRemoveButton->setEnabled(false);
            }
            else if (engineInfo->availableVoices[i].name == IndicSxvTaFlite)
            {
                IndicSxvTaFliteInstallButton->setEnabled(true);
                IndicSxvTaFliteRemoveButton->setEnabled(false);
            }
        //}
    }
}

void InstallVoicesDialog::downloadFinished()
{
    //If file exists download was successful. Otherwise download was cancelled
    if (QFile::exists(downloadFilename))
    {
        QString text = tr("Installing...");
        myProgressDialog->textBrowser->setText(text);
        myProgressDialog->progressBar->setValue(100);
        QString command = "sh";
        QString c = "-c";
        QStringList list;
        list << c << sudoCommand + installationCommandPart2;
        installationProcess->start(command, list);
    }
    else
        myProgressDialog->hide();
    myProgressDialog->stopTimer();
}

void InstallVoicesDialog::installationFinished()
{
    myProgressDialog->hide();
    engineInfo->update();
    updateAvailableVoices();
}

void InstallVoicesDialog::removeFinished()
{
    engineInfo->update();
    updateAvailableVoices();
    isSelectedVoiceInstalled(currentVoice);
}

void InstallVoicesDialog::showWindow(QString currentVoice)
{
    updateAvailableVoices();
    this->show();
    this->currentVoice = currentVoice;
}

void InstallVoicesDialog::on_closeButton_clicked()
{
    this->accept();
}

void InstallVoicesDialog::startDownload()
{
    if (!myProgressDialog)
        myProgressDialog = new progressDialog(downloadProcess, downloadFilename, this);
    else
    {
        myProgressDialog->updateProgressDialog(downloadProcess, downloadFilename);
        myProgressDialog->textBrowser->setFocus();
    }
    myProgressDialog->setModal(true);
    QString text = tr("Downloading...\nVoice packages are over 100 Megabytes, this may take a while...\nYour password will be required.");
    myProgressDialog->textBrowser->setText(text);
    myProgressDialog->show();
    downloadProcess->start("sh", QStringList() << "-c" << downloadCommand);
}

void InstallVoicesDialog::on_awbInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_awb_arctic.sh";
    downloadFilename = "/tmp/cmu_us_awb_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_awb_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_bdlInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_bdl_arctic.sh";
    downloadFilename = "/tmp/cmu_us_bdl_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_bdl_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_clbInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_clb_arctic.sh";
    downloadFilename = "/tmp/cmu_us_clb_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_clb_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_jmkInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_jmk_arctic.sh";
    downloadFilename = "/tmp/cmu_us_jmk_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_jmk_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_kspInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_ksp_arctic.sh";
    downloadFilename = "/tmp/cmu_us_ksp_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_ksp_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_rmsInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_rms_arctic.sh";
    downloadFilename = "/tmp/cmu_us_rms_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_rms_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_sltInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_cmu_us_slt_arctic.sh";
    downloadFilename = "/tmp/cmu_us_slt_arctic-0.95-release.tar.bz2";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu_us_slt_arctic-0.95-release.tar.bz2";
    startDownload();
}

void InstallVoicesDialog::on_awbRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_awb_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_bdlRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_bdl_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_clbRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_clb_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_jmkRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_jmk_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_kspRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_ksp_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_rmsRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_rms_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_sltRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo-qt5/scripts/remove_cmu_us_slt_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::isSelectedVoiceInstalled(QString voice)
{
    bool found = false;
    int size = engineInfo->installedVoices.size();
    for (int i = 0; i < size; i++)
    {
        VoiceInfo tmp = engineInfo->installedVoices.at(i);
        if (tmp.name == voice)
            found = true;
    }
    if (!found)
    {
        emit updateVoice();
    }
}

void InstallVoicesDialog::on_aewFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_aew_flite.sh";
    downloadFilename = "/tmp/cmu_us_aew.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_aew.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_ahwFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_ahw_flite.sh";
    downloadFilename = "/tmp/cmu_us_ahw.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_ahw.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_aupFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_aup_flite.sh";
    downloadFilename = "/tmp/cmu_us_aup.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_aup.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_awb2FliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_awb_flite.sh";
    downloadFilename = "/tmp/cmu_us_awb.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_awb.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_axbFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_axb_flite.sh";
    downloadFilename = "/tmp/cmu_us_axb.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_axb.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_bdlFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_bdl_flite.sh";
    downloadFilename = "/tmp/cmu_us_bdl.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_bdl.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_clbFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_clb_flite.sh";
    downloadFilename = "/tmp/cmu_us_clb.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_clb.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_eeyFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_eey_flite.sh";
    downloadFilename = "/tmp/cmu_us_eey.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_eey.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_femFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_fem_flite.sh";
    downloadFilename = "/tmp/cmu_us_fem.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_fem.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_gkaFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_gka_flite.sh";
    downloadFilename = "/tmp/cmu_us_gka.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_gka.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_jmkFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_jmk_flite.sh";
    downloadFilename = "/tmp/cmu_us_jmk.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_jmk.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_kspFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_ksp_flite.sh";
    downloadFilename = "/tmp/cmu_us_ksp.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_ksp.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_ljmFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_ljm_flite.sh";
    downloadFilename = "/tmp/cmu_us_ljm.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_ljm.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_rms2FliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_rms_flite.sh";
    downloadFilename = "/tmp/cmu_us_rms.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_rms.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_rxrFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_rxr_flite.sh";
    downloadFilename = "/tmp/cmu_us_rxr.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_rxr.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_slt2FliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_slt_flite.sh";
    downloadFilename = "/tmp/cmu_us_slt.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_us_slt.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_indicAupMrFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_aup_mr_flite.sh";
    downloadFilename = "/tmp/cmu_indic_aup_mr.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_aup_mr.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicAxbGuFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_axb_gu_flite.sh";
    downloadFilename = "/tmp/cmu_indic_axb_gu.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_axb_gu.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicAxbHiFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_axb_hi_flite.sh";
    downloadFilename = "/tmp/cmu_indic_axb_hi.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_axb_hi.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicKnrTeFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_knr_te_flite.sh";
    downloadFilename = "/tmp/cmu_indic_knr_te.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_knr_te.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicSksTaFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_sks_ta_flite.sh";
    downloadFilename = "/tmp/cmu_indic_sks_ta.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_sks_ta.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicSlpMrFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_slp_mr_flite.sh";
    downloadFilename = "/tmp/cmu_indic_slp_mr.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_slp_mr.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicSxsHiFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_sxs_hi_flite.sh";
    downloadFilename = "/tmp/cmu_indic_sxs_hi.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_sxs_hi.flitevox";
    startDownload();
}

void InstallVoicesDialog::on_IndicSxvTaFliteInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo-qt5/scripts/install_indic_sxv_ta_flite.sh";
    downloadFilename = "/tmp/cmu_indic_sxv_ta.flitevox";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/flite/cmu_indic_sxv_ta.flitevox";
    startDownload();
}
