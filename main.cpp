// main.cpp
/**
 * @file main.cpp
 * @brief Application entry point.
 */
#include "configurationwindow.h"
#include <QApplication>

/**
 * @brief Create and show the ConfigurationWindow.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigurationWindow w;
    w.show();
    return a.exec();
}
