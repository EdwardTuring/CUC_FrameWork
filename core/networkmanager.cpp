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
    qDebug()<<"NetWorkManager::tst_getFilesWithHttp():get���̽���";
    QFileInfo info(url_->path());
    QFile file(getCurrentDir()+"log/"+info.fileName());
    if(!(file.open(QIODevice::WriteOnly)))
    {
        //TODO���޷����ļ��Ĵ�����
        emit testFaild("�޷����ļ���"+info.fileName());
    }
    else
    {
        QTextStream out(&file);
        QByteArray tmp_a=reply->readAll();
         QString tmp_str=QString::fromUtf8(tmp_a);
       //test
        QDomDocument d;
          d.setContent(tmp_str);
        //TODO:��xml�ַ���������һ���ṹ���У�����ʹ��
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
    //TODO:����get����ʧ�ܵ��źţ�BrowserӦ���ܹ����񵽣�
}

void NetWorkManager::get()
{
    qDebug()<<"NetWorkManager::get():get���̿�ʼ��url:"+url_->toString();
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
    //TODO:����õ���Ϣ����settings.dat��



}
void NetWorkManager::downloadProess(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"NetWorkManager::downloadProess():"+QString::number(bytesReceived)+"/"+QString::number(bytesTotal);
}

#endif
