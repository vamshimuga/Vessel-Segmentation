#include <QApplication>
#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MiaSolution");
    QCoreApplication::setOrganizationDomain("mia-solution.com");
    QCoreApplication::setApplicationName("MiaLite");
    MainWindow w;
    w.show();

    return a.exec();
}
