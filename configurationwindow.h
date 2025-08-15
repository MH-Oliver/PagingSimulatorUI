// configurationwindow.h
/**
 * @file configurationwindow.h
 * @brief Main configuration window for the paging simulator.
 *
 * This window collects numeric parameters, validates them, and launches
 * the simulation dialog with a computed number of frames/pages and TLB size.
 */
#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ConfigurationWindow; }
QT_END_NAMESPACE

/**
 * @class ConfigurationWindow
 * @brief Collects simulation parameters and launches the simulation dialog.
 *
 * Derived values:
 * - frames = physicalMemory / pageSize
 * - pages  = virtualMemory  / pageSize
 */
class ConfigurationWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Construct the window.
     * @param parent Parent widget.
     */
    explicit ConfigurationWindow(QWidget *parent = nullptr);
    ~ConfigurationWindow() override;

private slots:
    /**
     * @brief Open SimulationWindow with current parameters.
     */
    void loadConfigurations();

    /**
     * @brief Validate inputs and recompute page/frame counts.
     */
    void onStorageSizeChanges();

private:
    Ui::ConfigurationWindow *ui{nullptr};
    int countPages{0};
    int countFrames{0};
    int tlbSize{0};
};

#endif // CONFIGURATIONWINDOW_H
