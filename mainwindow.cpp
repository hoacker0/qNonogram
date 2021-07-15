#include "mainwindow.h"
#include <QMessageBox>
#include "dimensiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // Create game object
    game = new nonogame(this);
    connect(game, SIGNAL(solved()), SLOT(solved()));
    connect(game, SIGNAL(setUndo(bool)), SLOT(setUndo(bool)));
    connect(game, SIGNAL(setRedo(bool)), SLOT(setRedo(bool)));

    // Layout and Menues
    QWidget *nonowidget = new QWidget;
    setCentralWidget(nonowidget);

    createMenu();
    createLayout();

    nonowidget->setLayout(mainLayout);

    // set GUI to neutral state
    restartAction->setEnabled(false);
    solutionShown = false;

}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenu()
{

    gameMenu = menuBar()->addMenu(tr("&Game"));

    newAction = new QAction(tr("&New game"), this);
    connect(newAction, &QAction::triggered, this, &MainWindow::newGame);
    gameMenu->addAction(newAction);

    restartAction = new QAction(tr("&Restart game"), this);
    connect(restartAction, &QAction::triggered, this, &MainWindow::restartGame);
    gameMenu->addAction(restartAction);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quit);
    gameMenu->addAction(quitAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));

    helpAction = new QAction(tr("&Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::help);
    helpMenu->addAction(helpAction);

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(aboutAction);

}

void MainWindow::createLayout()
{

    mainLayout = new QVBoxLayout;

    // Top
    undoButton = new QPushButton(this);
    undoButton->setIcon(QIcon(":/undo.png"));
    redoButton = new QPushButton(this);
    redoButton->setIcon(QIcon(":/redo.png"));
    undoButton->setEnabled(false);
    redoButton->setEnabled(false);

    toggleSolutionButton = new QPushButton(tr("Show solution"), this);
    toggleSolutionButton->setEnabled(false);

    top = new QHBoxLayout();
    top->setAlignment(Qt::AlignLeft);
    top->addWidget(undoButton);
    connect(undoButton, &QPushButton::released, this, &MainWindow::undo);
    top->addWidget(redoButton);
    connect(redoButton, &QPushButton::released, this, &MainWindow::redo);
    top->addWidget(redoButton);
    connect(redoButton, &QPushButton::released, this, &MainWindow::redo);
    top->addWidget(toggleSolutionButton);
    connect(toggleSolutionButton, &QPushButton::released, this, &MainWindow::toggleSolution);

    // Final assembly
    //mainLayout->addSpacing(20);
    mainLayout->addLayout(top);
    mainLayout->addLayout(game->grid());

}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    if ( (event->modifiers() & Qt::ControlModifier)
        && (event->modifiers() & Qt::ShiftModifier)
        && (event->key() == Qt::Key_Z) ) {
        // CTRL+SHIFT+Z
        redo();
        return;
    }

    if ( (event->modifiers() & Qt::ControlModifier)
        && (event->key() == Qt::Key_Z) ) {
        // CTRL+Z
        undo();
        return;
    }

    // No match, pass on event
    QMainWindow::keyPressEvent(event);
}

// SLOTs
void MainWindow::solved()
{
    QMessageBox mb;
    mb.setWindowTitle(tr("Well done"));
    mb.setText(tr("You have solved the puzzle!"));
    mb.exec();
}

void MainWindow::setUndo(bool status)
{
    undoButton->setEnabled(status);
}

void MainWindow::setRedo(bool status)
{
    redoButton->setEnabled(status);
}


void MainWindow::undo()
{
    game->undo();
}

void MainWindow::redo()
{
    game->redo();
}

// Toggle Solution shown/hidden
void MainWindow::toggleSolution() {

    if (solutionShown) {
        game->hideSolution();
        toggleSolutionButton->setText(tr("Show solution"));
    } else {
        game->showSolution();
        toggleSolutionButton->setText(tr("Hide solution"));
    }
    solutionShown = !solutionShown;
    return;
}

void MainWindow::newGame()
{

    // if game running, ask before starting new
    if (game->running()) {
        QMessageBox msgBox;
        msgBox.setText(tr("A game is running. Quit game and start new?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(
        QMessageBox::Cancel);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
    }

    // Ask user for game size
    dimensionDialog dimDialog(this);
    dimDialog.exec();

    // new game
    game->newGame(dimDialog.pWidth(), dimDialog.pHeight());
    restartAction->setEnabled(true);
    toggleSolutionButton->setEnabled(true);

}

void MainWindow::restartGame()
{
    // ask before clearing
    QMessageBox msgBox;
    msgBox.setText(tr("Restart game?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok)
    {
        // restart
        game->restart();
    }
}

void MainWindow::quit() {
    exit(0);
}

void MainWindow::about() {
    QMessageBox mb;
    mb.setWindowTitle(tr("About"));
    mb.setText(tr("<p><h3>qNonogram 0.1</h3></p><p>Copyright: Holger Ackermann, 2021.<br>\
 Based on nonogram-qt by Daniel Suni.</p>\
<p>Distributed under the GNU GPL v3</p>"));
    mb.exec();
}

void MainWindow::help() {
    QWidget *helpWindow = new QWidget();
    helpWindow->setWindowTitle(tr("Help"));
    QHBoxLayout *bl = new QHBoxLayout(helpWindow);
    QLabel *text = new QLabel(tr("<h2>What are nonograms?</h2>\
<p>Nonograms are logic puzzles consisting of a rectangular grid divided into cells. These cells can be either<br>\
colored (solids) or blank (X). At the start of the game all cells are blank, and the purpose of the game is<br>\
to figure out which ones should be colored.</p>\
<p>Each row and column in the grid is fitted with a clue consisting of a series of numbers. These numbers reveal<br>\
how many solids there are on the row/column as well as something about their grouping. If e.g. the clue is<br>\
(3 1 2) we know that from left to right there is first a series of 3 consequtive solids, then X number of blanks,<br>\
where X >= 1, then a single solid, then another unspecified number of blanks, and finally 2 consequtive solids.<br>\
If the row was 10 cells long, one possible arrangement would be (-###-#--##), another one would be (###-#-##--).<br>\
Since there are rules for both rows and columns, only one arrangement is actually correct, though. The puzzle is<br>\
to find the arrangement that conforms to all the given clues.</p>\
<h2>How to play</h2>\
<p>In order to start a new game, please select the size of the desired playing field, and click the &quot;Generate puzzle&quot;<br>\
button. Generating the puzzle can take a few seconds - especially if it's a big one, so please be patient.</p>\
<p>When starting all cells are blank. You can mark a cell as a solid by clicking on it. You can also mark larger areas<br>\
by dragging the mouse with the button pressed. If you've made a mistake, just click the cell again to revert it to a<br>\
blank. You can also mark known blanks by right clicking (and dragging). This will be shown by an 'X' appearing in that<br>\
cell. Notice that you don't need to explicitly mark the blanks in order to solve the puzzle. That functionality only<br>\
exists for your own convenience.</p>\
<p>Every time you make a move the computer will automatically check whether you've successfully solved the puzzle<br>\
 or not. Once the puzzle is solved, you will immediately be informed. If the puzzle turns out to be too hard, you can<br>\
end it, and look at the solution by pressing the &quot;Give up&quot; button. Remaining solids will be displayed in<br>\
grey, and possible mistakes (i.e. blanks marked as solids) will be displayed in red.</p>"), helpWindow);
    helpWindow->setLayout(bl);
    bl->addWidget(text);
    helpWindow->show();
}
