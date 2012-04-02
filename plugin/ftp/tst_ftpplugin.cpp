#ifdef CUC_TEST
#include "tst_ftpplugin.h"

Tst_FtpPlugin::Tst_FtpPlugin(FtpPlugin *ftpplugin, QObject *parent) :
    QObject(parent)
{
    connect(ftpplugin,SIGNAL(getFinished(QString)),this,SLOT(slot_getfinished(QString)));
}
void Tst_FtpPlugin::slot_getfinished(const QString &info)
{
    qDebug()<<info;
}
#endif
