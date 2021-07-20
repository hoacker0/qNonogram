#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QMimeData>

class nonobutton_base : public QPushButton {
    Q_OBJECT

public:
    nonobutton_base(QWidget *parent);
    void paint(QColor color, QString marker = NULL);
    void setSize(int size);

};

class nonobutton : public nonobutton_base {
    Q_OBJECT

public:
    nonobutton(QWidget *parent, int *b, bool *f);

private:
   int *button;
   bool *first;
   bool processed;

protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void mousePressEvent(QMouseEvent *e);
	
 signals:
	void solid();
	void dot();
};
#endif
