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
    post 下面我实现一个http post方法，用来解决QtWebKit原生Ajax不稳定的问题。
    @param url： post的url地址；data:需要传输的数据；func:完成后的回调函数
    @author Ma Xiao(mxturing@yeah.net)
    @date 2012.4.9
    */
    void post(const QString &url,const QMap<QString, QVariant> &data,const QString &func);

    void showAboutDialog(const QString &msg="这是关于...");
    void debug(const QString &msg);
    void log(const QString &msg);
private slots:
    /**
      postFinshed 一个私有槽，当post请求完成之后调用；
      在函数体中，会执行相应的javascript函数
      @author Ma Xiao(mxturing@yeah.net)
      @date 2012.4.9
    */
    void slot_postFinished();

    void replyError(QNetworkReply::NetworkError code);
private://一些已有成员函数

private:


    QNetworkAccessManager *net_manager_;
   QNetworkReply *reply_;
   QList<ReplyQueue> replys_;
   QString func_;
   bool flag_;
};
}//namespace Web
#endif // JSOBJOS_H
