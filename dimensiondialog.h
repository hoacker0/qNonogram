#ifndef DIMENSIONDIALOG_H
#define DIMENSIONDIALOG_H

#define MIN_PUZZLE_SIZE 3
#define MAX_PUZZLE_SIZE 30
#define DEFAULT_PUZZLE_SIZE 20

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QDialogButtonBox>

class dimensionDialog : public QDialog        
{
    Q_OBJECT

public:
    dimensionDialog(QWidget *parent);
    int pWidth();
    int pHeight();

private:
    QHBoxLayout *sizeBoxLayout;
    QVBoxLayout *dialogLayout;
    QLabel *heightLabel;
    QSpinBox *widthBox;
    QLabel *widthLabel;
    QSpinBox *heightBox;
    QDialogButtonBox *buttonBox;
};

#endif // DIMENSIONDIALOG_H
