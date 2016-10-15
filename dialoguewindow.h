#ifndef DIALOGUEWINDOW_H
#define DIALOGUEWINDOW_H

#include "speechengineinfo.h"
#include "speechengine.h"
#include <QDialog>

namespace Ui {
class DialogueWindow;
}

class DialogueWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogueWindow(QWidget *parent = 0);
    void setSpeechEngine(SpeechEngine *speechEngine);
    ~DialogueWindow();

private slots:
    void on_addButton_clicked();
    void on_loadButton_clicked();
    void on_removeButton_clicked();

    void on_okButton_clicked();

    void on_saveButton_clicked();

signals:
    void startDialogue();

private:
    Ui::DialogueWindow *ui;
    SpeechEngineInfo speechEngineInfo;
    void checkTotalWidgets();
    SpeechEngine *speechEngine;
    QList<QString> voices;
    QList<QString> texts;
    QList<QString> filenames;
    void saveToFile(QFile &file);
};

#endif // DIALOGUEWINDOW_H
