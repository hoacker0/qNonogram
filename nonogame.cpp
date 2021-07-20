#include "nonogame.h"
#include <QPalette>

nonogame::nonogame(QObject* gameParent)
{
    gameGrid = new QGridLayout();
    gameGrid->setSpacing(0);
    gameGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    gameEngine = NULL;

    parent = gameParent;
    style = new QCommonStyle();

    firstClick= false;
    solutionShown = false;
    isRunning = false;

}

nonogame::~nonogame()
{
    cleanUp();
    delete style;
    delete gameGrid;
}

QGridLayout* nonogame::grid()
{
    return gameGrid;
}

void nonogame::setPaintValues(qnono::paintValues paintVals)
{
    paintValues = paintVals;
    if (isRunning) {
        repaintGrid();
    }
}

void nonogame::newGame(int gameWidth, int gameHeight)
{
    if (gameEngine)
    {
        cleanUp();
    }

    width = gameWidth;
    height = gameHeight;

    startPuzzle();
    isRunning = true;

}

void nonogame::clearGrid()
{
    //QLayout *gridLayout = gameGrid->layout();
    if (gameGrid) {
      // Process all child items recursively.
      int itemCount = gameGrid->count();
      for (int i = 0; i < itemCount; i++) {
          if (gameGrid->itemAt(i)->widget()) {
              delete gameGrid->itemAt(i)->widget();
          }
          if (gameGrid->itemAt(i)->layout()) {
              delete gameGrid->itemAt(i)->layout();
          }
          gameGrid->removeItem(gameGrid->itemAt(i));
      }

    }
}

// Generates, verifies and displays the gameEngine and its graphical components.
void nonogame::startPuzzle() {
    int pos, spacer_x, spacer_y;

    clearGrid();

    gameEngine = new nonoengine(width, height);
    xAxisClue = gameEngine->getXAxis();
    yAxisClue = gameEngine->getYAxis();
    nonosolver *gameSolver = new nonosolver(width, height, xAxisClue, yAxisClue);

    // Retry gameEngine creation until we get a solvable one.
    while (!gameSolver->solve()) {
        delete gameSolver;
        delete gameEngine;
        gameEngine = new nonoengine(width, height);
        xAxisClue = gameEngine->getXAxis();
        yAxisClue = gameEngine->getYAxis();
        gameSolver = new nonosolver(width, height, xAxisClue, yAxisClue);
    }
    delete gameSolver;

    // Create and add the clue labels
    spacer_x = 0;
    spacer_y = 0;
    for (int i = 0; i < width; ++i) {
        QString str = "";
        QString num = "";
        for (size_t j = 0; j < xAxisClue[i]->size(); ++j) {
            num.setNum(xAxisClue[i]->at(j), 10);
            str.append(num);
            if (j < xAxisClue[i]->size() - 1) {
                str.append("\n");
            }
        }
        xAxis.push_back(new QLabel(str));
        xAxis.at(i)->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
        // We want to separate the UI buttons in 5 x 5 chunks, so that counting
        // blocks becomes easier for the user.
        if (i > 0 && i % 5 == 0) {
            ++spacer_x;
            gameGrid->setColumnMinimumWidth(i + spacer_x, 2);
        }
        gameGrid->addWidget(xAxis.at(i), 0, i + spacer_x + 1);
    }
    for (int i = 0; i < height; ++i) {
        QString str = "";
        QString num = "";
        for (size_t j = 0; j < yAxisClue[i]->size(); ++j) {
            num.setNum(yAxisClue[i]->at(j), 10);
            str.append(num);
            str.append("  ");
        }
        yAxis.push_back(new QLabel(str));
        yAxis.at(i)->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        if (i > 0 && i % 5 == 0) {
            ++spacer_y;
            gameGrid->setRowMinimumHeight(i + spacer_y, 2);
        }
        gameGrid->addWidget(yAxis.at(i), i + spacer_y + 1, 0);
    }

    // We need to map the clicks and right clicks to the respective methods.
    // Using mappers allows us to easily identify the button that sent the signal.
    mapperLeftButton = new QSignalMapper(this);
    mapperRightButton = new QSignalMapper(this);

    // Create the playing field itself.
    spacer_y = 0;
    for (int i = 0; i < height; ++i) {
        if (i > 0 && i % 5 == 0) {
            ++spacer_y;
        }
        spacer_x = 0;
        for (int j = 0; j < width; ++j) {
            if (j > 0 && j % 5 == 0) {
                ++spacer_x;
            }
            pos = i * width + j;
            status.push_back(STATUS_UNDECIDED);
            puzzle.push_back(new nonobutton(NULL, &mouseButton, &firstClick));
            puzzle.at(pos)->setStyle(style);
            puzzle.at(pos)->setSize(paintValues.field_size);
            puzzle.at(pos)->paint(paintValues.undecided);
            QObject::connect(puzzle.at(pos), SIGNAL(solid()), mapperLeftButton, SLOT(map()));
            QObject::connect(puzzle.at(pos), SIGNAL(dot()), mapperRightButton, SLOT(map()));
            QObject::connect(puzzle.at(pos), SIGNAL(released()), this, SLOT(checkSolution()));
            mapperLeftButton->setMapping(puzzle.at(pos), pos);
            mapperRightButton->setMapping(puzzle.at(pos), pos);
            gameGrid->addWidget(puzzle.at(pos), i + spacer_y + 1, j + spacer_x + 1);
        }
    }
    QObject::connect(mapperLeftButton, SIGNAL(mapped(int)), this, SLOT(leftClicked(int)));
    QObject::connect(mapperRightButton, SIGNAL(mapped(int)), this, SLOT(rightClicked(int)));
}

// Called when left button is clicked or dragged over with button depressed.
void nonogame::leftClicked(int position) {

    // If no game currently running, do nothing
    if (!isRunning) return;

    if (firstClick) {
        // single click or first click in draw action
        // determine new status and keep it until new "firstClick"
        if (status.at(position) == STATUS_SOLID) {
            currentStatus = STATUS_UNDECIDED;
        }
        else {
            currentStatus = STATUS_SOLID;
        }
        firstClick = false;
    }

    // set new status at position
    setStatus(position, currentStatus, true);
    return;
}

// Called when right button is clicked or dragged over with button depressed.
void nonogame::rightClicked(int position) {

    // If no game currently running, do nothing
    if (!isRunning) return;

    if (firstClick) {
        // single click or first click in draw action
        // determine new status and keep it until new "firstClick"
        if (status.at(position) == STATUS_BLANK) {
            currentStatus = STATUS_UNDECIDED;
        }
        else {
            currentStatus = STATUS_BLANK;
        }
        firstClick = false;
    }

    // set new status at position
    setStatus(position, currentStatus, true);
    return;
}

// Check whether the puzzle is solved or not. Display a message if it is.
void nonogame::checkSolution() {
    // Since this function is called when the mouse button is released,
    // we use it to unset the lockAction variable.
    for (int i = 0; i < height; ++i) {
        size_t test = 0;
        size_t mask = 1 << width;
        for (int j = 0; j < width; ++j) {
            mask >>= 1;
            if (status.at(i * width + j) == STATUS_SOLID) {
                test |= mask;
            }
        }
        if (test != gameEngine->getField()[i]) {
            return;
        }
    }
    for (int i = 0; i < width * height; ++i) {
        puzzle.at(i)->setEnabled(false);
    }
    isRunning = false;
    emit solved();
}

void nonogame::repaintGrid() {
    int position;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            position = i * width + j;
            paintPosition(position, status.at(position));
            puzzle.at(position)->setSize(paintValues.field_size);
        }
    }
}


// Hide the solution (see ShowSolution) again and show only user marks
void nonogame::hideSolution() {
    repaintGrid();
}


// Display all solids that the user has missed, in grey and all
// blanks he has mistakenly identified as solids in red.
void nonogame::showSolution() {
    int pos;
    for (int i = 0; i < height; ++i) {
        size_t mask = 1 << width;
        for (int j = 0; j < width; ++j) {
            mask >>= 1;
            pos = i * width + j;
            if (status.at(pos) == STATUS_SOLID) {
                if (!(gameEngine->getField()[i] & mask)) {
                    paintPosition(pos, STATUS_HINT_BLANK);
                }
            }
            else if (gameEngine->getField()[i] & mask) {
                paintPosition(pos, STATUS_HINT_SOLID);
            }
        }
    }
}

void nonogame::restart() {
    int position;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            position = i * width + j;
            status.at(position) = STATUS_UNDECIDED;
            paintPosition(position, status.at(position));
        }
    }
}

void nonogame::addUndoStep(int position) {
    undoStack.push(pStatus{position, status.at(position)});
    emit setUndo(true);
    return;
}


void nonogame::addRedoStep(int position) {
    redoStack.push(pStatus{position, status.at(position)});
    emit setRedo(true);
    return;
}

void nonogame::undo() {

    // If no game currently running, do nothing
    if (!isRunning) return;

    // If nothing to undo, do nothing
    if (undoStack.isEmpty()) return;

    pStatus pState = undoStack.pop();
    addRedoStep(pState.position);
    setStatus(pState.position, pState.status, false);

    emit setUndo(!undoStack.isEmpty());

    return;
}

void nonogame::redo() {

    // If no game currently running, do nothing
    if (!isRunning) return;

    // If nothing to redo, do nothing
    if (redoStack.isEmpty()) return;

    pStatus pState = redoStack.pop();
    addUndoStep(pState.position);
    setStatus(pState.position, pState.status, false);

    emit setRedo(!redoStack.isEmpty());

    return;
}

void nonogame::paintPosition(int position, int state)  {

    switch (state) {
    case STATUS_UNDECIDED:
        puzzle.at(position)->paint(paintValues.undecided, " ");
        break;
    case STATUS_SOLID:
        puzzle.at(position)->paint(paintValues.solid, " ");
        break;
    case STATUS_BLANK:
        puzzle.at(position)->paint(paintValues.blank, "X");
        break;
    case STATUS_HINT_BLANK:
        puzzle.at(position)->paint(paintValues.hint_blank);
        break;
    case STATUS_HINT_SOLID:
        puzzle.at(position)->paint(paintValues.hint_solid);
        break;
    }

    return;
}

void nonogame::setStatus(int position, int state, bool addUndo) {

    // Do do anything if status is already set
    if (status.at(position) == state) return;

    // only add undo if clicked()
    // do not add undo if called from undo/redo
    if (addUndo) {
        addUndoStep(position);
        redoStack.clear();
    }

    status.at(position) = state;
    paintPosition(position, state);
    return;
}

// Cleans up stuff before a new game is started
void nonogame::cleanUp() {
    delete gameEngine;
    gameEngine = NULL;
    for (size_t i = 0; i < xAxis.size(); ++i) {
        delete xAxis.at(i);
    }
    xAxis.clear();
    for (size_t i = 0; i < yAxis.size(); ++i) {
        delete yAxis.at(i);
    }
    yAxis.clear();
    for (int i = 0; i < width * height; ++i) {
        delete puzzle.at(i);
    }
    puzzle.clear();
    delete mapperLeftButton;
    delete mapperRightButton;
    status.clear();
    solutionShown = false;
}
