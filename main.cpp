#include <QApplication>
#include <CustCNVEditor.h>
#include <CustViewWid.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustCNVEditor w;
    w.show();
    CustViewWid m;
    m.show();
    return a.exec();
}
