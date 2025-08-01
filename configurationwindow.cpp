#include "configurationwindow.h"
#include "ui_configurationwindow.h"
#include "simulationwindow.h"

ConfigurationWindow::ConfigurationWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConfigurationWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_loadConfigs, &QPushButton::clicked, this, &ConfigurationWindow::loadConfigurations);

    connect(ui->lineEdit_physicalStorage, &QLineEdit::textEdited, this, &ConfigurationWindow::onStorageSizeChanges);
    connect(ui->lineEdit_virtualStorage, &QLineEdit::textEdited, this, &ConfigurationWindow::onStorageSizeChanges);
    connect(ui->lineEdit_page, &QLineEdit::textEdited, this, &ConfigurationWindow::onStorageSizeChanges);
    connect(ui->lineEdit_tlb, &QLineEdit::textEdited, this, &ConfigurationWindow::onStorageSizeChanges);
    ui->pushButton_loadConfigs->setEnabled(false);
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::loadConfigurations() {
    SimulationWindow simulationWindow;
    simulationWindow.setSimulationParams(
        countFrames,
        countPages,
        tlbSize,
        ui->comboBox_pagingAlgorithmus->currentText());
    simulationWindow.exec();
}

void ConfigurationWindow::onStorageSizeChanges() {
    static QRegularExpression numberCheck("^\\d+$"); // Pruefen, ob nur Zahlen enthalten sind
    QString defaultPageText = "Anzahl virtuelle Seiten: ";
    QString defaultFrameText = "Anzahl Seitenrahmen: ";


    if (numberCheck.match(ui->lineEdit_physicalStorage->text()).hasMatch() &&
        numberCheck.match(ui->lineEdit_virtualStorage->text()).hasMatch() &&
        numberCheck.match(ui->lineEdit_page->text()).hasMatch() &&
        numberCheck.match(ui->lineEdit_tlb->text()).hasMatch()) {

        int physicalStorage = ui->lineEdit_physicalStorage->text().toInt();
        int virtualStorage = ui->lineEdit_virtualStorage->text().toInt();
        int pageSize = ui->lineEdit_page->text().toInt();
        this->tlbSize = ui->lineEdit_tlb->text().toInt();

        this->countPages = virtualStorage/pageSize;
        this->countFrames = physicalStorage/pageSize;

        ui->label_countPages->setText(defaultPageText +
                                      QString::number(countPages));
        ui->label_countFrames->setText(defaultFrameText +
                                      QString::number(countFrames));
        ui->pushButton_loadConfigs->setEnabled(true);
    } else {
        ui->label_countPages->setText(defaultPageText);
        ui->label_countFrames->setText(defaultFrameText);
        ui->pushButton_loadConfigs->setEnabled(false);
    }
}
