#include "calcwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // a.setWindowIcon(QIcon(":/images/icon.png"));
    CalcWindow w;
    w.setWindowIcon(QIcon(":/images/icon.png"));
   //  QGuiApplication::setWindowIcon(QIcon(":/images/icon.png"));
//Buffer::instance().data()=0.0;
    w.show();
    return a.exec();
}
