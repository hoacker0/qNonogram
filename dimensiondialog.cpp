#include "dimensiondialog.h"
#include "defines.h"

dimensionDialog::dimensionDialog(QWidget *parent, int *w, int *h) : QDialog(parent), width(w), height(h)
{

    widthBox = new QSpinBox(this);
    heightBox = new QSpinBox(this);
    widthBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    heightBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    widthBox->setValue(*width);
    heightBox->setValue(*height);
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
