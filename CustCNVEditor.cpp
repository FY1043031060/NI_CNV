#include "CustCNVEditor.h"
#include "CustCNV.h"
#include "CnvWid.h"
#include <QRegExp>
CustCNVEditor::CustCNVEditor(QWidget *parent) :
    QWidget(parent), m_pCust(Q_NULLPTR)
{
    ui.setupUi(this);
    CnvWid* wid = new CnvWid;
    wid->setParent(this, Qt::Window);
    wid->hide();
    connect(wid,&CnvWid::emitCNVPath, this, &CustCNVEditor::onCNVPathChanged);
    connect(ui.pushButtonCfg, &QPushButton::clicked,[&](){

       this->findChild<CnvWid*>()->show();
    });
}

void CustCNVEditor::onCNVPathChanged(QString str)
{
    ui.labelCNVPath->setText(str);
    if(m_pCust != Q_NULLPTR)
        m_pCust->deleteLater();
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

