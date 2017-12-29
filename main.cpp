#include "CnvWid.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CnvWid w;
    w.show();

    return a.exec();
}
