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
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::loadConfigurations() {
    SimulationWindow simulationWindow;
    simulationWindow.exec();
}

void ConfigurationWindow::onStorageSizeChanges() {
    static QRegularExpression numberCheck("^\\d+$"); // Pruefen, ob nur Zahlen enthalten sind
    QString defaultPageText = "Anzahl virtuelle Seiten: ";
    QString defaultFrameText = "Anzahl Seitenrahmen: ";


    if (numberCheck.match(ui->lineEdit_physicalStorage->text()).hasMatch() &&
        numberCheck.match(ui->lineEdit_virtualStorage->text()).hasMatch() &&
        numberCheck.match(ui->lineEdit_page->text()).hasMatch()) {

        int physicalStorage = ui->lineEdit_physicalStorage->text().toInt();
        int virtualStorage = ui->lineEdit_virtualStorage->text().toInt();
        int pageSize = ui->lineEdit_page->text().toInt();


        double countPages = (double)virtualStorage/pageSize;
        double countFrames = (double)physicalStorage/pageSize;

        ui->label_countPages->setText(defaultPageText +
                                      QString::number(countPages));
        ui->label_countFrames->setText(defaultFrameText +
                                      QString::number(countFrames));
    } else {
        ui->label_countPages->setText(defaultPageText);
        ui->label_countFrames->setText(defaultFrameText);
    }
}
