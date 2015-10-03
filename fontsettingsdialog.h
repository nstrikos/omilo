#ifndef FONTSETTINGSDIALOG_H
#define FONTSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class FontSettingsDialog;
}

class FontSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontSettingsDialog(QWidget *parent = 0);
    ~FontSettingsDialog();
    QFont getFont();
    unsigned int getPointSize();

private slots:
    void okButtonClicked();
    void cancelButtonClicked();

private:
    Ui::FontSettingsDialog *ui;
    QFont font;
    unsigned int pointSize;
};

#endif // FONTSETTINGSDIALOG_H
