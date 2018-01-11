#include "CustCNVEditor.h"
#include "CustCNV.h"
#include "CnvWid.h"
#include <QRegExp>
#include "CustViewWid.h"
CustCNVEditor::CustCNVEditor(QWidget *parent) :
    QWidget(parent), m_pCust(Q_NULLPTR)
{
#if 1
    ui.setupUi(this);
    CustViewWid* pView = new CustViewWid;
    pView->setParent(this, Qt::Window);
    pView->hide();
    connect(pView, &CustViewWid::emitCNVPath,this, &CustCNVEditor::onCNVPathChanged);
    connect(ui.pushButtonCfg,&QPushButton::clicked, [&](){
        this->findChild<CustViewWid*>()->show();
    });
#else
    CnvWid* wid = new CnvWid;
    wid->setParent(this, Qt::Window);
    wid->hide();
    connect(wid,&CnvWid::emitCNVPath, this, &CustCNVEditor::onCNVPathChanged);
    connect(ui.pushButtonCfg, &QPushButton::clicked,[&](){

       this->findChild<CnvWid*>()->show();
    });
#endif
}

void CustCNVEditor::onCNVPathChanged(QString str)
{
    ui.labelCNVPath->setText(str);
    if(m_pCust != Q_NULLPTR)
    {
//        m_pCust->deleteLater();
        delete m_pCust;
    }
    m_pCust = new CustCNV(str, this);

    connect(m_pCust,&CustCNV::recvData, [&](QVariant var){
        if(var.type() == QVariant::Double )
            ui.lineEditCNVOut->setText(QString("%1").arg(var.toDouble()));
    });

    connect(ui.lineEditCNVIn,&QLineEdit::textChanged,[&](QString strValue){
        QRegExp spliter("(\\d+)");
        bool ok;
        double dValue = strValue.section(spliter, 0).toDouble(&ok);
        if(ok)
            m_pCust->sendData(QVariant::fromValue(dValue));
    });
}

