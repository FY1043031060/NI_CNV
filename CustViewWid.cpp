#include "CustViewWid.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QHash>
void CustViewWid::getErrorDescripter(int status)
{
    if(status < 0)
         qDebug() << __FUNCTION__<< CNVGetErrorDescription(status);
}

void CustViewWid::searchRecursivly(CNVBrowser cnvbrowser,
                                   const char* strPath,
                                   QStandardItem* pParentItem)
{
//    if(pParentItem != Q_NULLPTR)
//        qDebug() << __FUNCTION__<<pParentItem->text();
    int status = 0;
    int leaf = 0;
    char* strName = 0;
    CNVBrowseType eBroweType;
    CNVData typeData = 0;
    getErrorDescripter(status);
    status = CNVBrowse(cnvbrowser, strPath);
    getErrorDescripter(status);
    QString strTemp;
    if(!strPath)
    {
        strTemp +=("\\\\");
        strTemp += QString::fromLocal8Bit(strPath);
    }
    else
    {
        strTemp += QString::fromLocal8Bit(strPath);
        strTemp +=("\\");
    }
    QList<QString> listPath;
    QHash<QString, QStandardItem*> hashPath;
    for(status =1; status == 1;)
    {
        ////	int CVIFUNC CNVBrowseNextItem(
        /// 		CNVBrowser browser,
        /// 		char ** item,
        /// 		int * leaf, 1 ? leaf : not
        /// 		CNVBrowseType * browseType,
        /// 		CNVData * typeData);
        ///
        status = CNVBrowseNextItem(cnvbrowser,&strName, &leaf, &eBroweType, &typeData);
        getErrorDescripter(status);

        if(status == 1 && (eBroweType != CNVBrowseTypeUndefined))
        {
#if 1
            unsigned long	nDims;
            CNVDataType		type;
            CNVGetDataType(typeData, &type, &nDims);
            if(nDims == 0)
            {

            }
#endif //// leaf type
            QString strNameTmp = QString::fromLocal8Bit(strName);
            QString str = strTemp + strNameTmp;
            qDebug() <<"name:"<< strNameTmp
                    <<"type:" << eBroweType
                   <<"path:" << str;
            listPath.append(str);
            {
                QStandardItem* pItem = new QStandardItem;
                pItem->setData(QVariant::fromValue(str),Qt::UserRole);
                qDebug() << __FUNCTION__ << pItem->data(Qt::UserRole);
                pItem->setText(strNameTmp);
                QStandardItem* pItem1 = new QStandardItem;
                pItem1->setText(QString("%1").arg(eBroweType));
                QStandardItem* pItem2 = new QStandardItem;
                pItem2->setText(QString("%1").arg(type));

                if(pParentItem == Q_NULLPTR)
                {
                    m_pModel->appendRow(pItem);
                    m_pModel->setItem(pItem->index().row(),1, pItem1);
                    m_pModel->setItem(pItem->index().row(),2, pItem2);
                }else
                {
                    pParentItem->appendRow(pItem);
                    pParentItem->setChild(pItem->index().row(),1, pItem1);
                    pParentItem->setChild(pItem->index().row(),2,pItem2);
                }
                qDebug() << pItem->text();
                hashPath.insert(str, pItem);
            }
        }
        CNVFreeMemory(strName);
    }
    for(int index = 0; index < hashPath.size(); index++)
    {
        searchRecursivly(cnvbrowser, listPath.value(index).toLocal8Bit().data(), hashPath.value(listPath.value(index)));
    }
}

CustViewWid::CustViewWid(QWidget *parent) :
    QWidget(parent), m_pModel(new QStandardItemModel(this))
{
    ui.setupUi(this);
    ui.treeView->setModel(m_pModel);
    m_pModel->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("名称")<<QStringLiteral("节点类型") <<QStringLiteral("数值类型"));
    int status = 0;
    CNVBrowser cnvbrowser;
    status = CNVCreateBrowser(&cnvbrowser);
    getErrorDescripter(status);

    searchRecursivly(cnvbrowser, NULL, NULL);
    status = CNVDisposeBrowser(cnvbrowser);
    getErrorDescripter(status);
//    connect(ui.treeWidget, &QTreeWidget::itemActivated,[&](QTreeWidgetItem *item, int column){
//        ui.label->setText(item->data(column,  Qt::UserRole).toString());
//    });
//    connect(ui.pushButton, &QPushButton::clicked, [&](){
//        bool ok;
//        int iType = ui.treeWidget->currentItem()->text(2).toInt(&ok);
//        if(!ok)
//            return;
//        if(iType <CNVEmpty || iType > CNVStruct)
//            return;
//        Q_EMIT emitCNVPath(ui.treeWidget->currentItem()->data(0, Qt::UserRole).toString());
//    });
    connect(m_pModel, &QStandardItemModel::itemChanged,[&](QStandardItem *item){
//        ui.label->setText(item->data(column,  Qt::UserRole).toString());
    });
}
