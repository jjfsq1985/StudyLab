#include "stdafx.h"
#include "clienttest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientTest w;
    w.show();
    return a.exec();
}
