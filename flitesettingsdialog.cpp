#include "flitesettingsdialog.h"
#include "ui_flitesettingsdialog.h"

FliteSettingsDialog::FliteSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FliteSettingsDialog)
{
    ui->setupUi(this);
    duration = 100;
    target = 100;
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
}

FliteSettingsDialog::~FliteSettingsDialog()
{
    delete ui;
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

void FliteSettingsDialog::resetDialog()
{
    ui->checkBox->setChecked(false);
    ui->durationSlider->setValue(100);
    ui->targetSlider->setValue(100);
    updateDurationLabel();
    updateTargetLabel();
}
