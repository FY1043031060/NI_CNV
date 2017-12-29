#ifndef CNVWID_H
#define CNVWID_H

#include "ui_CnvWid.h"
#include <cvinetv.h>
#include <cvirte.h>
#include <ansi_c.h>
#include <utility.h>
class CnvWid : public QWidget
{
    Q_OBJECT

public:
    explicit CnvWid(QWidget *parent = 0);

    void searchRecursivly(CNVBrowser cnvbrowser, const char *strPath);

private:
    Ui::CnvWid ui;
    void getErrorDescripter(int status);
};

#endif // CNVWID_H
