#include "flitesettingsdialog.h"
#include "ui_flitesettingsdialog.h"

FliteSettingsDialog::FliteSettingsDialog(QString voice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FliteSettingsDialog)
{
    this->voice = voice;
    ui->setupUi(this);
    ui->checkBox->setChecked(false);
    ui->durationSlider->setEnabled(false);
    ui->durationSlider->setValue(duration);
    ui->targetSlider->setEnabled(false);
    ui->targetSlider->setValue(target);
    updateDurationLabel();
    updateTargetLabel();
    connect(ui->checkBox, SIGNAL(clicked()), this, SLOT(checkBoxChanged()));
    connect(ui->durationSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDuration()));
    connect(ui->targetSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTarget()));
    update(voice);
    qDebug() << "Flite settings dialog created...";
}

FliteSettingsDialog::~FliteSettingsDialog()
{
    qDebug() << "Deleting flite settings dialog...";
    delete ui;
}

void FliteSettingsDialog::update(QString voice)
{
    //The dialog should be updated only when flite voice is active
    //Further, the whole dialog should be active only when flite voice is active

    this->voice = voice;
    qDebug() << "Updating flite dialog controls. Voice is: " << voice;
    if (voice == SltFlite)
        target = defSltTargetMean;
    else
        target = defTargetMean;
    duration = defDurationStretch;
    qDebug() << "Target mean is: " << target;
    qDebug() << "Duration is: " << duration;
    ui->durationSlider->setValue(duration);
    ui->targetSlider->setValue(target);
    updateDurationLabel();
    updateTargetLabel();
}

void FliteSettingsDialog::checkBoxChanged()
{
    if (ui->checkBox->isChecked())
    {
        ui->durationSlider->setEnabled(true);
        ui->targetSlider->setEnabled(true);
    }
    else
    {
        ui->durationSlider->setEnabled(false);
        ui->targetSlider->setEnabled(false);
        update(voice);
    }
}

unsigned int FliteSettingsDialog::getDuration()
{
    return duration;
}

unsigned int FliteSettingsDialog::getTarget()
{
    return target;
}

void FliteSettingsDialog::updateDuration()
{
    duration = ui->durationSlider->value();
    updateDurationLabel();
}

void FliteSettingsDialog::updateTarget()
{
    target = ui->targetSlider->value();
    updateTargetLabel();
}

void FliteSettingsDialog::updateDurationLabel()
{
    ui->durationLabel->setText(tr("Duration stretch:") + QString::number(duration));
}

void FliteSettingsDialog::updateTargetLabel()
{
    ui->targetLabel->setText(tr("Target mean:") + QString::number(target));
}

void FliteSettingsDialog::on_okButton_clicked()
{
    this->accept();
}
