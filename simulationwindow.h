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
 * @brief Qt dialog to run the paging simulation step-by-step and visualize state.
 *
 * This window renders three tables (physical memory, page table, TLB) as snapshots
 * of the current state, and maintains a textual log panel with time-stamped lines.
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

    /**
     * @brief Destructor.
     */
    ~SimulationWindow() override;

    /**
     * @brief Provide initial parameters and create the simulation.
     * @param numFrames    Number of physical frames.
     * @param numPages     Number of virtual pages for the process.
     * @param tlbCapacity  TLB capacity.
     * @param algorithmName Replacement algorithm name as chosen in the combo box.
     */
    void setSimulationParams(int numFrames, int numPages, int tlbCapacity, QString algorithmName);

private:
    Ui::SimulationWindow *ui{nullptr};

    // Core simulation state
    std::unique_ptr<Simulation> simulation;  ///< Simulation engine.
    std::unique_ptr<Process>    process;     ///< Current process with page table.
    EventQueue                  eventQueue;  ///< Queue of scheduled memory accesses.

    // Trace state
    QString tracePath;   ///< Path to loaded trace file.
    bool    traceLoaded{false};

    // Remember chosen algorithm (for logging header line).
    QString algorithmName_;

    // --- UI helpers ---

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
     * @brief Append the final statistics summary to the log.
     */
    void logStatsToHistory();
    /**
     * @brief Load the text file and show it in the trace preview box.
    * @param path Filesystem path to the trace file.
    */
    void updateTracePreview(const QString& path);
private slots:
    /**
     * @brief Choose and load a trace file, prepare the event queue.
     */
    void runSimulation();

    /**
     * @brief Execute a single event (one page access).
     */
    void onRunNextAccess();
};

#endif // SIMULATIONWINDOW_H
