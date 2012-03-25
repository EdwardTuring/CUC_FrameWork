#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "qt4.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
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
    QUrl *url_;
    QNetworkReply *reply_;


};

#endif // NETWORKMANAGER_H
