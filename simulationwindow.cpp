#include "simulationwindow.h"
#include "PagingSimulator/src/core/Algorithms/NRUAlgorithm.h"
#include "ui_simulationwindow.h"

#include "PagingSimulator/src/core/PagingAlgorithm.h"

SimulationWindow::SimulationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SimulationWindow)
{
    ui->setupUi(this);

    /*ui->tableWidget_physikalischerSpeicher->setRowCount(10);
    for (int i = 0; i < 10; i++) {
        ui->tableWidget_physikalischerSpeicher->setItem(i,0, new QTableWidgetItem(QString("Qt")));
        ui->tableWidget_physikalischerSpeicher->setItem(i,1, new QTableWidgetItem(QString("Qt1")));
        ui->tableWidget_physikalischerSpeicher->setItem(i,2, new QTableWidgetItem(QString("Qt2")));
    }*/
}

SimulationWindow::~SimulationWindow()
{
    delete ui;
}

void SimulationWindow::setSimulationParams(int numFrames, int numPages, int tlbCapacity, QString algorithmName) {
    PagingAlgorithm* pagingAlgorithm = new NRUAlgorithm();

    //simulation = new Simulation(numFrames, )
}
