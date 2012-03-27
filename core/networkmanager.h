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
    httpProessFinished�źţ���http������ɵ�ʱ���ͣ���һ�������ǿͻ������֣�ͨ������֯�����֣���
    �ڶ�����������֤ͨ������boolֵ��
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
      �˲��Ժ������ڲ��Դ����http�����Ƿ���С�
      ���ɹ������ڳ���ĸ�Ŀ¼��log�ļ����µõ�һ��������ɵ��ļ���
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
