#include "settingsdialog.h"
#include "defaults.h"

settingsDialog::settingsDialog(QWidget *parent, int w, int h) : QDialog(parent), width(w), height(h)
{

    widthBox = new QSpinBox(this);
    heightBox = new QSpinBox(this);
    widthBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    heightBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    widthBox->setValue(width);
    heightBox->setValue(height);
    sizeLabel = new QLabel(tr("Default Puzzle Size: "), this);
    heightLabel = new QLabel(tr("Rows"), this);
    widthLabel = new QLabel(tr("Columns"), this);

    QHBoxLayout *sizeBoxLayout = new QHBoxLayout;
    sizeBoxLayout->addWidget(heightLabel);
    sizeBoxLayout->addWidget(heightBox);
    sizeBoxLayout->addWidget(widthLabel);
    sizeBoxLayout->addWidget(widthBox);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addWidget(sizeLabel);
    dialogLayout->addLayout(sizeBoxLayout);
    dialogLayout->addWidget(buttonBox);

    setLayout(dialogLayout);

    setWindowTitle(tr("Settings"));
}

int settingsDialog::sWidth() { return widthBox->value(); }
int settingsDialog::sHeight() { return heightBox->value(); }
