#ifndef JSOBJOS_H
#define JSOBJOS_H

#include "../../qt4.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class ReplyQueue
{
public:
    QString func;
    QNetworkReply *reply;
    ReplyQueue(const QString &func,QNetworkReply *r);
    ReplyQueue(const ReplyQueue &other);
};

namespace Web{


class JSOBJWindowx : public QObject
{
    Q_OBJECT
public:
    explicit JSOBJWindowx(QObject *parent = 0);

signals:

public slots:
    void showMaxSize();
    void showNormal();
    void showFullScreen();
    void quit();
    void closeChildWindow(const QString title);
    QString getChildWindow();
    /**
    post ������ʵ��һ��http post�������������QtWebKitԭ��Ajax���ȶ������⡣
    @param url�� post��url��ַ��data:��Ҫ��������ݣ�func:��ɺ�Ļص�����
    @author Ma Xiao(mxturing@yeah.net)
    @date 2012.4.9
    */
    void post(const QString &url,const QMap<QString, QVariant> &data,const QString &func);

    void showAboutDialog(const QString &msg="���ǹ���...");
    void debug(const QString &msg);
    void log(const QString &msg);
private slots:
    /**
      postFinshed һ��˽�вۣ���post�������֮����ã�
      �ں������У���ִ����Ӧ��javascript����
      @author Ma Xiao(mxturing@yeah.net)
      @date 2012.4.9
    */
    void slot_postFinished();

    void replyError(QNetworkReply::NetworkError code);
private://һЩ���г�Ա����

private:


    QNetworkAccessManager *net_manager_;
   QNetworkReply *reply_;
   QList<ReplyQueue> replys_;
   QString func_;
   bool flag_;
};
}//namespace Web
#endif // JSOBJOS_H
