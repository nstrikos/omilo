#include "exportprogressdialog.h"
#include "ui_exportprogressdialog.h"

#include <QDebug>
#include <math.h>

ExportProgressDialog::ExportProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportProgressDialog)
{
    ui->setupUi(this);
    connect(ui->hideButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
    setWindowFlags(Qt::Dialog
                   | Qt::FramelessWindowHint
                   | Qt::CustomizeWindowHint);
}

ExportProgressDialog::~ExportProgressDialog()
{
    delete ui;
}

void ExportProgressDialog::setLabelText(QString text)
{
    ui->label->setText(text);
}

void ExportProgressDialog::hide()
{
    this->setVisible(false);
}

void ExportProgressDialog::setMaxId(int maxId)
{
    this->maxId = maxId;
}

void ExportProgressDialog::setCurrentId(int currentId)
{
    this->currentId = currentId;
    float percent = currentId * 100 / maxId ;
    ui->progressBar->setValue( round (percent) );
    qDebug() << currentId << maxId << percent;
}

void ExportProgressDialog::reset()
{
    ui->progressBar->setValue(0);
}
