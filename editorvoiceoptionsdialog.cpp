#include "editorvoiceoptionsdialog.h"
#include "ui_editorvoiceoptionsdialog.h"

EditorVoiceOptionsDialog::EditorVoiceOptionsDialog(QString *voice, SpeechEngineInfo *speechEngineInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditorVoiceOptionsDialog)
{
    ui->setupUi(this);
    engineInfo = speechEngineInfo;
    this->voice = voice;
    qDebug() << "Voice options dialog created...";
}

EditorVoiceOptionsDialog::~EditorVoiceOptionsDialog()
{
    qDebug() << "Deleting voice options dialog...";
    delete ui;
}

void EditorVoiceOptionsDialog::updateVoices()
{
    unsigned int i;

    ui->comboBox->clear();
    engineInfo->update();
    for (i = 0; i < engineInfo->installedVoices.size(); i++)
    {
        ui->comboBox->addItem(engineInfo->installedVoices[i].name);
        //set active voice
        if (ui->comboBox->itemText(i) == *voice)
        {
            ui->comboBox->setCurrentIndex(i);
        }
    }
}

void EditorVoiceOptionsDialog::selectVoice()
{
    updateVoices();
    ui->comboBox->setFocus();
    this->show();
}

void EditorVoiceOptionsDialog::on_cancelButton_clicked()
{
    qDebug() << "Voice options dialog rejected...";
    this->reject();
}

void EditorVoiceOptionsDialog::on_okButton_clicked()
{
    *voice = ui->comboBox->currentText();
    qDebug() << "Voice options dialog executed...";
    qDebug() << "Voice now is: " << *voice;
    this->accept();
}
