#include "CustViewWid.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QHash>
#include <CustCNV.h>
#include <QLineEdit>
void CustViewWid::getErrorDescripter(int status)
{
    if(status < 0)
         qDebug() << __FUNCTION__<< CNVGetErrorDescription(status);
}

void CustViewWid::searchRecursivly(CNVBrowser cnvbrowser,
                                   const char* strPath,
                                   QStandardItem* pParentItem)
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
                pItem->setData(QVariant::fromValue(str), Qt::UserRole);
                qDebug() << __FUNCTION__ << pItem->data(Qt::UserRole);
                pItem->setText(strNameTmp);
                QStandardItem* pItem1 = new QStandardItem;
                pItem1->setText(QString("%1").arg(eBroweType));
                QStandardItem* pItem2 = new QStandardItem;
                pItem2->setText(QString("%1").arg(type));
                QStandardItem* pItem3 = new QStandardItem;
                {
                    if(type <= CNVString && type >= CNVInt8)
                    {
                        auto pCNV = new CustCNV(str, this);
                        pItem3->setData(QVariant::fromValue(pCNV));
                        m_hash.insert(pItem3, pCNV);
                        BridgeCnvItem* pBridge = new BridgeCnvItem(pCNV, pItem3, this);
                    }
                }
                if(pParentItem == Q_NULLPTR)
                {
                    m_pModel->appendRow(pItem);
                    m_pModel->setItem(pItem->index().row(),1, pItem1);
                    m_pModel->setItem(pItem->index().row(),2, pItem2);
                    m_pModel->setItem(pItem->index().row(),3, pItem3);
                }else
                {
                    pParentItem->appendRow(pItem);
                    pParentItem->setChild(pItem->index().row(),1, pItem1);
                    pParentItem->setChild(pItem->index().row(),2, pItem2);
                    pParentItem->setChild(pItem->index().row(),3, pItem3);
                }
                qDebug() << pItem->text();
                hashPath.insert(str, pItem);
            }
        }
        CNVFreeMemory(strName);
    }
    ////NOTE::�ݹ����е�ǰ�ڵ�
    /// ����ݹ�
    for(int index = 0; index < hashPath.size(); index++)
    {
        searchRecursivly(cnvbrowser, listPath.value(index).toLocal8Bit().data(), hashPath.value(listPath.value(index)));
    }
}

const CustCNV *CustViewWid::getCustCNV(QStandardItem *pItem)
{
    return m_hash.value(pItem, Q_NULLPTR);
}

CustViewWid::CustViewWid(QWidget *parent) :
    QWidget(parent), m_pModel(new QStandardItemModel(this))
{
    ui.setupUi(this);
    CustFilterProxyModel* m_pFilterModel = new CustFilterProxyModel(this);
    m_pFilterModel->setSourceModel(m_pModel);
    ui.treeView->setModel(m_pFilterModel);

    m_pFilterModel->setFilterRole(Qt::DisplayRole);
    m_pFilterModel->setFilterRegExp(QStringLiteral("DESKTOP-E6DSLSR"));
    CustStyleItemDelegate* pDelegate = new CustStyleItemDelegate(this);
    ui.treeView->setItemDelegate(pDelegate);
    m_pModel->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("����")<<QStringLiteral("�ڵ�����") <<QStringLiteral("��ֵ����"));
    int status = 0;
    CNVBrowser cnvbrowser;
    status = CNVCreateBrowser(&cnvbrowser);
    getErrorDescripter(status);

    searchRecursivly(cnvbrowser, NULL, NULL);
    status = CNVDisposeBrowser(cnvbrowser);
    getErrorDescripter(status);
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

CustFilterProxyModel::CustFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent){}

bool CustFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    ////����������ˣ���ָ����Ӧ�Ľڵ�
    qDebug() <<__FUNCTION__<<  source_row << source_parent << source_parent.data();
    if(!source_parent.isValid())
    {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    else
        return true;
}

CustStyleItemDelegate::CustStyleItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}
#include <QPainter>
void CustStyleItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 2)
    {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        QString str;
        switch(index.data().toInt())
        {
        case CNVEmpty:
            str = "NULL";
            break;
        case CNVInt8:
            str = "INT8";
            break;
        case CNVInt16:
            str = "INT16";
            break;
        case CNVInt32:
            str = "INT32";
            break;
        case CNVInt64:
            str = "INT64";
            break;
        case CNVUInt8:
            str = "UINT8";
            break;
        case CNVUInt16:
            str = "UINT16";
            break;
        case CNVUInt32:
            str = "UINT32";
            break;
        case CNVUInt64:
            str = "UINT64";
            break;
        case CNVBool:
            str = "BOOL";
            break;
        case CNVSingle:
            str = "SINGLE";
            break;
        case CNVDouble:
            str = "DOUBLE";
            break;
        case CNVString:
            str = "STRING";
            break;
        case CNVStruct:
            str = "STRUCT";
            break;
        case CNVDataType4Bytes:
            str = "4BYTES";
            break;
        default:
            str = "NULL";
        }
        painter->drawText(option.rect, str);
    }
    else if(index.column() == 1)
    {
        QString str;
        switch(index.data().toInt())
        {

        case CNVBrowseTypeUndefined:
            str = QStringLiteral("Undefined");
            break;
        case CNVBrowseTypeMachine:
            str = QStringLiteral("Machine");
            break;
        case CNVBrowseTypeProcess:
            str = QStringLiteral("Process");
            break;
        case CNVBrowseTypeFolder:
            str = QStringLiteral("Folder");
            break;
        case CNVBrowseTypeItem:
            str = QStringLiteral("Item");
            break;
        case CNVBrowseTypeItemRange:
            str = QStringLiteral("ItemRange");
            break;
        case CNVBrowseTypeImplicitItem:
            str = QStringLiteral("ImplicitItem");
            break;
        case CNVBrowseType4Bytes:
            str = QStringLiteral("4Bytes");
            break;
        default:
            str = "NULL";
        }
        painter->drawText(option.rect, str);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void CustStyleItemDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QLineEdit* pWid = Q_NULLPTR;
    if(index.column() == 3)
    {
        CNVDataType eType = CNVEmpty;
        auto indexData = index.parent().sibling(index.row(), 2).data();
        qDebug() << index.parent() << index.parent().sibling(0,0).data() << index.parent().sibling(0,2).data(Qt::UserRole+1);

        eType = static_cast<CNVDataType>(index.sibling(index.row(), 2).data().toInt());
        pWid = qobject_cast<QLineEdit*>(editor);
        if(pWid == Q_NULLPTR)
            return;
        QVariant data;
        switch (eType) {
        case CNVDouble:
            data = pWid->text().trimmed().toDouble();
            break;
        default:
            break;
        }
        auto pCustCNV = index.data(Qt::UserRole+1).value<CustCNV*>();
        if(pCustCNV != Q_NULLPTR)
            pCustCNV->sendData(data);
    }
    else
        return QStyledItemDelegate::setModelData(editor, model, index);
}

QWidget *CustStyleItemDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        qDebug() << __FUNCTION__;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CustStyleItemDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        qDebug() << __FUNCTION__ << editor->metaObject()->className();
    }
    return QStyledItemDelegate::setEditorData(editor, index);
}

BridgeCnvItem::BridgeCnvItem(CustCNV *pCNV, QStandardItem *pItem, QObject *parent)
    : QObject(parent), m_pCnv(pCNV), m_pItem(pItem)
{
    connect(pCNV, &CustCNV::recvData, [&](QVariant var){
        if(m_pItem == Q_NULLPTR)
            return;
        QString str = var.toString();
        m_pItem->setText(str);
    });
}
