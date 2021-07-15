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

void MainWindow::help()
{
    bool todo;
}

void MainWindow::about()
{
    bool todo;
}
