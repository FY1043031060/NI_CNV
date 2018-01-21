#include <QApplication>
#include <CustViewWid.h>
#include <CCnvProxyView.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CCnvProxyView m;
    m.show();
    return a.exec();
}
