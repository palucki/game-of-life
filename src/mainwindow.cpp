

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));

    this->setFixedSize(820,540);
    LifeField = new QTableWidget(this);
    timer = new QTimer(this);
    timer->setInterval(100);

    QMenu *file = menuBar()->addMenu("File");
    loadAction = new QAction("Load the map", this);
    saveAction = new QAction("Save the map", this);
    file->addAction(loadAction);
    file->addAction(saveAction);

    InitGroupBox();
    InitLifeField();
    InitMenu();
}

MainWindow::~MainWindow()
{
}

void MainWindow::InitMenu()
{
    connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
}

void MainWindow::InitGroupBox()
{
    aliveString = new QLabel( this);
    aliveString->setFixedSize(200,20);
    aliveString->move(600, 390);
    lifeString = new QLabel( this);
    lifeString->setFixedSize(170,20);
    lifeString->move(600, 370);

    Iterator = new QPushButton("Next generation",this);
    Starter = new QPushButton("Start auto-play",this);
    Stop = new QPushButton("Pause the game",this);
    Cleaner = new QPushButton("Clear the map", this);
    Random = new QPushButton("Random map", this);
    Exit = new QPushButton("Quit",this);
    time = new QLabel("Generation time [ms]", this);
    time->move(600,40);
    time->setFixedSize(170,20);
    intervaledit = new QComboBox(this);
    intervaledit->move(600,70);
    for(int i=0; i<10; i++)
    {
        intervaledit->insertItem(i,QString::number((i+1)*100));
    }
    connect(intervaledit,SIGNAL(currentIndexChanged(int)), this, SLOT(changeinterval(int)));

    Iterator->setFixedSize(150,40);
    Starter->setFixedSize(150,40);
    Stop->setFixedSize(150,40);
    Exit->setFixedSize(150,40);
    Cleaner->setFixedSize(150,40);
    Random->setFixedSize(150,40);

    Starter->move(600,120);
    Stop->move(600,170);
    Iterator->move(600,220);
    Cleaner->move(600,320);
    Random->move(600, 270 );
    Exit->move(660,450);

    connect(Exit,SIGNAL(clicked()),this,SLOT(close()));
    connect(LifeField, SIGNAL(cellPressed(int,int)), this, SLOT(changestatus(int,int)));
    connect(Cleaner,SIGNAL(clicked()),this,SLOT(clear()));
    connect(Cleaner,SIGNAL(clicked()),timer,SLOT(stop()));
    connect(Iterator, SIGNAL(clicked()),this,SLOT(next_generation()));

    connect(Random,SIGNAL(clicked()), this, SLOT(GenerateRandomField()));

    connect(Starter,SIGNAL(clicked()), timer,SLOT(start()));
    connect(timer, SIGNAL(timeout()), this, SLOT(next_generation()));
    connect(Stop,SIGNAL(clicked()), timer, SLOT(stop()));

    connect(Starter, SIGNAL(clicked()), this, SLOT(StartButtonActions()));
    connect(Stop, SIGNAL(clicked()), this, SLOT(StopButtonActions()));
    Stop->setEnabled(false);
}

void MainWindow::InitLifeField()
{
    clearHazard();
    CurrentGeneration = 0;
    AliveCells = 0;
    IsAnybodyThere = false;
    updateStrings();

    LifeField->verticalHeader()->setVisible(false);
    LifeField->horizontalHeader()->setVisible(false);
    LifeField->move(20,50);
    LifeField->setSelectionMode(QAbstractItemView::SingleSelection);
    LifeField->setEditTriggers(QAbstractItemView::NoEditTriggers);
    LifeField->setFixedSize(552,472);
    LifeField->setRowCount(ROWS);
    LifeField->setColumnCount(COLS);

    LifeField->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    LifeField->verticalHeader()->setDefaultSectionSize(10);

    LifeField->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    LifeField->horizontalHeader()->setDefaultSectionSize(10);

    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            areastate[i][j] = false;
            LifeField->setItem(i,j,new QTableWidgetItem());
        }
    }

    LifeField->setSelectionMode(QAbstractItemView::NoSelection);
}

void MainWindow::changestatus(int x, int y)
{
    if(areastate[x][y])
        deactivatecell(x,y);
    else
        activatecell(x,y);
}

void MainWindow::activatecell(int x, int y)
{
    areastate[x][y]=true;
    LifeField->item(x,y)->setBackgroundColor(Qt::blue);
}

void MainWindow::deactivatecell(int x, int y)
{
    areastate[x][y]=false;
    LifeField->item(x,y)->setBackgroundColor(Qt::white);
}

void MainWindow::clearHazard()
{
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            areastate[i][j] = false;
        }
    }
}

void MainWindow::clear()
{
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            areastate[i][j]=false;
            LifeField->item(i,j)->setBackgroundColor(Qt::white);
        }
    }

    CurrentGeneration = 0;
    updateStrings();
    StopButtonActions();
}

void MainWindow::refreshLifeField()
{
    IsAnybodyThere = false;
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            if(areastate[i][j])
            {
                activatecell(i,j);
                IsAnybodyThere = true;
            }
            else  deactivatecell(i,j);
        }
    }

    if(!IsAnybodyThere)
    {
        StopButtonActions();
        timer->stop();
    }
}

void MainWindow::next_generation()
{
    areastate = areastate.next_generation();
    CurrentGeneration++;

    refreshLifeField();
    updateStrings();
}

void MainWindow::changeinterval(int changedinterval)
{
    interval = 100*(changedinterval+1);

    timer->setInterval(interval);
}

void MainWindow::StartButtonActions()
{
    timer->start();
    Starter->setEnabled(false);
    Iterator->setEnabled(false);
    Stop->setEnabled(true);
}

void MainWindow::StopButtonActions()
{
    timer->stop();
    Stop->setEnabled(false);
    Starter->setEnabled(true);
    Iterator->setEnabled(true);
}

void MainWindow::updateStrings()
{
    QString templife, tempalive;
    templife = "Generation no.:  " + QString::number(CurrentGeneration);
    lifeString->setText(templife);
    countALiveCells();
    tempalive = "Alive cells:  " + QString::number(AliveCells);
    aliveString->setText(tempalive);
}

void MainWindow::countALiveCells()
{
    int tempAliveCells=0;
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            if(areastate[i][j])
            {
                tempAliveCells++;
            }
        }
    }

    AliveCells = tempAliveCells;
}

bool MainWindow::GenerateState()
{
    if (qrand() % ((100 + 1) - 1) + 1 > 49 )
        return true;
    else
        return false;
}

void MainWindow::GenerateRandomField()
{
    clear();
    for(int i=0; i<ROWS; i++)
    {
        for (int j=0; j<COLS; j++)
        {
            areastate[i][j] = GenerateState();
        }
    }
    refreshLifeField();
    updateStrings();
}

void MainWindow::load()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
    tr("GameofLife Files (*.gl)"));

    if (fileName != "")
    {
        clear();
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"),
                tr("Could not open file"));
                return;
        }
        QTextStream stream(&file);
        QString line ;

        while (!stream.atEnd())
        {
            int xpos;
            int ypos;
            line = file.readLine().constData();
            string tekst = line.toStdString();
            stringstream ss(tekst);
            while(ss >> xpos)
            {
                ss >> ypos;
                if(xpos >=0 && xpos <=47 && ypos <=55 && ypos >= 0)
                    areastate[xpos][ypos]=true;
            }
        }

        file.close();
        refreshLifeField();
        updateStrings();
    }

}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
               tr("GameOfLife Files (*.gl)"));

    if (fileName != "")
    {
       QFile file;
       if(fileName.endsWith(".gl"))
       {
            file.setFileName(fileName);
       }
       else
       {
           file.setFileName(fileName.append(".gl"));
       }

       if (!file.open(QIODevice::WriteOnly))
       {
           // error message
       }
       else
       {
            QTextStream stream(&file);
            QString tmpx;
            QString tmpy;

            for(int i=0; i<ROWS; i++)
            {
                for(int j=0; j<COLS; j++)
                {
                    if(areastate[i][j]==true)
                    {
                       stream << i << " " << j << endl;
                    }
                }
            }

            file.close();
       }
    }
}
