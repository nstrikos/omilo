#include "displaymessagedialog.h"
#include "ui_displaymessagedialog.h"

DisplayMessageDialog::DisplayMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayMessageDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog
                   | Qt::FramelessWindowHint
                   | Qt::CustomizeWindowHint);
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
}

DisplayMessageDialog::~DisplayMessageDialog()
{
    delete ui;
}

void DisplayMessageDialog::hide()
{
    setVisible(false);
    this->accept();
}

void DisplayMessageDialog::setText(QString text)
{
    ui->textBrowser->setText(text);
    ui->textBrowser->setFocus();
}
