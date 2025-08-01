#include "configurationwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigurationWindow w;
    w.show();
    return a.exec();
}
