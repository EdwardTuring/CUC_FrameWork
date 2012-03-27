#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "qt4.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace CUCCore
{
class PlatformSetting;
class NetWorkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetWorkManager(const QString &url,QObject *parent = 0);
    void get();
    ~NetWorkManager();
signals:
#ifdef CUC_TEST
    void testFaild(QString );

#endif
   /*
    *
    httpProessFinished信号：当http请求完成的时候发送，第一个参数是客户的名字（通常是组织的名字）；
    第二个参数是验证通过与否的bool值。
    */
    void httpProessFinished(QString,bool);
     void downloadProgress(qint64 , qint64 );
     void wrongVerifCode();
public slots:
    void replyFinished(QNetworkReply *);

private slots:
    void replyError(QNetworkReply::NetworkError);
    void replyReadyRead();
    void downloadProess( qint64 bytesReceived, qint64 bytesTotal );
#ifdef CUC_TEST
    /*
     *
      此测试函数用于测试代码的http功能是否可行。
      若成功，会在程序的根目录的log文件夹下得到一个下载完成的文件。
   */
    void tst_getFilesWithHttp(QNetworkReply *reply);
#endif
private:

private:
    QUrl *url_;
    QNetworkReply *reply_;


};
}
#endif // NETWORKMANAGER_H
