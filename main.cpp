#include "sniffout.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SniffOut w;
    w.show();

    return a.exec();
}
