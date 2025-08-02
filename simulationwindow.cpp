#include "simulationwindow.h"
#include "PagingSimulator/src/core/Algorithms/NRUAlgorithm.h"
#include "core/Algorithms/FIFOAlgorithm.h"
#include "core/Algorithms/LRUAlgorithm.h"
#include "core/Algorithms/NFUAlgorithm.h"
#include "core/Algorithms/NFUNoAgingAlgorithm.h"
#include "core/Algorithms/SecondChanceAlgorithm.h"
#include "ui_simulationwindow.h"

#include "PagingSimulator/src/core/PagingAlgorithm.h"
#include "TraceLoader.h"

SimulationWindow::SimulationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SimulationWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_runSimulation, &QPushButton::clicked, this, &SimulationWindow::runSimulation);
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

    updateTables();
}

void SimulationWindow::runSimulation() {
    //loadTrace("../../resources/trace.txt", eventQueue, simulation, 1.0, 1.0);

    //eventQueue.run();
    //simulation->printStatistics();
    //updateTables();
}

void SimulationWindow::updateTables() {
    auto mainMemory = simulation->getMain_memory();
    for (int i = 0; i < mainMemory.size(); i++) {
        ui->tableWidget_physikalischerSpeicher->setItem(i,0, new QTableWidgetItem(QString::number(i)));
        ui->tableWidget_physikalischerSpeicher->setItem(i,1, new QTableWidgetItem(QString::number(mainMemory[i].pageId)));
        ui->tableWidget_physikalischerSpeicher->setItem(i,2, new QTableWidgetItem(QVariant(mainMemory[i].referencedBit).toString()));
    }

    auto virtualStorage = simulation->getMMU().currentProcess->page_table.entries;
    for (int i = 0; i < virtualStorage.size(); i++) {
        ui->tableWidget_virtuellerSpeicher->setItem(i,0, new QTableWidgetItem(QString::number(i)));
        ui->tableWidget_virtuellerSpeicher->setItem(i,1, new QTableWidgetItem(QString::number(virtualStorage[i].frameIndex)));
        ui->tableWidget_virtuellerSpeicher->setItem(i,2, new QTableWidgetItem(QVariant(virtualStorage[i].isPresent).toString()));
    }

    auto tlbEntries = simulation->getMMU().tlb.entries;
    for (int i = 0; i < tlbEntries.size(); i++) {
        ui->tableWidget_tlb->setItem(i,1, new QTableWidgetItem(QString::number(tlbEntries[i].page_frame_index)));
        ui->tableWidget_tlb->setItem(i,2, new QTableWidgetItem(QVariant(tlbEntries[i].page_index).toString()));
    }
}
