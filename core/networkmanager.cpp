#include "networkmanager.h"
#include "tool.h"
 #include <QDomElement>
NetWorkManager::NetWorkManager(const QString &url,QObject *parent) :
    QNetworkAccessManager(parent)
{
    url_=new QUrl(url);
    reply_=NULL;

    qDebug()<<"NetWorkManager::NetWorkManager():NetWorkManager created";
#ifdef CUC_TEST
    CONNECT(this,finished(QNetworkReply*),this,tst_getFilesWithHttp(QNetworkReply*));
#else
    CONNECT(manager_,finished(QNetworkReply*),this,replyFinished(QNetworkReply*));
#endif

}
#ifdef CUC_TEST
void NetWorkManager::tst_getFilesWithHttp(QNetworkReply *reply)
{
    qDebug()<<"NetWorkManager::tst_getFilesWithHttp():get过程结束";
    QFileInfo info(url_->path());
    QFile file(getCurrentDir()+"log/"+info.fileName());
    if(!(file.open(QIODevice::WriteOnly)))
    {
        //TODO：无法打开文件的错误处理
        emit testFaild("无法打开文件："+info.fileName());
    }
    else
    {
        QTextStream out(&file);
        QByteArray tmp_a=reply->readAll();
         QString tmp_str=QString::fromUtf8(tmp_a);
       //test
        QDomDocument d;
          d.setContent(tmp_str);
        //TODO:将xml字符串解析到一个结构体中，后面使用
        //test end

        qDebug()<<tmp_str;
        out<<tmp_a;
        file.close();

        reply->deleteLater();
    }


}
 NetWorkManager::~NetWorkManager()
{

}

void NetWorkManager::replyError(QNetworkReply::NetworkError error)
{
#ifdef CUC_TEST
    emit testFaild("NetWorkManager::replyError():QNetworkReply::NetworkError("+QString::number(error)+")");
#endif
    //TODO:发送get请求失败的信号（Browser应该能够捕获到）
}

void NetWorkManager::get()
{
    qDebug()<<"NetWorkManager::get():get过程开始，url:"+url_->toString();
    QNetworkRequest request(*url_);

    request.setRawHeader("User-Agent", "CUC_FrameWork 1.4");
    request.setRawHeader("Content-type","xml; charset=utf-8");
    reply_=QNetworkAccessManager::get(request);
    CONNECT(reply_,readyRead(),this,replyReadyRead());
    CONNECT(reply_,error(QNetworkReply::NetworkError),this,replyError(QNetworkReply::NetworkError));
    CONNECT(reply_,	downloadProgress ( qint64 , qint64  ),this,downloadProess(qint64,qint64));
}
void NetWorkManager::replyReadyRead()
{
    qDebug()<<"NetWorkManager::replyReadyRead():called";
}

void NetWorkManager::replyFinished(QNetworkReply *reply)
{
    qDebug()<<"NetWorkManager::replyFinished():called";
    //TODO:将获得的信息存入settings.dat中



}
void NetWorkManager::downloadProess(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"NetWorkManager::downloadProess():"+QString::number(bytesReceived)+"/"+QString::number(bytesTotal);
}

#endif
