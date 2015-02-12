#include "editorvoiceoptionsdialog.h"
#include "ui_editorvoiceoptionsdialog.h"

EditorVoiceOptionsDialog::EditorVoiceOptionsDialog(QString *voice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditorVoiceOptionsDialog)
{
    ui->setupUi(this);
    engineInfo = new SpeechEngineInfo();
    this->voice = voice;
}

EditorVoiceOptionsDialog::~EditorVoiceOptionsDialog()
{
    delete engineInfo;
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
    this->reject();
}

void EditorVoiceOptionsDialog::on_okButton_clicked()
{
    *voice = ui->comboBox->currentText();
    this->accept();
}
