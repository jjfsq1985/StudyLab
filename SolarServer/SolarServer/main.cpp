#include "stdafx.h"
#include "solarserver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SolarServer w;
    w.show();
    return a.exec();
}
