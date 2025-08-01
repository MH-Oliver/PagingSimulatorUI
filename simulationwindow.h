#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include <QDialog>
#include "PagingSimulator/src/Simulation.h"

namespace Ui {
class SimulationWindow;
}

class SimulationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationWindow(QWidget *parent = nullptr);
    ~SimulationWindow();
    void setSimulationParams();

private:
    Ui::SimulationWindow *ui;
    Simulation* simulation;
};

#endif // SIMULATIONWINDOW_H
