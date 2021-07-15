#include "dimensiondialog.h"

dimensionDialog::dimensionDialog(QWidget *parent) : QDialog(parent)
{

    widthBox = new QSpinBox(this);
    heightBox = new QSpinBox(this);
    widthBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    heightBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    widthBox->setValue(DEFAULT_PUZZLE_SIZE);
    heightBox->setValue(DEFAULT_PUZZLE_SIZE);
    heightLabel = new QLabel(tr("Rows"), this);
    widthLabel = new QLabel(tr("Columns"), this);

    QHBoxLayout *sizeBoxLayout = new QHBoxLayout;
    sizeBoxLayout->addWidget(heightLabel);
    sizeBoxLayout->addWidget(heightBox);
    sizeBoxLayout->addWidget(widthLabel);
    sizeBoxLayout->addWidget(widthBox);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addLayout(sizeBoxLayout);
    dialogLayout->addWidget(buttonBox);

    setLayout(dialogLayout);

    setWindowTitle(tr("Puzzle dimensions"));
}

int dimensionDialog::pWidth()
{
    return widthBox->value();
}

int dimensionDialog::pHeight()
{
    return heightBox->value();
}
