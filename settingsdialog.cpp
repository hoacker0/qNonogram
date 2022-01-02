#include "settingsdialog.h"
#include "defines.h"
#include <QColorDialog>

settingsDialog::settingsDialog(QWidget *parent, int w, int h, qnono::paintValues pVals, int cm)
    : QDialog(parent), width(w), height(h), paintValues(pVals)
{

    // Sizes
    widthBox = new QSpinBox(this);
    heightBox = new QSpinBox(this);
    widthBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    heightBox->setRange(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
    widthBox->setValue(width);
    heightBox->setValue(height);
    heightLabel = new QLabel(tr("Default Rows"), this);
    widthLabel = new QLabel(tr("Columns"), this);

    QHBoxLayout *sizeBoxLayout = new QHBoxLayout;
    sizeBoxLayout->addWidget(heightLabel);
    sizeBoxLayout->addWidget(heightBox);
    sizeBoxLayout->addWidget(widthLabel);
    sizeBoxLayout->addWidget(widthBox);

    // Click mode
    clickModeLabel = new QLabel(tr("Click mode"), this);
    clickModeCombo = new QComboBox(this);
    clickModeCombo->addItem(tr("left and right click"));
    clickModeCombo->addItem(tr("left click or tap"));
    clickModeCombo->setCurrentIndex(cm);

    QHBoxLayout *clickModeLayout = new QHBoxLayout;
    clickModeLayout->addWidget(clickModeLabel);
    clickModeLayout->addWidget(clickModeCombo);

    // Field size
    fieldSizeBox = new QSpinBox(this);
    fieldSizeBox->setRange(15, 40);
    fieldSizeBox->setValue(paintValues.field_size);
    fieldSizeLabel = new QLabel(tr("Puzzle field size: "), this);
    connect(fieldSizeBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &settingsDialog::setFieldSize);

    // Colors and DOT-Markers
    colorSolidLabel = new QLabel(tr("\"Solid\" color"), this);
    colorSolidButton = new nonobutton_base(this);
    QPushButton::connect(colorSolidButton, &QPushButton::released, this, &settingsDialog::setSolid);

    colorBlankLabel = new QLabel(tr("\"Blank\" mark color"), this);
    colorBlankButton = new nonobutton_base(this);
    QPushButton::connect(colorBlankButton, &QPushButton::released, this, &settingsDialog::setBlank);

    colorUndecidedLabel = new QLabel(tr("Undecided field color"), this);
    colorUndecidedButton = new nonobutton_base(this);
    QPushButton::connect(colorUndecidedButton, &QPushButton::released, this, &settingsDialog::setUndecided);

    colorHintSolidLabel = new QLabel(tr("Hint \"Solid\" color"), this);
    colorHintSolidButton = new nonobutton_base(this);
    QPushButton::connect(colorHintSolidButton, &QPushButton::released, this, &settingsDialog::setHintSolid);

    colorHintBlankLabel = new QLabel(tr("Hint \"Blank\" color"), this);
    colorHintBlankButton = new nonobutton_base(this);
    QPushButton::connect(colorHintBlankButton, &QPushButton::released, this, &settingsDialog::setHintBlank);

    sizeColorButtons(pVals.field_size);
    paintColorButtons();

    int row = 0;
    QGridLayout *optionsGrid = new QGridLayout;
    optionsGrid->addWidget(fieldSizeLabel,row,0,1,1);
    optionsGrid->addWidget(fieldSizeBox,row++,1,1,1);
    optionsGrid->addWidget(colorSolidLabel,row,0,1,1);
    optionsGrid->addWidget(colorSolidButton,row++,1,1,1);
    optionsGrid->addWidget(colorBlankLabel,row,0,1,1);
    optionsGrid->addWidget(colorBlankButton,row++,1,1,1);
    optionsGrid->addWidget(colorUndecidedLabel,row,0,1,1);
    optionsGrid->addWidget(colorUndecidedButton,row++,1,1,1);
    optionsGrid->addWidget(colorHintSolidLabel,row,0,1,1);
    optionsGrid->addWidget(colorHintSolidButton,row++,1,1,1);
    optionsGrid->addWidget(colorHintBlankLabel,row,0,1,1);
    optionsGrid->addWidget(colorHintBlankButton,row++,1,1,1);

    // Reset button
    resetButton = new QPushButton(tr("Reset to defaults"),this);
    connect(resetButton, &QPushButton::released, this, &settingsDialog::reset);

    // Buttonbox
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Assemble layout
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addLayout(sizeBoxLayout);
    dialogLayout->addLayout(optionsGrid);
    dialogLayout->addLayout(clickModeLayout);
    dialogLayout->addWidget(resetButton);
    dialogLayout->addWidget(buttonBox);

    setLayout(dialogLayout);

    setWindowTitle(tr("Settings"));
}

settingsDialog::~settingsDialog()
{
}

void settingsDialog::sizeColorButtons(int size)
{
    colorSolidButton->setSize(size);
    colorBlankButton->setSize(size);
    colorUndecidedButton->setSize(size);
    colorHintSolidButton->setSize(size);
    colorHintBlankButton->setSize(size);

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

void settingsDialog::setFieldSize()
{
    paintValues.field_size = fieldSizeBox->value();
    sizeColorButtons(paintValues.field_size);
}

void settingsDialog::setClickMode(int cm)
{
    clickModeCombo->setCurrentIndex(cm);
}

int settingsDialog::getClickMode()
{
    return clickModeCombo->currentIndex();
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

    fieldSizeBox->setValue(DEFAULT_FIELD_SIZE);

    paintColorButtons();
}

int settingsDialog::sWidth() { return widthBox->value(); }
int settingsDialog::sHeight() { return heightBox->value(); }
qnono::paintValues settingsDialog::sPaintValues() {return paintValues; }
