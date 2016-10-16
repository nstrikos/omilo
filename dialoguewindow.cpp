#include "dialoguewindow.h"
#include "ui_dialoguewindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

DialogueWindow::DialogueWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogueWindow)
{
    ui->setupUi(this);
    ui->removeButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->okButton->setEnabled(false);
}

DialogueWindow::~DialogueWindow()
{
    delete ui;
}

void DialogueWindow::on_addButton_clicked()
{
    addWidgets("", "", 0);
}

void DialogueWindow::addWidgets(QString voice, QString text, double pause)
{
    QComboBox *tmpCombo = new QComboBox();
    //The best way for dynamic widgets is to keep track with a list
    qComboBoxList.push_back(tmpCombo);
    for (unsigned int i = 0; i < speechEngineInfo.installedVoices.size(); i++)
        tmpCombo->addItem(speechEngineInfo.installedVoices.at(i).name);
    tmpCombo->setCurrentText(voice);
    //Nice way to set widget name
    //tmpCombo->setObjectName("combobox"+QString::number(qComboBoxList.count()));

    QLineEdit *tmpLine = new QLineEdit(this);
    qLineEditList.push_back(tmpLine);
    tmpLine->setText(text);
    //tmpLine->setObjectName("lineedit"+QString::number(qLineEditList.count()));

    QDoubleSpinBox *tmpSpinBox = new QDoubleSpinBox();
    qDoubleSpinBoxList.push_back(tmpSpinBox);
    tmpSpinBox->setValue(pause);
    //tmpSpinBox->setObjectName("spinbox"+QString::number(qDoubleSpinBoxList.count()));

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(tmpCombo);
    hBoxLayout->addWidget(tmpLine);
    hBoxLayout->addWidget(tmpSpinBox);

    ui->VLayout->insertLayout(ui->VLayout->count() - 1, hBoxLayout, 0);

    checkTotalWidgets();
}

void DialogueWindow::on_removeButton_clicked()
{
    //This is a nice way to get all widgets
    //But it's not necessary in this code.
    //QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    //QList<QLineEdit *> allLinedits = ui->scrollArea->findChildren<QLineEdit *>();
    //QList<QDoubleSpinBox *> allSpinboxes = ui->scrollArea->findChildren<QDoubleSpinBox *>();

    if (!qComboBoxList.isEmpty())
    {
        QComboBox *tmpComboBox = qComboBoxList.takeLast();
        ui->VLayout->removeWidget(tmpComboBox);
        delete tmpComboBox;
    }
    if (!qLineEditList.isEmpty())
    {
        QLineEdit *tmpLineEdit = qLineEditList.takeLast();
        ui->VLayout->removeWidget(tmpLineEdit);
        delete tmpLineEdit;
    }
    if (!qDoubleSpinBoxList.isEmpty())
    {
        QDoubleSpinBox *tmpSpinBox = qDoubleSpinBoxList.takeLast();
        ui->VLayout->removeWidget(tmpSpinBox);
        delete tmpSpinBox;
    }
    checkTotalWidgets();
}

void DialogueWindow::checkTotalWidgets()
{
    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    if (allCombos.length() == 0)
    {
        ui->removeButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->okButton->setEnabled(false);
    }
    else if (allCombos.length() > 0)
    {
        ui->removeButton->setEnabled(true);
        ui->saveButton->setEnabled(true);
        ui->okButton->setEnabled(true);
    }
}

void DialogueWindow::on_okButton_clicked()
{
    clearLists();

    for (int i = 0; i < qComboBoxList.size(); i++)
    {
        QComboBox *combo = qComboBoxList.at(i);
        QLineEdit *lineedit = qLineEditList.at(i);
        QDoubleSpinBox *spin = qDoubleSpinBoxList.at(i);
        voices.append(combo->currentText());
        texts.append(lineedit->text().simplified());
        filenames.append(wavPrefix + QString::number(i) + ".wav");
        pauses.append(spin->value());
        qDebug() << combo->currentText() << lineedit->text() << spin->value();
    }
    speechEngine->setRate(1.0);
    speechEngine->makeDialogue(voices, texts, pauses, filenames);
    this->hide();
}

void DialogueWindow::setSpeechEngine(SpeechEngine *speechEngine)
{
    this->speechEngine = speechEngine;
}

void DialogueWindow::on_saveButton_clicked()
{
    //Set filename
    qDebug() << "Saving as...";
    QString filename = QFileDialog::getSaveFileName(this);
    qDebug() << "Filename is " << filename;
    if (filename.isEmpty())
        return;

    QFile file(filename);
    qDebug() << "Filename is " << filename;
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Omilo"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(filename)
                             .arg(file.errorString()));
        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    saveToFile(file);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void DialogueWindow::saveToFile(QFile &file)
{
    //Write to file
    QTextStream out (&file);

    for (int i = 0; i < qComboBoxList.size(); i++)
    {
        QComboBox *combo = qComboBoxList.at(i);
        QLineEdit *lineedit = qLineEditList.at(i);
        QDoubleSpinBox *spin = qDoubleSpinBoxList.at(i);
        out << "(" << combo->currentText() << ",";
        out << lineedit->text().simplified() << ",";
        out << QString::number(spin->value()) << ")\n";
    }
}

void DialogueWindow::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    qDebug() << "Filename is " << filename;
    if (!filename.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Omilo"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(filename)
                                 .arg(file.errorString()));
            return;
        }

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

        loadFromFile(file);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        qDebug() << "Loaded file..." << filename;
    }
}

void DialogueWindow::loadFromFile(QFile &file)
{
    QTextStream in (&file);
    QString inText = in.readAll();
    makeDialogueFromInputText(inText);
}

void DialogueWindow::makeDialogueFromInputText(QString inText)
{
    int count = 0;
    count = inText.count("(");
    if (count > 0)
    {
        removeAllWidgets();
    }
    while (count > 0)
    {
        int pos1 = inText.indexOf(",");
        int pos2 = inText.indexOf(",", pos1 + 1);
        int pos3 = inText.indexOf(")\n", pos2 + 1);
        QString voice = inText.mid(1, pos1 - 1);
        QString text = inText.mid(pos1 + 1, pos2 - pos1 - 1);
        QString pause = inText.mid(pos2 + 1, pos3 - pos2 - 1);

        inText = inText.right(inText.size() - pos3 - 2);
        addWidgets(voice, text, pause.toDouble());
        count--;
    }
}

void DialogueWindow::removeAllWidgets()
{
    while (!qComboBoxList.isEmpty())
    {
        QComboBox *tmpComboBox = qComboBoxList.takeLast();
        ui->VLayout->removeWidget(tmpComboBox);
        delete tmpComboBox;
    }
    while (!qLineEditList.isEmpty())
    {
        QLineEdit *tmpLineEdit = qLineEditList.takeLast();
        ui->VLayout->removeWidget(tmpLineEdit);
        delete tmpLineEdit;
    }
    while (!qDoubleSpinBoxList.isEmpty())
    {
        QDoubleSpinBox *tmpSpinBox = qDoubleSpinBoxList.takeLast();
        ui->VLayout->removeWidget(tmpSpinBox);
        delete tmpSpinBox;
    }
    checkTotalWidgets();
}

void DialogueWindow::clearLists()
{
    voices.clear();
    texts.clear();
    filenames.clear();
    pauses.clear();
}
