#include "simulationwindow.h"
#include "PagingSimulator/src/core/Algorithms/NRUAlgorithm.h"
#include "core/Algorithms/FIFOAlgorithm.h"
#include "core/Algorithms/LRUAlgorithm.h"
#include "core/Algorithms/NFUAlgorithm.h"
#include "core/Algorithms/NFUNoAgingAlgorithm.h"
#include "core/Algorithms/SecondChanceAlgorithm.h"
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
    PagingAlgorithm* pagingAlgorithm;
    if (algorithmName == "First In First Out") pagingAlgorithm = new FIFOAlgorithm();
    else if (algorithmName == "Second Chance") pagingAlgorithm = new SecondChanceAlgorithm();
    else if (algorithmName == "Least Recently Used") pagingAlgorithm = new LRUAlgorithm();
    else if (algorithmName == "Not Frequently Used (mit Aging)") pagingAlgorithm = new NFUAlgorithm();
    else if (algorithmName == "Not Frequently Used (ohne Aging)") pagingAlgorithm = new NFUNoAgingAlgorithm();
    else pagingAlgorithm = new NRUAlgorithm();


    simulation = new Simulation(numFrames, pagingAlgorithm, tlbCapacity);
    Process process(1, numPages);
    simulation->setCurrentProcess(&process);

    ui->tableWidget_virtuellerSpeicher->setRowCount(numPages);
    ui->tableWidget_physikalischerSpeicher->setRowCount(numFrames);
    ui->tableWidget_tlb->setRowCount(tlbCapacity);
}
