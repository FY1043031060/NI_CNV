#ifndef CUSTVIEWWID_H
#define CUSTVIEWWID_H

#include "ui_CustViewWid.h"
#include <cvinetv.h>
#include <QSortFilterProxyModel>
class QStandardItemModel;
class QStandardItem;
class CustFilterProxyModel;
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
//class CustFilterProxyModel: public QSortFilterProxyModel
//{
//    Q_OBJECT
//public:
//    explicit CustFilterProxyModel(QObject *parent = Q_NULLPTR);
//protected:
//    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

//}

#endif // CUSTVIEWWID_H
