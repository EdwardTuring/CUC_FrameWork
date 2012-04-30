#ifdef CUC_TEST
#include "tst_ftpplugin.h"

Tst_FtpPlugin::Tst_FtpPlugin(FtpPlugin *ftpplugin, QObject *parent) :
    QObject(parent)
{
    ftpplugin_=ftpplugin;
    connect(ftpplugin_,SIGNAL(getFinished(QString)),this,SLOT(slot_getfinished(QString)));
    connect(ftpplugin_,SIGNAL(hasGetTaskUnfinished(QString,QString,QString)),
            this,SLOT(slot_hasGetTaskUnfinished(QString,QString,QString)));
    connect(ftpplugin_,SIGNAL(hasPutTaskUnfinished(QString,QMap<QString,QVariant>,QString,QString)),
            this,SLOT(slot_hasPutTaskUnfinished(QString,QMap<QString,QVariant>,QString,QString)));
}
void Tst_FtpPlugin::slot_getfinished(const QString &info)
{
    qDebug()<<info;
}
void Tst_FtpPlugin::slot_hasGetTaskUnfinished(const QString &info,
                                              const QString &url,
                                              const QString &des_url)
{

    qDebug()<<"Tst_FtpPlugin::slot_hasGetTaskUnfinished():called";

    ftpplugin_->addGetTask(info,url,des_url);

}
void Tst_FtpPlugin::slot_hasPutTaskUnfinished(const QString &data_url,
                                              const QMap<QString, QVariant> &datas,
                                              const QString &url,
                                              const QString &file_name)
{
     qDebug()<<"Tst_FtpPlugin::slot_hasPutTaskUnfinished():called";

     ftpplugin_->addPutTask(data_url,datas,url,file_name);
}

#endif
