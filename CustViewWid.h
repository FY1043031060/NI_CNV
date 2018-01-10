#ifndef CUSTVIEWWID_H
#define CUSTVIEWWID_H

#include "ui_CustViewWid.h"
#include <cvinetv.h>
class QStandardItemModel;
class QStandardItem;
class CustViewWid : public QWidget
{
    Q_OBJECT

public:
    explicit CustViewWid(QWidget *parent = 0);
    void searchRecursivly(CNVBrowser cnvbrowser, const char *strPath, QStandardItem *pParentItem);
Q_SIGNALS:
    void emitCNVPath(QString strPath);
private:
    Ui::CustViewWid ui;
    QStandardItemModel *m_pModel;
    void getErrorDescripter(int status);
};

#endif // CUSTVIEWWID_H
