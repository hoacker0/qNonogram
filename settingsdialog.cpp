#include "settingsdialog.h"
#include "defines.h"
#include <QColorDialog>

settingsDialog::settingsDialog(QWidget *parent, int w, int h, qnono::paintValues pVals)
    : QDialog(parent), width(w), height(h), paintValues(pVals)
{

    // Sizes
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

    // Colors and DOT-Markers
    style = new QCommonStyle();

    colorSolidLabel = new QLabel(tr("\"Solid\" color"), this);
    colorSolidButton = new nonobutton_base(this);
    colorSolidButton->setStyle(style);
    QPushButton::connect(colorSolidButton, &QPushButton::released, this, &settingsDialog::setSolid);

    colorBlankLabel = new QLabel(tr("\"Blank\" mark color"), this);
    colorBlankButton = new nonobutton_base(this);
    colorBlankButton->setStyle(style);
    QPushButton::connect(colorBlankButton, &QPushButton::released, this, &settingsDialog::setBlank);

    colorUndecidedLabel = new QLabel(tr("Undecided field color"), this);
    colorUndecidedButton = new nonobutton_base(this);
    colorUndecidedButton->setStyle(style);
    QPushButton::connect(colorUndecidedButton, &QPushButton::released, this, &settingsDialog::setUndecided);

    colorHintSolidLabel = new QLabel(tr("Hint \"Solid\" color"), this);
    colorHintSolidButton = new nonobutton_base(this);
    colorHintSolidButton->setStyle(style);
    QPushButton::connect(colorHintSolidButton, &QPushButton::released, this, &settingsDialog::setHintSolid);

    colorHintBlankLabel = new QLabel(tr("Hint \"Blank\" color"), this);
    colorHintBlankButton = new nonobutton_base(this);
    colorHintBlankButton->setStyle(style);
    QPushButton::connect(colorHintBlankButton, &QPushButton::released, this, &settingsDialog::setHintBlank);

    paintColorButtons();

    QGridLayout *colorGrid = new QGridLayout;
    colorGrid->addWidget(colorSolidLabel,0,0,1,1);
    colorGrid->addWidget(colorSolidButton,0,1,1,1);
    colorGrid->addWidget(colorBlankLabel,1,0,1,1);
    colorGrid->addWidget(colorBlankButton,1,1,1,1);
    colorGrid->addWidget(colorUndecidedLabel,2,0,1,1);
    colorGrid->addWidget(colorUndecidedButton,2,1,1,1);
    colorGrid->addWidget(colorHintSolidLabel,3,0,1,1);
    colorGrid->addWidget(colorHintSolidButton,3,1,1,1);
    colorGrid->addWidget(colorHintBlankLabel,4,0,1,1);
    colorGrid->addWidget(colorHintBlankButton,4,1,1,1);

    // Reset button
    resetButton = new QPushButton(tr("Reset to defaults"),this);
    connect(resetButton, &QPushButton::released, this, &settingsDialog::reset);

    // Buttonbox
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Assemble layout
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addWidget(sizeLabel);
    dialogLayout->addLayout(sizeBoxLayout);
    dialogLayout->addLayout(colorGrid);
    dialogLayout->addWidget(resetButton);
    dialogLayout->addWidget(buttonBox);

    setLayout(dialogLayout);

    setWindowTitle(tr("Settings"));
}

settingsDialog::~settingsDialog()
{
    delete style;
}

void settingsDialog::paintColorButtons()
{
    colorSolidButton->paint(paintValues.solid);
    colorBlankButton->paint(paintValues.blank, "X");
    colorUndecidedButton->paint(paintValues.undecided);
    colorHintSolidButton->paint(paintValues.hint_solid);
    colorHintBlankButton->paint(paintValues.hint_blank);

}

QColor settingsDialog::getColor(QColor pColor)
{
    QColor color = QColorDialog::getColor(pColor, this );
    if (color.isValid())
    {
        return color;
    }
    return pColor;
}

void settingsDialog::setSolid()
{
    paintValues.solid = getColor(paintValues.solid);
    colorSolidButton->paint(paintValues.solid);
}

void settingsDialog::setBlank()
{
    paintValues.blank = getColor(paintValues.blank);
    colorBlankButton->paint(paintValues.blank);
}

void settingsDialog::setUndecided()
{
    paintValues.undecided = getColor(paintValues.undecided);
    colorUndecidedButton->paint(paintValues.undecided);
}

void settingsDialog::setHintSolid()
{
    paintValues.hint_solid = getColor(paintValues.hint_solid);
    colorHintSolidButton->paint(paintValues.hint_solid);
}

void settingsDialog::setHintBlank()
{
    paintValues.hint_blank = getColor(paintValues.hint_blank);
    colorHintSolidButton->paint(paintValues.hint_blank);
}

void settingsDialog::reset()
{
    widthBox->setValue(DEFAULT_PUZZLE_SIZE);
    heightBox->setValue(DEFAULT_PUZZLE_SIZE);

    paintValues.solid = DEFAULT_COLOR_SOLID;
    paintValues.blank = DEFAULT_COLOR_BLANK;
    paintValues.undecided = DEFAULT_COLOR_UNDECIDED;
    paintValues.hint_solid = DEFAULT_COLOR_HINT_SOLID;
    paintValues.hint_blank = DEFAULT_COLOR_HINT_BLANK;

    paintColorButtons();
}

int settingsDialog::sWidth() { return widthBox->value(); }
int settingsDialog::sHeight() { return heightBox->value(); }
qnono::paintValues settingsDialog::sPaintValues() {return paintValues; }
