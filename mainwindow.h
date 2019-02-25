#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHeaderView>
#include <QTableView>
#include <QMainWindow>
#include <QAction>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "area.h"
#include <unistd.h>
#include <QTimer>
#include <QTime>
#include <QComboBox>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <string>
#include <sstream>
#define rowheigth 10
#define colwidth 10
#define ROWS 47
#define COLS 55

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QLabel *time;
    QTimer *timer;
    QLabel *lifeString;
    QLabel *aliveString;
    int AliveCells;
    void countALiveCells();
    int CurrentGeneration;
    QTableWidget *LifeField;
    area areastate;
    QGroupBox *Options;
    QPushButton *Starter;
    QPushButton *Iterator;
    QPushButton *Stop;
    QPushButton *Exit;
    QPushButton *Cleaner;
    QPushButton *Random;
    QComboBox *intervaledit;
    QAction *saveAction;
    QAction *loadAction;
    void clearHazard();
    void refreshLifeField();
    void InitGroupBox();
    void InitLifeField();
    void InitMenu();
    void run();
    void updateStrings();
    bool GenerateState();

    int interval;
    bool IsAnybodyThere;
public slots:
    void changestatus(int,int);
    void activatecell(int, int);
    void deactivatecell(int, int);
    void clear();
    void next_generation();
    void changeinterval(int);
    void StartButtonActions();
    void StopButtonActions();
    void GenerateRandomField();

    void load();
    void save();
};

#endif // MAINWINDOW_H
