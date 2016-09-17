#include "flitesettingsdialog.h"
#include "ui_flitesettingsdialog.h"

FliteSettingsDialog::FliteSettingsDialog(QString voice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FliteSettingsDialog)
{
    this->voice = voice;
    ui->setupUi(this);
    ui->durationCheckBox->setChecked(false);
    ui->targetMeanCheckBox->setChecked(false);
    ui->durationSlider->setEnabled(false);
    ui->durationSlider->setValue(duration);
    ui->targetSlider->setEnabled(false);
    ui->targetSlider->setValue(target);
    updateDurationLabel();
    updateTargetLabel();
    connect(ui->durationCheckBox, SIGNAL(clicked()), this, SLOT(durationCheckboxClicked()));
    connect(ui->targetMeanCheckBox, SIGNAL(clicked(bool)), this, SLOT(targetMeanCheckboxClicked()));
    connect(ui->durationSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDuration()));
    connect(ui->targetSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTarget()));
    useDurationStretch = false;
    useTargetMean = false;
    updateDurationStretch();
    updateTargetMean();
    qDebug() << "Flite settings dialog created...";
}

FliteSettingsDialog::~FliteSettingsDialog()
{
    qDebug() << "Deleting flite settings dialog...";
    delete ui;
}

void FliteSettingsDialog::updateTargetMean()
{
    //The dialog should be updated only when flite voice is active
    //Further, the whole dialog should be active only when flite voice is active

    target = defTargetMean;
    qDebug() << "Target mean is: " << target;
    ui->targetSlider->setValue(target);
    updateTargetLabel();
}

void FliteSettingsDialog::updateDurationStretch()
{
    //The dialog should be updated only when flite voice is active
    //Further, the whole dialog should be active only when flite voice is active

    duration = defDurationStretch;
    qDebug() << "Duration is: " << duration;
    ui->durationSlider->setValue(duration);
    updateDurationLabel();
}

void FliteSettingsDialog::durationCheckboxClicked()
{
    if (ui->durationCheckBox->isChecked())
    {
        ui->durationSlider->setEnabled(true);
        useDurationStretch = true;
    }
    else
    {
        ui->durationSlider->setEnabled(false);
        useDurationStretch = false;
        updateDurationStretch();
    }
}

void FliteSettingsDialog::targetMeanCheckboxClicked()
{
    if (ui->targetMeanCheckBox->isChecked())
    {
        ui->targetSlider->setEnabled(true);
        useTargetMean = true;
    }
    else
    {
        ui->targetSlider->setEnabled(false);
        useTargetMean = false;
        updateTargetMean();
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

bool FliteSettingsDialog::getUseDurationStretch()
{
    return useDurationStretch;
}

bool FliteSettingsDialog::getUseTargetMean()
{
    return useTargetMean;
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
