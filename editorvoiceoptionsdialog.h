#ifndef EDITORVOICEOPTIONSDIALOG_H
#define EDITORVOICEOPTIONSDIALOG_H

#include <QDialog>
#include <speechengineinfo.h>

namespace Ui {
class EditorVoiceOptionsDialog;
}

class EditorVoiceOptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditorVoiceOptionsDialog(QString *voice, SpeechEngineInfo *speechEngineInfo, QWidget *parent = 0);
    ~EditorVoiceOptionsDialog();

    void selectVoice();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    Ui::EditorVoiceOptionsDialog *ui;
    SpeechEngineInfo *engineInfo;
    void updateVoices();
    QString *voice;
};

#endif // EDITORVOICEOPTIONSDIALOG_H
