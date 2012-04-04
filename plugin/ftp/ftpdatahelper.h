#ifndef FTPDATAHELPER_H
#define FTPDATAHELPER_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/**
  FtpDataHelper:��������ftp�����FtpPlugin������������������Ϣ��
  ���ٵ���$.post�Ĳ��ȶ��������ѵ�������������δ����QtWebKit�汾�����bug����
    ���������ʱ���������Ľ��ᱻ$.postȡ������
   @author Ma Xiao(mxturing@yeah.net)
   @date 2012.4.3
  */
class FtpDataHelper:public QObject
{
    Q_OBJECT
public:
     FtpDataHelper(QObject *parent=0);
signals:
     /*���������ݷ�����ɣ����������ݷ���ʱ�����ʹ��ź�*/
     void ftpDataFinished(const QString &);
#ifdef CUC_TEST
     void tst_ftpDataFinished(const QString &);
#endif
public slots:



    /**
      * postFtpData:�����Ա������js�ű��л�ȡ���루5��Qstring �ַ�����
      ��ص�json������ʾ�� data={
                        "tags":null,
                        "dir":null,
                        "filename":null,
                        "filedescription":null,
                        "file_start_put_time":null
                        };
      Ȼ��ͨ��Qt��http��post�����������json�����ݵ���������
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

     /*ע�⣺��������������ִ�д˳�Ա�����Ա�֤��WebPage���
        ����ͨ��һ��*/
 public:
     void setNetWorkManager(QNetworkAccessManager *manager){manager_=manager;}

private slots:
     void replyError(QNetworkReply::NetworkError);
     void postFinished();
#ifdef CUC_TEST
     void tst_postFinished();

#endif
private:
    /*qt ����ͨ�Ź����࣬ͨ�������ʵ��http post*/
    QNetworkAccessManager *manager_;

    QNetworkReply *reply_;
};


#endif // FTPDATAHELPER_H
