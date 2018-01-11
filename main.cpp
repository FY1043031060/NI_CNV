#include <QApplication>
#include <CustCNVEditor.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustCNVEditor w;
    w.show();
    return a.exec();
}
