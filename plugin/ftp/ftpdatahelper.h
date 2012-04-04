#ifndef FTPDATAHELPER_H
#define FTPDATAHELPER_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/**
  FtpDataHelper:这个类帮助ftp插件（FtpPlugin）向服务器传递相关信息。
  （操蛋的$.post的不稳定，不得已的做法。或许在未来的QtWebKit版本中这个bug可以
    被解决。那时，这个插件的将会被$.post取代。）
   @author Ma Xiao(mxturing@yeah.net)
   @date 2012.4.3
  */
class FtpDataHelper:public QObject
{
    Q_OBJECT
public:
     FtpDataHelper(QObject *parent=0);
signals:
     /*当本次数据发送完成，并且有数据返回时，发送此信号*/
     void ftpDataFinished(const QString &);
#ifdef CUC_TEST
     void tst_ftpDataFinished(const QString &);
#endif
public slots:



    /**
      * postFtpData:这个成员函数从js脚本中获取输入（5个Qstring 字符串）
      相关的json如下所示： data={
                        "tags":null,
                        "dir":null,
                        "filename":null,
                        "filedescription":null,
                        "file_start_put_time":null
                        };
      然后，通过Qt的http的post方法将这个“json”传递到服务器。
    */
     void postFtpData(const QString &url,
                      const QString &tag,
                      const QString &dir,
                      const QString &filename,
                      const QString &filedescription, const QString &file_start_put_time,const QString &uid);

     void FtpDataHelper::postFtpData(const QString &url,
                                     const QList<QVariant> &tag,
                                     const QString &dir,
                                     const QString &filename,
                                     const QString &filedescription,
                                     const QString &file_start_put_time, const QString &uid);

     /*注意：插件在载入后，首先执行此成员函数以保证与WebPage类的
        网络通信一致*/
 public:
     void setNetWorkManager(QNetworkAccessManager *manager){manager_=manager;}

private slots:
     void replyError(QNetworkReply::NetworkError);
     void postFinished();
#ifdef CUC_TEST
     void tst_postFinished();

#endif
private:
    /*qt 网络通信管理类，通过这个类实现http post*/
    QNetworkAccessManager *manager_;

    QNetworkReply *reply_;
};


#endif // FTPDATAHELPER_H
