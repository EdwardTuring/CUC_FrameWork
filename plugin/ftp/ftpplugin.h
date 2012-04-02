#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QtCore>
#include <QFtp>
#include "../CUCplugininterface.h"
namespace Plugin
{
    class FtpPlugin;
    class CUCPluginInterface;

}
/*
*   FtpTask ������ftp�ϡ��´�������ࡣ
*/
class FtpTask
{
public:
    QString filename;
    QString des_url;
    QString url;
    QString info;//!important:���ڴ�����Ϣ
    int type;//type:0=>get,1=>put
    //�������캯��
    FtpTask(const FtpTask &other);
    FtpTask();
};

/*
* TaskManager �����ڹ���ftp�����ϡ��´����񡣴������Ӧ����
FtpPlugin��ĳ�Ա����FtpPlugin����á�����js����Ľ�����
*/
class TaskManager:public QObject
{
    Q_OBJECT
public:
    TaskManager(QObject *parent=0);
    ~TaskManager();
    void addGetTask(const QString &info,const QString &url, const QString &des_url);
    void addPutTask(const QString &info,const QString &url,const QString &file_name);
   /*ɾ������Ϊi������*/
    void deleteTask(int i);
    /*�����������ɣ���ôList����Ԫ��Ӧ��shift*/
    void finishTask();
    /*��ȡ���ж��˵�task*/
    FtpTask getTopTask();
    bool isQueueEmpty() const;
private:

    QList<FtpTask> *list_;
};

class FtpPlugin:public QObject,public CUCPluginInterface
{
    Q_OBJECT
 #ifndef CUC_TEST
    Q_INTERFACES(CUCPluginInterface)
#endif
public:
     FtpPlugin(QObject *parent=0);
public slots:

    virtual void debug() const;
    virtual QString pluginName() const;
    virtual QString pluginVersion() const;
    virtual QString pluginIcoUrl() const;

     void ftpCommandFinished(int, bool error);
    /*���淽������TaskManager���ͬ������*/

     void addGetTask(const QString &info,const QString &url, const QString &des_url);
     void addPutTask(const QString &info,const QString &url,const QString &file_name);
     void deleteTask(int i);

     /*isQueueEmpty:�ж���������Ƿ�Ϊ��*/
     bool isQueueEmpty() ;

     int	cd ( const QString & dir );




     int	close ();
     int	connectToHost( const QString & host, QString  port = "21" );
     int mkdir(const QString & foldername);

     int	get ( const QString & srcfileName, const QString &fileName);
     int    put(const QString & choosed_files_dir_, const QString file_name);
     int    put(const QString & choosed_files_dir_);

     int	list ( const QString & dir = QString() );
     int	login ( const QString & user = QString(), const QString & password = QString() );
protected slots:
     void updateDataTransferProgress(qint64 readBytes,qint64 totalBytes);
     void getListInfo(const QUrlInfo & i);
signals:
     void getFinished(const QString &);
     void putFinished(const QString &);
     void mkdirfinished(bool);
     void listInfo( QString,bool);
     void dataTransferProgress(QString,QString);
     void signal_startNextTask();
private slots:
     /*��ʼ��һ������*/
     void startNextTask();
private:
     QFtp *ftp;
     QFile *file_;
     QString uploadfilename_;
     QString host_;
     QString port_;
     QString user_;
     QString pwd_;
     TaskManager *manager_;
};


#endif // FTPPLUGIN_H
