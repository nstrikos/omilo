#ifndef DIALOGUEWINDOW_H
#define DIALOGUEWINDOW_H

#include "speechengineinfo.h"
#include "speechengine.h"
#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "customtextedit.h"
#include "displaymessagedialog.h"

namespace Ui {
class DialogueWindow;
}

class DialogueWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogueWindow(QWidget *parent = 0);
    void setSpeechEngine(SpeechEngine *speechEngine);
    bool exporting;
    QString exportFilename;
    ~DialogueWindow();

private slots:
    void on_addButton_clicked();
    void on_loadButton_clicked();
    void on_removeButton_clicked();
    void on_okButton_clicked();
    void on_saveButton_clicked();
    void setSelectedText(QString name, QString text);
    void on_speakButton_clicked();
    void on_exportButton_clicked();

signals:
    void startDialogue();
    void clearPlaylist();

private:
    Ui::DialogueWindow *ui;
    SpeechEngineInfo speechEngineInfo;
    void checkTotalWidgets();
    SpeechEngine *speechEngine;
    QList<QString> voices;
    QList<QString> texts;
    QList<QString> filenames;
    QList<double> pauses;
    void clearLists();
    void saveToFile(QFile &file);
    void loadFromFile(QFile &file);
    void makeDialogueFromInputText(QString inText);
    void removeAllWidgets();
    void addWidgets(QString voice, QString text, double pause);
    QList<CustomTextEdit*> qTextEditList;
    QList<QComboBox*> qComboBoxList;
    QList<QDoubleSpinBox*> qDoubleSpinBoxList;
    QString selectedVoice, selectedText;
    DisplayMessageDialog *displayMessageDialog;
};

#endif // DIALOGUEWINDOW_H
