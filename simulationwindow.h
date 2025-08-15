// simulationwindow.h
/**
 * @file simulationwindow.h
 * @brief Simulation dialog that runs the trace and visualizes state.
 */
#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include <QDialog>
#include <memory>
#include <QString>

#include "Simulation.h"
#include "des/EventQueue.h"

namespace Ui { class SimulationWindow; }

/**
 * @class SimulationWindow
 * @brief Dialog to run the simulation step-by-step and show current state.
 *
 * It renders three tables (physical memory, page table, TLB) as snapshots
 * of the current state, and keeps textual history in a log panel.
 */
class SimulationWindow : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct the dialog.
     * @param parent Parent widget.
     */
    explicit SimulationWindow(QWidget *parent = nullptr);
    ~SimulationWindow() override;

    /**
     * @brief Provide initial parameters and create the simulation.
     * @param numFrames Number of physical frames.
     * @param numPages  Number of virtual pages for the process.
     * @param tlbCapacity TLB capacity.
     * @param algorithmName Replacement algorithm name from combo box.
     */
    void setSimulationParams(int numFrames, int numPages, int tlbCapacity, QString algorithmName);

private:
    Ui::SimulationWindow *ui{nullptr};

    // Core state
    std::unique_ptr<Simulation> simulation;
    std::unique_ptr<Process>    process;
    EventQueue                  eventQueue;

    // Trace state
    QString tracePath;
    bool    traceLoaded{false};

    // UI helpers
    /**
     * @brief Refresh all three tables from the current simulation state.
     */
    void updateTables();

    /**
     * @brief Update the compact statistics label.
     */
    void updateStatsLabel();

    /**
     * @brief Append one line to the textual log with a timestamp prefix.
     * @param line Text to append (plain text; will be HTML-escaped).
     */
    void appendLog(const QString& line);

    /**
     * @brief Append the final summary (same content as the stats label) to the log.
     */
    void logStatsToHistory();

private slots:
    /**
     * @brief Load a trace file and prepare the event queue.
     */
    void runSimulation();

    /**
     * @brief Execute a single event (one page access).
     */
    void onRunNextAccess();
};

#endif // SIMULATIONWINDOW_H
