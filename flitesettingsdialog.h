#ifndef FLITESETTINGSDIALOG_H
#define FLITESETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class FliteSettingsDialog;
}

class FliteSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FliteSettingsDialog(QWidget *parent = 0);
    ~FliteSettingsDialog();
    unsigned int getDuration();
    unsigned int getTarget();
    void resetDialog();

private slots:
    void checkBoxChanged();
    void updateDuration();
    void updateTarget();

    void on_okButton_clicked();

private:
    Ui::FliteSettingsDialog *ui;
    unsigned int duration;
    unsigned int target;
    void updateDurationLabel();
    void updateTargetLabel();
};

#endif // FLITESETTINGSDIALOG_H
