#include "ftpdatahelper.h"
#include <assert.h>
#include <QNetworkRequest>

#include <QByteArray>
FtpDataHelper::FtpDataHelper(QObject *parent):QObject(parent)
{
    qDebug()<<"FtpDataHelper::FtpDataHelper():Class FtpDataHelper Created";
    manager_=NULL;

}


void FtpDataHelper::postFtpData(const QString &url,
                                const QString &tag,
                                const QString &dir,
                                const QString &filename,
                                const QString &filedescription,
                                const QString &file_start_put_time)
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
    qDebug()<<"now the params string are:"+params.toString();
    data.append(params.toString());
    data.remove(0,1);
    /*post出去*/
    QNetworkReply *reply=manager_->post(request,data);
#ifndef CUC_TEST
    connect(manager_,SIGNAL(finished(QNetworkReply*)),manager_,SLOT(postFinished(QNetworkReply*)));
      connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(manager_,SIGNAL(finished(QNetworkReply*)),this,SLOT(tst_postFinished(QNetworkReply*)));

#endif

}
void FtpDataHelper::postFinished(QNetworkReply* reply)
{
    QByteArray tmp=reply->readAll();
    QString tmp_str(tmp);
    qDebug()<<"FtpDataHelper::postFinished():得到的响应为：\n"+tmp_str;
    reply->deleteLater();
    emit ftpDataFinished(tmp_str);
}
#ifdef CUC_TEST
void FtpDataHelper::tst_postFinished(QNetworkReply *reply)
{
    QByteArray tmp=reply->readAll();
    QString tmp_str(tmp);
    reply->deleteLater();
    emit tst_ftpDataFinished(tmp_str);
}
#endif
void FtpDataHelper::replyError(QNetworkReply::NetworkError code)
{
    qDebug()<<" FtpDataHelper::replyError():ERROR "+QString::number(code);
}
