#ifndef NONOGAME_H
#define NONOGAME_H

#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QCommonStyle>
#include <QSignalMapper>
#include <QStack>

#include <vector>

#include "nonobutton.h"
#include "nonoengine.h"
#include "defines.h"

struct pStatus {
    int position;
    int status;
};

class nonogame: public QObject
{

Q_OBJECT

public:
    nonogame(QObject* gameParent);
    ~nonogame();

    QGridLayout* grid();
    void newGame(int gameWidth, int gameHeight);
    void setPaintValues(qnono::paintValues paintVals);
    bool running() {return isRunning;}
    void startPuzzle();
    void cleanUp();
    void repaintGrid();
    void hideSolution();
    void showSolution();
    void restart();
    void undo();
    void redo();
    void setClickMode(int cm) {clickMode = cm;};
    void setClickStatus(int cs) {currentStatus = cs;};

private:
    QObject* parent;
    QGridLayout *gameGrid;
    QCommonStyle *style;
    QSignalMapper *mapperLeftButton;
    QSignalMapper *mapperRightButton;

    qnono::paintValues paintValues;

    nonoengine *gameEngine;
    nonosolver *gameSolver;

    QStack<pStatus> undoStack;
    QStack<pStatus> redoStack;

    void setStatus(int position, int state, bool addUndo);
    void paintPosition(int position, int state);
    void clearGrid();

    void addUndoStep(int position);
    void addRedoStep(int position);

    bool firstClick, solutionShown, isRunning;
    int width, height;
    int mouseButton, currentStatus;
    int clickMode;

    vector<QLabel*> xAxis, yAxis;
    vector<size_t> **xAxisClue;
    vector<size_t> **yAxisClue;
    vector<int> status;
    vector<nonobutton*> puzzle;

private slots:
    void leftClicked(int position);
    void rightClicked(int position);
    void checkSolution();

signals:
   void solved();
   void setUndo(bool status);
   void setRedo(bool status);
};

#endif // NONOGAME_H
