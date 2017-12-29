#include "CnvWid.h"
#include <QDebug>
#include <QTreeWidgetItem>
void CnvWid::getErrorDescripter(int status)
{
    if(status < 0)
         qDebug() << __FUNCTION__<< CNVGetErrorDescription(status);
}

void CnvWid::searchRecursivly(CNVBrowser cnvbrowser, const char* strPath)
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
#if 0
            unsigned int	nDims;
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
//                QTreeWidgetItem* pItem = new QTreeWidgetItem(pParentItem);
//                pItem->setData(0, Qt::DisplayRole, QVariant::fromValue(str));
//                pItem->setText(0, strNameTmp);
//                pItem->setText(1, QString("%1").arg(eBroweType));
            }
        }
        CNVFreeMemory(strName);
    }
    for(int index = 0;index < listPath.size();index++)
    {
        searchRecursivly(cnvbrowser, listPath.value(index).toLocal8Bit().data());
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

    searchRecursivly(cnvbrowser, NULL);
    status = CNVDisposeBrowser(cnvbrowser);
    getErrorDescripter(status);
}
