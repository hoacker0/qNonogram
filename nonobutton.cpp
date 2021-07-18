#include "nonobutton.h"
#include "defines.h"

nonobutton_base::nonobutton_base(QWidget *parent) : QPushButton(parent)
{
    setMinimumSize(DEFAULT_BUTTON_SIZE, DEFAULT_BUTTON_SIZE);
    setMaximumSize(DEFAULT_BUTTON_SIZE, DEFAULT_BUTTON_SIZE);
}

void nonobutton_base::paint(QColor color, QString marker)
{
    //Set marker char and paint button background
    if (marker != NULL) {
        setText(marker);
    }
    setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));

}

// button & first are pointers to the parent's variables. These are used to
// convey information that the parent needs. "button" for which mouse button
// was pressed, and "first" for whether this was the first operation in
// a "click and drag" operation or not.
nonobutton::nonobutton(QWidget *parent, int *b, bool *f) : nonobutton_base(parent), button(b), first(f)
{
	setAcceptDrops(true);
    setStyleSheet("background-color: rgb(255, 255, 255)");
    setMinimumSize(DEFAULT_BUTTON_SIZE, DEFAULT_BUTTON_SIZE);
    setMaximumSize(DEFAULT_BUTTON_SIZE, DEFAULT_BUTTON_SIZE);
	processed = false;
}

void nonobutton::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton && e->button() != Qt::RightButton) {
		return;
	}
	*(button) = e->button();
	*(first) = true;
	processed = true;
	if (e->button() == Qt::LeftButton) {
		emit solid();
	}
	else {
		emit dot();
	}
	// Qt will clean up the QDrag and associated QMimeData objects, so they mustn't be deleted.
	QDrag *drag = new QDrag(this);
	QMimeData *md = new QMimeData();
	drag->setMimeData(md);
	drag->exec(Qt::IgnoreAction);
	// After a drag action the released signal isn't auto-emitted, but we need it.
	emit released();
}

void nonobutton::dragEnterEvent(QDragEnterEvent *e)
{
	if (processed) {
		processed = false;
		return;
	}
	if (*(button) == Qt::LeftButton) {
		emit solid();
	}
	else {
		emit dot();
    }
}
