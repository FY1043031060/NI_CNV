#include "CnvWid.h"
#include <QDebug>
#include <QTreeWidgetItem>
#include <QHash>
void CnvWid::getErrorDescripter(int status)
{
    if(status < 0)
         qDebug() << __FUNCTION__<< CNVGetErrorDescription(status);
}

void CnvWid::searchRecursivly(CNVBrowser cnvbrowser, const char* strPath, QTreeWidgetItem* pParentItem)
{
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
    QHash<QString, QTreeWidgetItem*> hashPath;
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
                QTreeWidgetItem* pItem = new QTreeWidgetItem(pParentItem);
                pItem->setData(0, Qt::UserRole, QVariant::fromValue(str));
                qDebug() << __FUNCTION__ << pItem->data(0, Qt::UserRole);
                pItem->setText(0, strNameTmp);
                pItem->setText(1, QString("%1").arg(eBroweType));
                pItem->setText(2, QString("%1").arg(type));
                if(pParentItem == Q_NULLPTR)
                    ui.treeWidget->addTopLevelItem(pItem);
                else
                    pParentItem->addChild(pItem);
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

CnvWid::CnvWid(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    int status = 0;
    CNVBrowser cnvbrowser;
    status = CNVCreateBrowser(&cnvbrowser);
    getErrorDescripter(status);

    searchRecursivly(cnvbrowser, NULL, NULL);
    status = CNVDisposeBrowser(cnvbrowser);
    getErrorDescripter(status);
    connect(ui.treeWidget, &QTreeWidget::itemActivated,[&](QTreeWidgetItem *item, int column){
        ui.label->setText(item->data(column,  Qt::UserRole).toString());
    });
    connect(ui.pushButton, &QPushButton::clicked, [&](){
        bool ok;
        int iType = ui.treeWidget->currentItem()->text(2).toInt(&ok);
        if(!ok)
            return;
        if(iType <CNVEmpty || iType > CNVStruct)
            return;
        Q_EMIT emitCNVPath(ui.treeWidget->currentItem()->data(0, Qt::UserRole).toString());
    });
}
