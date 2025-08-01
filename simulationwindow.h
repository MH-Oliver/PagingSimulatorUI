#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include <QDialog>

namespace Ui {
class SimulationWindow;
}

class SimulationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationWindow(QWidget *parent = nullptr);
    ~SimulationWindow();

private:
    Ui::SimulationWindow *ui;
};

#endif // SIMULATIONWINDOW_H
