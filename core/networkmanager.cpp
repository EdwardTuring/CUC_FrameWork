#include "networkmanager.h"
#include "tool.h"
#include "configparser.h"
 #include <QDomElement>
namespace CUCCore{
NetWorkManager::NetWorkManager(const QString &url,QObject *parent) :
    QNetworkAccessManager(parent)
{
    url_=new QUrl(url);
    reply_=NULL;

    qDebug()<<"NetWorkManager::NetWorkManager():NetWorkManager created";
#ifdef CUC_TEST
    CONNECT(this,finished(QNetworkReply*),this,tst_getFilesWithHttp(QNetworkReply*));
#else
    CONNECT(this,finished(QNetworkReply*),this,replyFinished(QNetworkReply*));
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
         //按照服务器端的php脚本所指定的规则，当返回的是空字符串时，则验证失败；
         if(tmp_str=="")
         {
             //发送验证失败的信号
             emit wrongVerifCode();
             qDebug()<<"验证失败";
             return ;
         }
    qDebug()<<tmp_str;
        QDomDocument doc;
          doc.setContent(tmp_str);
        //TODO:将xml字符串解析到一个结构体中，后面使用

        QDomElement docElem = doc.documentElement();
        //qDebug()<<docElem.tagName();
        QString customer_name=docElem.attribute("name");
        QDomElement server_el=docElem.firstChildElement("server");
        QString host=server_el.firstChildElement("host").text();
        QString path=server_el.firstChildElement("path").text();
        QString url=host+"/"+path;
        qDebug()<<"客户："+customer_name;
        qDebug()<<"地址："+url;
        out<<tmp_a;
        file.close();

        reply->deleteLater();
    }


}
#endif
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

    //将http协议获取到的信息存入tmp_str,注意编码转换；并利用qt提供的
    //xml解析类构建一个QDomElement对象。
    QByteArray tmp_a=reply->readAll();
    QString tmp_str=QString::fromUtf8(tmp_a);
    //按照服务器端的php脚本所指定的规则，当返回的是空字符串时，则验证失败；
    if(tmp_str=="")
    {
        //发送验证失败的信号
        emit httpProessFinished("",false);
        qDebug()<<"验证失败";
        return ;
    }
    QDomDocument doc;
    doc.setContent(tmp_str);
    QDomElement doc_el = doc.documentElement();

    //从xml字符串tmp_str中获取有关信息
    QString customer_name=doc_el.attribute("name");
    QDomElement server_el=doc_el.firstChildElement("server");
    QString host=server_el.firstChildElement("host").text();
    QString path=server_el.firstChildElement("path").text();
    QString url=host+"/"+path;

    //将得到的客户名字和服务器地址以二进制形式存入settings.dat中
    PlatformSetting t_setting(url,"北京城乡新媒体视频内容管理平台");
    QFile file(qApp->applicationDirPath()+"/data/settings.dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        FILE_OPEN_ERROR(file);


    }
    else
    {
        QDataStream out(&file);
        out<<t_setting.getHostUrl();
        out<<t_setting.getWindowTitle();
        out<<t_setting.getWindowISMaxsize();
        out<<t_setting.getWindowWidth();
        out<<t_setting.getWindowHeight();
        out.setVersion(QDataStream::Qt_4_0);
        qDebug()<<"NetWorkManager::replyFinished()：write setting data completed";
    }

    //服务器地址写入settings.dat文件之后，就可以向GUI模块发送完成的信号了，注意也要将
    //客户的名字一并发送给GUI。
    emit httpProessFinished(customer_name,true);

}
void NetWorkManager::downloadProess(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"NetWorkManager::downloadProess():"+QString::number(bytesReceived)+"/"+QString::number(bytesTotal);
    //向呈现层发送进度：
    emit downloadProgress(bytesReceived,bytesTotal);
}
}

