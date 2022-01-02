#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include "nonobutton.h"
#include "nonogame.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);

private:
    QWidget *nonowidget;

    QMenuBar *menu;
    QMenu *gameMenu;
    QMenu *helpMenu;
    QAction *newAction;
    QAction *restartAction;
    QAction *settingsAction;
    QAction *quitAction;
    QAction *helpAction;
    QAction *aboutAction;

    QVBoxLayout *mainLayout;
    QHBoxLayout *top;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QPushButton *toggleSolutionButton;

    nonobutton_base *paintSolid;
    nonobutton_base *paintEmpty;
    nonobutton_base *paintUndecided;

    nonogame *game;
    int gameWidth, gameHeight;
    int defaultWidth, defaultHeight;
    int clickMode;

    qnono::paintValues paintValues;

    bool solutionShown;

    void showStateSelectors(bool show);
    void createMenu();
    void createLayout();
    void loadSettings();
    void saveSettings();


private slots:
    void solved();
    void setUndo(bool status);
    void setRedo(bool status);
    void undo();
    void redo();
    void toggleSolution();
    void newGame();
    void restartGame();
    void settings();
    void quit();
    void help();
    void about();
    void setClickSolid();
    void setClickEmpty();
    void setClickUndecided();

};
#endif // MAINWINDOW_H
