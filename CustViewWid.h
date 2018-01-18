#ifndef CUSTVIEWWID_H
#define CUSTVIEWWID_H

#include "ui_CustViewWid.h"
#include <cvinetv.h>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QHash>
#include <QStandardItem>
class QStandardItemModel;
class QStandardItem;
class CustFilterProxyModel;
class CustCNV;

class CustViewWid : public QWidget
{
    Q_OBJECT

public:
    explicit CustViewWid(QWidget *parent = 0);
    void searchRecursivly(CNVBrowser cnvbrowser, const char *strPath, QStandardItem *pParentItem);
    const CustCNV* getCustCNV(QStandardItem* pItem);
Q_SIGNALS:
    void emitCNVPath(QString strPath);
private:
    Ui::CustViewWid ui;
    QStandardItemModel *m_pModel;
    void getErrorDescripter(int status);

    QHash<QStandardItem*, CustCNV*> m_hash;
};

class CustFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit CustFilterProxyModel(QObject *parent = Q_NULLPTR);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

};

class CustStyleItemDelegate :public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustStyleItemDelegate(QObject *parent = Q_NULLPTR);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void  setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

class BridgeCnvItem : public QObject
{
    Q_OBJECT
public:
    explicit BridgeCnvItem(CustCNV* pCNV,
                           QStandardItem* pItem,
                           QObject* parent = Q_NULLPTR);
    ~BridgeCnvItem() {}
private:
    CustCNV* m_pCnv;
    QStandardItem* m_pItem;
};
#endif // CUSTVIEWWID_H
