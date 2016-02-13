#include "customfestivaldialog.h"
#include "ui_customfestivaldialog.h"

CustomFestivalDialog::CustomFestivalDialog(QString *command, QString *arguments, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomFestivalDialog)
{
    ui->setupUi(this);
    customFestivalCommand = command;
    customFestivalCommandArguments = arguments;
    ui->customCommandLineEdit->setText(*command);
    ui->customCommandArgumentsLineEdit->setText(*arguments);
}

CustomFestivalDialog::~CustomFestivalDialog()
{
    delete ui;
}

void CustomFestivalDialog::on_cancelButton_clicked()
{
    this->reject();
}

void CustomFestivalDialog::reject()
{
    ui->customCommandLineEdit->setText(tmpCommand);
    ui->customCommandArgumentsLineEdit->setText(tmpArguments);
    this->hide();
}

void CustomFestivalDialog::on_okButton_clicked()
{
    *customFestivalCommand = ui->customCommandLineEdit->text();
    *customFestivalCommandArguments = ui->customCommandArgumentsLineEdit->text();
    this->accept();
}

void CustomFestivalDialog::showDialog()
{
    tmpCommand = ui->customCommandLineEdit->text();
    tmpArguments = ui->customCommandArgumentsLineEdit->text();
    this->show();
}

//void CustomFestivalDialog::setCustomFestivalCommand(QString command)
//{
//    this->customFestivalCommand = command;
//    ui->customCommandLineEdit->setText(command);
//}

//void CustomFestivalDialog::setCustomFestivalCommandArguments(QString arguments)
//{
//    this->customFestivalCommandArguments = arguments;
//    ui->customCommandArgumentsLineEdit->setText(arguments);
//}
