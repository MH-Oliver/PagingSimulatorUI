// configurationwindow.cpp
/**
 * @file configurationwindow.cpp
 * @brief Implementation of the main configuration window.
 */
#include "configurationwindow.h"
#include "ui_configurationwindow.h"
#include "simulationwindow.h"

#include <QRegularExpression>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

/**
 * @brief Utility: check if a string is composed only of digits.
 */
static inline bool isAllDigits(const QString& s) {
    static const QRegularExpression re(QStringLiteral("^\\d+$"));
    return re.match(s).hasMatch();
}

ConfigurationWindow::ConfigurationWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConfigurationWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_loadConfigs, &QPushButton::clicked,
            this, &ConfigurationWindow::loadConfigurations);

    connect(ui->lineEdit_physicalStorage, &QLineEdit::textEdited,
            this, &ConfigurationWindow::onStorageSizeChanges);
    connect(ui->lineEdit_virtualStorage,  &QLineEdit::textEdited,
            this, &ConfigurationWindow::onStorageSizeChanges);
    connect(ui->lineEdit_page,            &QLineEdit::textEdited,
            this, &ConfigurationWindow::onStorageSizeChanges);
    connect(ui->lineEdit_tlb,             &QLineEdit::textEdited,
            this, &ConfigurationWindow::onStorageSizeChanges);

    ui->pushButton_loadConfigs->setEnabled(false);
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::loadConfigurations()
{
    SimulationWindow simulationWindow(this);
    simulationWindow.setSimulationParams(
        countFrames,
        countPages,
        tlbSize,
        ui->comboBox_pagingAlgorithmus->currentText()
        );
    simulationWindow.exec();
}

void ConfigurationWindow::onStorageSizeChanges()
{
    const QString pageLbl  = tr("Anzahl virtuelle Seiten: ");
    const QString frameLbl = tr("Anzahl Seitenrahmen: ");

    const bool okPhys = isAllDigits(ui->lineEdit_physicalStorage->text());
    const bool okVirt = isAllDigits(ui->lineEdit_virtualStorage->text());
    const bool okPage = isAllDigits(ui->lineEdit_page->text());
    const bool okTlb  = isAllDigits(ui->lineEdit_tlb->text());

    if (okPhys && okVirt && okPage && okTlb) {
        const int physicalStorage = ui->lineEdit_physicalStorage->text().toInt();
        const int virtualStorage  = ui->lineEdit_virtualStorage->text().toInt();
        const int pageSize        = ui->lineEdit_page->text().toInt();
        tlbSize                   = ui->lineEdit_tlb->text().toInt();

        countPages  = pageSize ? (virtualStorage  / pageSize) : 0;
        countFrames = pageSize ? (physicalStorage / pageSize) : 0;

        ui->label_countPages->setText(pageLbl  + QString::number(countPages));
        ui->label_countFrames->setText(frameLbl + QString::number(countFrames));
        ui->pushButton_loadConfigs->setEnabled(countPages > 0 && countFrames > 0);
    } else {
        ui->label_countPages->setText(pageLbl);
        ui->label_countFrames->setText(frameLbl);
        ui->pushButton_loadConfigs->setEnabled(false);
    }
}
