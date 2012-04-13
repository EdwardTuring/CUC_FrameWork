#include "ftpdatahelper.h"
#include <assert.h>
#include <QNetworkRequest>

#include <QByteArray>
FtpDataHelper::FtpDataHelper(QObject *parent):QObject(parent)
{
    qDebug()<<"FtpDataHelper::FtpDataHelper():Class FtpDataHelper Created";
    manager_=new QNetworkAccessManager(this);

}


void FtpDataHelper::postFtpData(const QString &url,
                                const QString &tag,
                                const QString &dir,
                                const QString &filename,
                                const QString &filedescription,
                                const QString &file_start_put_time,
                                const QString &uid)
{
    qDebug()<<"FtpDataHelper::postFtpData():called";
    assert(manager_);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QByteArray data;
    QUrl params;
    /*下面将输入的信息装载到data中*/
    params.addQueryItem("tags%5B%5D",tag);//后面的"%5B%5D"应该被php端解析成Array
    params.addQueryItem("dir",dir);
    params.addQueryItem("filename",filename);
    params.addQueryItem("filedescription",filedescription);
    params.addQueryItem("file_start_put_time",file_start_put_time);
     params.addQueryItem("uid",uid);
    qDebug()<<"now the params string are:"+params.toString();
    data.append(params.toString());
    data.remove(0,1);
    /*post出去*/
    reply_=manager_->post(request,data);
#ifndef CUC_TEST
    connect(reply_,SIGNAL(finished()),this,SLOT(postFinished()));
      connect(reply_,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply_,SIGNAL(finished()),this,SLOT(tst_postFinished()));

#endif

}
void FtpDataHelper::postFtpData(const QString &url,
                                const QList<QVariant> &tags,
                                const QString &dir,
                                const QString &filename,
                                const QString &filedescription,
                                const QString &file_start_put_time,
                                const QString &uid)
{
    qDebug()<<"FtpDataHelper::postFtpData():called";
    assert(manager_);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QByteArray data;
    QUrl params;
    /*下面将输入的信息装载到data中*/
    for(int i=0;i<tags.size();i++)
    {
        params.addQueryItem("tags%5B%5D",tags[i].toString());//后面的"%5B%5D"应该被php端解析成Array
    }


    params.addQueryItem("dir",dir);
    params.addQueryItem("filename",filename);
    params.addQueryItem("filedescription",filedescription);
    params.addQueryItem("file_start_put_time",file_start_put_time);
     params.addQueryItem("uid",uid);
    qDebug()<<"now the params string are:"+params.toString();

    data.append(params.toString().toUtf8());
    data.remove(0,1);

    /*post出去*/
    reply_=manager_->post(request,data);
     connect(reply_,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(replyError(QNetworkReply::NetworkError)));
#ifndef CUC_TEST
    connect(reply_,SIGNAL(finished()),this,SLOT(postFinished()));

#else
    connect(reply_,SIGNAL(finished()),this,SLOT(tst_postFinished()));

#endif

}
void FtpDataHelper::postFinished()
{

    QByteArray tmp=reply_->readAll();
    QString tmp_str(tmp);
    qDebug()<<"FtpDataHelper::postFinished():得到的响应为：\n"+tmp_str;
    reply_->deleteLater();
    emit ftpDataFinished(tmp_str);
}
#ifdef CUC_TEST
void FtpDataHelper::tst_postFinished()
{
    QByteArray tmp=reply_->readAll();
    QString tmp_str(tmp);
    reply_->deleteLater();
     emit ftpDataFinished(tmp_str);
    emit tst_ftpDataFinished(tmp_str);
}
#endif
void FtpDataHelper::replyError(QNetworkReply::NetworkError code)
{
    qDebug()<<" FtpDataHelper::replyError():ERROR "+QString::number(code);
}
