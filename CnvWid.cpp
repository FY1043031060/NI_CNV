#include "CnvWid.h"
#include <QDebug>
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
//    status = CNVBrowse(cnvbrowser, strPath);
    getErrorDescripter(status);
    QString strTemp;
    if(!strPath)
    {
        strTemp +=("\\\\");
        strTemp += strPath;
    }
    else
    {
        strTemp += strPath;
        strTemp +=("\\");
    }
    for(status =1;status == 1;)
    {
        status = CNVBrowseNextItem(cnvbrowser,&strName, &leaf, &eBroweType, &typeData);
        getErrorDescripter(status);
        qDebug() <<"name:"<< QString::fromLocal8Bit(strName)<<"type:" << eBroweType;
        if(status == 1 && (eBroweType == CNVBrowseTypeMachine || eBroweType == CNVBrowseTypeProcess))
        {
            strTemp += strName;
            qDebug() << "path:" << strTemp;
            searchRecursivly(cnvbrowser, strTemp.toLocal8Bit().data());
        }
        CNVFreeMemory(strName);
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
