#include "CCnvProxyView.h"
#include <CustViewWid.h>
#include <QDebug>
CCnvProxyView::CCnvProxyView(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    CustFilterProxyModel* m_pFilterModel = new CustFilterProxyModel(this);
    m_pFilterModel->setSourceModel(CustViewWid::getInstance().getSourceModel());
    ui.treeView->setModel(m_pFilterModel);

    m_pFilterModel->setFilterRole(Qt::DisplayRole);
//    m_pFilterModel->setFilterRegExp(QStringLiteral("DESKTOP-E6DSLSR"));
    m_pFilterModel->setFilterRegExp(QStringLiteral("System"));
    CustStyleItemDelegate* pDelegate = new CustStyleItemDelegate(this);
    ui.treeView->setItemDelegate(pDelegate);
    connect(ui.treeView,&QAbstractItemView::clicked, [&](const QModelIndex &index){
        qDebug() << index.data();
        ui.label->setText(
                    index.sibling(index.row(),0).data(Qt::UserRole).toString());
    });
    connect(ui.pushButton, &QPushButton::clicked, [&](){
        bool ok;
        int iType = ui.treeView->currentIndex().sibling(0, 2).data().toInt(&ok);
        if(!ok)
            return;
        if(iType <CNVEmpty || iType > CNVStruct)
            return;
        Q_EMIT emitCNVPath(ui.treeView->currentIndex().sibling(ui.treeView->currentIndex().row(), 0).data(Qt::UserRole).toString());
    });
}
