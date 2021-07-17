#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QDialogButtonBox>

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    settingsDialog(QWidget *parent, int w, int h);
    int width, height;
    int sWidth();
    int sHeight();

private:
    QHBoxLayout *sizeBoxLayout;
    QVBoxLayout *dialogLayout;
    QLabel *sizeLabel;
    QLabel *heightLabel;
    QSpinBox *widthBox;
    QLabel *widthLabel;
    QSpinBox *heightBox;
    QDialogButtonBox *buttonBox;
};

#endif // SETTINGSDIALOG_H
