#ifndef CUSTCNVEDITOR_H
#define CUSTCNVEDITOR_H

#include "ui_CustCNVEditor.h"
class CustCNV;
class CustCNVEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CustCNVEditor(QWidget *parent = 0);
public Q_SLOTS:
    void onCNVPathChanged(QString str);
private:
    Ui::CustCNVEditor ui;
    CustCNV* m_pCust;
};

#endif // CUSTCNVEDITOR_H
