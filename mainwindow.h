#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QMessageBox>
#include <QCommonStyle>
#include <QSignalMapper>
#include <QString>
#include <vector>
#include "pushbutton.h"
#include "nonogram.h"

#define MIN_PUZZLE_SIZE 3
#define MAX_PUZZLE_SIZE 30
#define DEFAULT_PUZZLE_SIZE 20
#define STATUS_UNDECIDED 2
#define STATUS_BLANK 1
#define STATUS_SOLID 0
#define STATUS_HINT_BLANK 11
#define STATUS_HINT_SOLID 10

class MainWindow : public QMainWindow {
	Q_OBJECT

 private:
	QWidget *window;
	QMenuBar *menu;
	QMenu *fileMenu;
	QMenu *helpMenu;
	QAction *exitAction;
	QAction *aboutAction;
	QAction *helpAction;
	QSpinBox *widthBox;
	QSpinBox *heightBox;
	QVBoxLayout *layout;
	QHBoxLayout *top;
	QGridLayout *grid;
	QCommonStyle *style;
	QLabel *widthLabel;
	QLabel *heightLabel;
    QPushButton *startstopButton;
    QPushButton *toggleSolutionButton;
	QSignalMapper *mapperLeftButton;
	QSignalMapper *mapperRightButton;
    vector<PushButton*> puzzle;
	vector<QLabel*> xAxis, yAxis;
	vector<size_t> **xAxisClue;
	vector<size_t> **yAxisClue;
	vector<int> status;
    int width, height, mouseButton, newStatus;
    bool firstClick, solutionShown, gameRunning;
	Nonogram *ngram;
	void cleanUp();

 public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

 private slots:
    void startstop();
    void startPuzzle();
    void HideSolution();
    void ShowSolution();
    void toggleSolution();
    void paintPosition(int position, int status);
    void clicked(int position);
    void leftClicked(int position);
    void rightClicked(int position);
	void checkSolution();
	void quit();
	void help();
	void about();
};
#endif
