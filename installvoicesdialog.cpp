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

    QFile file1("/usr/bin/gksu");
    QFile file2("/usr/bin/kdesu");
    QFile file3("/usr/bin/gnomesu");
    if (file1.exists())
        sudoCommand = "gksu ";
    else if (file2.exists())
        sudoCommand = "kdesu ";
    else if (file3.exists())
        sudoCommand = "gnomesu ";
}

InstallVoicesDialog::~InstallVoicesDialog()
{
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
        if (engineInfo->installedVoices[i].mode == festivalCmu)
        {
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
        }
    }

    for (i = 0; i < engineInfo->availableVoices.size(); i++)
    {
        if (engineInfo->availableVoices[i].mode == festivalCmu)
        {
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
        }
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
        installationProcess->start("sh", QStringList() << "-c" << sudoCommand + installationCommandPart2);
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
    isSelectedVoiceInstalled();
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
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_awb_arctic.sh";
    downloadFilename = "/tmp/cmu-us-awb-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-awb-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_bdlInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_bdl_arctic.sh";
    downloadFilename = "/tmp/cmu-us-bdl-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-bdl-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_clbInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_clb_arctic.sh";
    downloadFilename = "/tmp/cmu-us-clb-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-clb-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_jmkInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_jmk_arctic.sh";
    downloadFilename = "/tmp/cmu-us-jmk-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-jmk-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_kspInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_ksp_arctic.sh";
    downloadFilename = "/tmp/cmu-us-ksp-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-ksp-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_rmsInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_rms_arctic.sh";
    downloadFilename = "/tmp/cmu-us-rms-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-rms-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_sltInstallButton_clicked()
{
    installationCommandPart2 = " /usr/share/omilo/scripts/install_cmu_us_slt_arctic.sh";
    downloadFilename = "/tmp/cmu-us-slt-arctic-voice_0.95-1_all.deb";
    downloadCommand = "cd /tmp && wget -c http://sourceforge.net/projects/o-milo/files/Omilo/linux/voices/cmu-us-slt-arctic-voice_0.95-1_all.deb";
    startDownload();
}

void InstallVoicesDialog::on_awbRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_awb_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_bdlRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_bdl_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_clbRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_clb_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_jmkRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_jmk_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_kspRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_ksp_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_rmsRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_rms_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::on_sltRemoveButton_clicked()
{
    removeCommandPart2 = " /usr/share/omilo/scripts/remove_cmu_us_slt_arctic.sh";
    removeProcess->start("sh", QStringList() << "-c" << sudoCommand + removeCommandPart2);
}

void InstallVoicesDialog::isSelectedVoiceInstalled()
{
    bool found = false;
    int size = engineInfo->installedVoices.size();
    for (int i = 0; i < size; i++)
    {
        VoiceInfo tmp = engineInfo->installedVoices.at(i);
        if (tmp.name == currentVoice)
            found = true;
    }
    if (!found)
    {
        emit updateVoice();
    }
}
