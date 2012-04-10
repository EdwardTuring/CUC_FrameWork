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
    /*���潫�������Ϣװ�ص�data��*/
    params.addQueryItem("tags%5B%5D",tag);//�����"%5B%5D"Ӧ�ñ�php�˽�����Array
    params.addQueryItem("dir",dir);
    params.addQueryItem("filename",filename);
    params.addQueryItem("filedescription",filedescription);
    params.addQueryItem("file_start_put_time",file_start_put_time);
     params.addQueryItem("uid",uid);
    qDebug()<<"now the params string are:"+params.toString();
    data.append(params.toString());
    data.remove(0,1);
    /*post��ȥ*/
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
    /*���潫�������Ϣװ�ص�data��*/
    for(int i=0;i<tags.size();i++)
    {
        params.addQueryItem("tags%5B%5D",tags[i].toString());//�����"%5B%5D"Ӧ�ñ�php�˽�����Array
    }


    params.addQueryItem("dir",dir);
    params.addQueryItem("filename",filename);
    params.addQueryItem("filedescription",filedescription);
    params.addQueryItem("file_start_put_time",file_start_put_time);
     params.addQueryItem("uid",uid);
    qDebug()<<"now the params string are:"+params.toString();

    data.append(params.toString().toUtf8());
    data.remove(0,1);

    /*post��ȥ*/
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
    qDebug()<<"FtpDataHelper::postFinished():�õ�����ӦΪ��\n"+tmp_str;
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
