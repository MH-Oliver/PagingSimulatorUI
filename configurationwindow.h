#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include "ui_simulationwindow.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConfigurationWindow;
}
QT_END_NAMESPACE

class ConfigurationWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigurationWindow(QWidget *parent = nullptr);
    ~ConfigurationWindow();

private:
    Ui::ConfigurationWindow *ui;
    //Ui::SimulationWindow *simuationWindow;

    void loadConfigurations();
    void onStorageSizeChanges();

};
#endif // CONFIGURATIONWINDOW_H
