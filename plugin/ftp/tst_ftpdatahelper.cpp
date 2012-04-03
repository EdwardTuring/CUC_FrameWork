#ifdef CUC_TEST
#include "tst_ftpdatahelper.h"
Tst_FtpDataHelper::Tst_FtpDataHelper(FtpDataHelper *helper, QObject *parent):QObject(parent)
{
    qDebug()<<"TEST:FtpDataHelper";
    connect(helper,SIGNAL(tst_ftpDataFinished(const QString &)),this,SLOT(slot_tst_postFinished(const QString &)));
}
void Tst_FtpDataHelper::slot_tst_postFinished(const QString &info)
{
    qDebug()<<"Tst_FtpDataHelper::slot_tst_postFinished()£ºÏìÓ¦Îª£º"+info;
}

#endif
