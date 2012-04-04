#ifdef CUC_TEST
#include "tst_ftpdatahelper.h"
Tst_FtpDataHelper::Tst_FtpDataHelper(FtpDataHelper *helper, QObject *parent):QObject(parent)
{
    qDebug()<<"TEST:FtpDataHelper";
    this->helper_=helper;
    connect(helper_,SIGNAL(tst_ftpDataFinished(const QString &)),this,SLOT(slot_tst_postFinished(const QString &)));

}
void Tst_FtpDataHelper::slot_tst_postFinished(const QString &info)
{
    qDebug()<<"Tst_FtpDataHelper::slot_tst_postFinished()£ºÏìÓ¦Îª£º"+info;
    QMap<QString,QVariant> obj;
    QList<QVariant> tags;
    QVariant var("Âíæç");
    tags.append(var);

    obj.insert("tags",QVariant(tags));
    obj.insert("dir",QVariant("12"));
      obj.insert("filename",QVariant("°¡°¡"));
        obj.insert("filedescription",QVariant("°¡°¡"));
          obj.insert("file_start_put_time",QVariant("1243"));
  obj.insert("uid",QVariant("1"));
    helper_->postFtpData("http://localhost/ftp_client/index.php/welcome/addtag",
                       tags,
                       obj["dir"].toString(),
                       obj["filename"].toString(),
                       obj["filedescription"].toString(),
                       obj["file_start_put_time"].toString(),
                       obj["uid"].toString());
}

#endif
