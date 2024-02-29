#include "calcwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CalcWindow w;

    w.setWindowIcon(QIcon(":/images/icon.png"));

    w.show();

    return a.exec();
}
