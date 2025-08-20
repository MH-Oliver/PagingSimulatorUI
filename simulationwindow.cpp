/**
 * @file simulationwindow.cpp
 * @brief Implementation of the simulation dialog.
 */

#include "simulationwindow.h"
#include "ui_simulationwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFontDatabase>
#include <QDateTime>
#include <QTextBrowser>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <QStringConverter>

#include "core/PagingAlgorithm.h"
#include "core/algorithms/FIFOAlgorithm.h"
#include "core/algorithms/SecondChanceAlgorithm.h"
#include "core/algorithms/LRUAlgorithm.h"
#include "core/algorithms/NRUAlgorithm.h"
#include "core/algorithms/NFUAlgorithm.h"
#include "core/algorithms/NFUNoAgingAlgorithm.h"
#include "TraceLoader.h"

SimulationWindow::SimulationWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SimulationWindow)
{
    ui->setupUi(this);

    // Connect buttons
    connect(ui->pushButton_runSimulation,     &QPushButton::clicked, this, &SimulationWindow::runSimulation);
    connect(ui->pushButton_runNextPageAccess, &QPushButton::clicked, this, &SimulationWindow::onRunNextAccess);

    // Setup monospace fonts for stats + log
    QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    if (ui->label_stats) {
        ui->label_stats->setFont(mono);
        ui->label_stats->clear();
    }
    if (ui->textBrowser_logAusgabe) {
        ui->textBrowser_logAusgabe->setFont(mono);
        ui->textBrowser_logAusgabe->setOpenExternalLinks(true);
        ui->textBrowser_logAusgabe->document()->setMaximumBlockCount(2000);
        ui->textBrowser_logAusgabe->clear();
    }

    // Trace preview under the TLB table (QPlainTextEdit added in .ui with objectName 'plainTextEdit_trace')
    if (ui->plainTextEdit_trace) {
        ui->plainTextEdit_trace->setReadOnly(true);
        ui->plainTextEdit_trace->setFont(mono);
        ui->plainTextEdit_trace->setPlainText(QString());
    }

    if (ui->pushButton_runNextPageAccess)
        ui->pushButton_runNextPageAccess->setEnabled(false);
}

SimulationWindow::~SimulationWindow()
{
    delete ui;
}

void SimulationWindow::setSimulationParams(int numFrames, int numPages, int tlbCapacity, QString algorithmName)
{
    // Remember algorithm name for initial log
    algorithmName_ = algorithmName;

    // Instantiate chosen replacement algorithm
    std::unique_ptr<PagingAlgorithm> alg;
    if      (algorithmName == "First In First Out")               alg = std::make_unique<FIFOAlgorithm>();
    else if (algorithmName == "Second Chance")                    alg = std::make_unique<SecondChanceAlgorithm>();
    else if (algorithmName == "Least Recently Used")              alg = std::make_unique<LRUAlgorithm>();
    else if (algorithmName == "Not Frequently Used (mit Aging)")  alg = std::make_unique<NFUAlgorithm>();
    else if (algorithmName == "Not Frequently Used (ohne Aging)") alg = std::make_unique<NFUNoAgingAlgorithm>();
    else                                                          alg = std::make_unique<NRUAlgorithm>();

    // Create simulation and process
    simulation = std::make_unique<Simulation>(numFrames, std::move(alg), tlbCapacity);
    process    = std::make_unique<Process>(1, static_cast<unsigned>(numPages));
    simulation->setCurrentProcess(process.get());

    // Connect simulation logger to UI log
    simulation->setLogger([this](const std::string& s){
        this->appendLog(QString::fromStdString(s));
    });

    // Pre-size tables
    if (ui->tableWidget_virtuellerSpeicher)      ui->tableWidget_virtuellerSpeicher->setRowCount(numPages);
    if (ui->tableWidget_physikalischerSpeicher)  ui->tableWidget_physikalischerSpeicher->setRowCount(numFrames);
    if (ui->tableWidget_tlb)                     ui->tableWidget_tlb->setRowCount(tlbCapacity);

    // Ensure correct column counts (match your .ui design)
    if (ui->tableWidget_physikalischerSpeicher)  ui->tableWidget_physikalischerSpeicher->setColumnCount(3); // Frame, Page, Referenced
    if (ui->tableWidget_virtuellerSpeicher)      ui->tableWidget_virtuellerSpeicher->setColumnCount(3);     // Page, Frame, Present
    if (ui->tableWidget_tlb)                     ui->tableWidget_tlb->setColumnCount(2);                    // Page, Frame

    updateTables();
    updateStatsLabel();

    // Clear trace preview until the user picks a file
    if (ui->plainTextEdit_trace) ui->plainTextEdit_trace->clear();
}

void SimulationWindow::runSimulation()
{
    if (!simulation) return;

    // Default directory: resources next to executable
    const QString defaultDir = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/resources");

    tracePath = QFileDialog::getOpenFileName(
        this, tr("Choose Trace File"),
        defaultDir,
        tr("Trace (*.txt);;All Files (*.*)")
        );
    if (tracePath.isEmpty()) return;

    // First header log
    appendLog(QStringLiteral("Info: Simulation gestartet. Algorithmus: %1.").arg(algorithmName_));

    // Reset queue and load trace
    eventQueue = EventQueue{};
    loadTrace(tracePath.toStdString(), eventQueue, simulation.get(), 1.0, 1.0);
    traceLoaded = true;

    if (ui->pushButton_runNextPageAccess)
        ui->pushButton_runNextPageAccess->setEnabled(!eventQueue.empty());

    appendLog("Trace loaded: " + tracePath);

    // Update the preview box with the chosen file
    updateTracePreview(tracePath);

    updateTables();
    updateStatsLabel();
}

void SimulationWindow::onRunNextAccess()
{
    if (!simulation) return;

    if (!traceLoaded) {
        runSimulation();              // Lazy load
        if (!traceLoaded) return;
    }

    if (eventQueue.empty()) {
        appendLog("No more events. Simulation finished.");
        if (ui->pushButton_runNextPageAccess)
            ui->pushButton_runNextPageAccess->setEnabled(false);
        logStatsToHistory();
        return;
    }

    // Run one event
    eventQueue.step();

    // Update UI
    updateTables();
    updateStatsLabel();

    const auto s = simulation->stats();
    appendLog(QString("Step: accesses=%1, faults=%2, tlb(h/m)=%3/%4")
                  .arg(s.accesses).arg(s.pageFaults).arg(s.tlbHits).arg(s.tlbMisses));

    if (eventQueue.empty()) {
        if (ui->pushButton_runNextPageAccess)
            ui->pushButton_runNextPageAccess->setEnabled(false);
        appendLog("No more events. Simulation finished.");
        logStatsToHistory();
    }
}

void SimulationWindow::updateTables()
{
    if (!simulation || !process) return;

    // Physical memory table
    if (ui->tableWidget_physikalischerSpeicher) {
        const auto& mm = simulation->mainMemoryView();
        ui->tableWidget_physikalischerSpeicher->setRowCount(static_cast<int>(mm.size()));
        for (int i = 0; i < static_cast<int>(mm.size()); ++i) {
            ui->tableWidget_physikalischerSpeicher->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            ui->tableWidget_physikalischerSpeicher->setItem(i, 1, new QTableWidgetItem(QString::number(mm[i].pageId)));
            ui->tableWidget_physikalischerSpeicher->setItem(i, 2, new QTableWidgetItem(mm[i].referencedBit ? "true" : "false"));
        }
    }

    // Virtual page table
    if (ui->tableWidget_virtuellerSpeicher) {
        const auto& entries = process->page_table.entries;
        ui->tableWidget_virtuellerSpeicher->setRowCount(static_cast<int>(entries.size()));
        for (int i = 0; i < static_cast<int>(entries.size()); ++i) {
            ui->tableWidget_virtuellerSpeicher->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            ui->tableWidget_virtuellerSpeicher->setItem(i, 1, new QTableWidgetItem(QString::number(entries[i].frameIndex)));
            ui->tableWidget_virtuellerSpeicher->setItem(i, 2, new QTableWidgetItem(entries[i].isPresent ? "true" : "false"));
        }
    }

    // TLB table
    if (ui->tableWidget_tlb) {
        const auto& tlbEntries = simulation->mmuView().tlb.entries;
        ui->tableWidget_tlb->setRowCount(static_cast<int>(tlbEntries.size()));
        for (int i = 0; i < static_cast<int>(tlbEntries.size()); ++i) {
            ui->tableWidget_tlb->setItem(i, 0, new QTableWidgetItem(QString::number(tlbEntries[i].page_index)));
            ui->tableWidget_tlb->setItem(i, 1, new QTableWidgetItem(QString::number(tlbEntries[i].frame_index)));
        }
    }
}

void SimulationWindow::updateStatsLabel()
{
    if (!simulation || !ui->label_stats) return;

    const auto s = simulation->stats();
    const QString text =
        QString("Accesses: %1\n"
                "Page Faults: %2\n"
                "TLB Hits: %3 / Misses: %4\n"
                "Avg access time (us): %5\n"
                "TLB hit rate: %6\n"
                "Page fault rate: %7")
            .arg(s.accesses)
            .arg(s.pageFaults)
            .arg(s.tlbHits)
            .arg(s.tlbMisses)
            .arg(QString::number(s.avgAccessTimeUs, 'f', 2))
            .arg(QString::number(s.tlbHitRate,      'f', 2))
            .arg(QString::number(s.pageFaultRate,   'f', 2));

    ui->label_stats->setText(text);
}

void SimulationWindow::appendLog(const QString& line)
{
    if (!ui->textBrowser_logAusgabe) return;
    const QString ts  = QDateTime::currentDateTime().toString("HH:mm:ss.zzz ");
    const QString msg = (ts + line).toHtmlEscaped();
    ui->textBrowser_logAusgabe->append(msg);
}

void SimulationWindow::logStatsToHistory()
{
    if (!simulation || !ui->label_stats) return;
    appendLog("— Simulation Summary —");
    const auto lines = ui->label_stats->text().split('\n');
    for (const QString& ln : lines) appendLog(ln);
}

/**
 * @brief Load the text file and show it in the trace preview box.
 * @param path Filesystem path to the trace file.
 */
void SimulationWindow::updateTracePreview(const QString& path)
{
    if (!ui->plainTextEdit_trace) return;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("Failed to open trace for preview: " + path);
        ui->plainTextEdit_trace->clear();
        return;
    }

    QTextStream ts(&f);
    ts.setEncoding(QStringConverter::Utf8);
    const QString content = ts.readAll();
    ui->plainTextEdit_trace->setPlainText(content.trimmed());
}
