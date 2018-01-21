#ifndef CCNVPROXYVIEW_H
#define CCNVPROXYVIEW_H

#include "ui_CCnvProxyView.h"

class CCnvProxyView : public QWidget
{
    Q_OBJECT

public:
    explicit CCnvProxyView(QWidget *parent = 0);
Q_SIGNALS:
    void emitCNVPath(QString strPath);
private:
    Ui::CCnvProxyView ui;
};

#endif // CCNVPROXYVIEW_H
