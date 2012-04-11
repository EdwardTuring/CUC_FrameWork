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
         //���շ������˵�php�ű���ָ���Ĺ��򣬵����ص��ǿ��ַ���ʱ������֤ʧ�ܣ�
         if(tmp_str=="")
         {
             //������֤ʧ�ܵ��ź�
             emit wrongVerifCode();
             qDebug()<<"��֤ʧ��";
             return ;
         }
    qDebug()<<tmp_str;
        QDomDocument doc;
          doc.setContent(tmp_str);
        //TODO:��xml�ַ���������һ���ṹ���У�����ʹ��

        QDomElement docElem = doc.documentElement();
        //qDebug()<<docElem.tagName();
        QString customer_name=docElem.attribute("name");
        QDomElement server_el=docElem.firstChildElement("server");
        QString host=server_el.firstChildElement("host").text();
        QString path=server_el.firstChildElement("path").text();
        QString url=host+"/"+path;
        qDebug()<<"�ͻ���"+customer_name;
        qDebug()<<"��ַ��"+url;
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

    //��httpЭ���ȡ������Ϣ����tmp_str,ע�����ת����������qt�ṩ��
    //xml�����๹��һ��QDomElement����
    QByteArray tmp_a=reply->readAll();
    QString tmp_str=QString::fromUtf8(tmp_a);
    //���շ������˵�php�ű���ָ���Ĺ��򣬵����ص��ǿ��ַ���ʱ������֤ʧ�ܣ�
    if(tmp_str=="")
    {
        //������֤ʧ�ܵ��ź�
        emit httpProessFinished("",false);
        qDebug()<<"��֤ʧ��";
        return ;
    }
    QDomDocument doc;
    doc.setContent(tmp_str);
    QDomElement doc_el = doc.documentElement();

    //��xml�ַ���tmp_str�л�ȡ�й���Ϣ
    QString customer_name=doc_el.attribute("name");
    QDomElement server_el=doc_el.firstChildElement("server");
    QString host=server_el.firstChildElement("host").text();
    QString path=server_el.firstChildElement("path").text();
    QString url=host+"/"+path;

    //���õ��Ŀͻ����ֺͷ�������ַ�Զ�������ʽ����settings.dat��
    PlatformSetting t_setting(url,"����������ý����Ƶ���ݹ���ƽ̨");
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
        qDebug()<<"NetWorkManager::replyFinished()��write setting data completed";
    }

    //��������ַд��settings.dat�ļ�֮�󣬾Ϳ�����GUIģ�鷢����ɵ��ź��ˣ�ע��ҲҪ��
    //�ͻ�������һ�����͸�GUI��
    emit httpProessFinished(customer_name,true);

}
void NetWorkManager::downloadProess(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"NetWorkManager::downloadProess():"+QString::number(bytesReceived)+"/"+QString::number(bytesTotal);
    //����ֲ㷢�ͽ��ȣ�
    emit downloadProgress(bytesReceived,bytesTotal);
}
}

