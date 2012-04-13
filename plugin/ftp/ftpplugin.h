#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QtCore>
#include <QFtp>
#include "ftpdatahelper.h"
#include "../CUCplugininterface.h"
#include "../core/coreerror.h"


/*
*   FtpTask 单个的ftp上、下传任务的类。
*/


class FtpTask
{
public:
    QString filename;
    QString des_url;//下载时用到
    QString url; //ftp服务器上文件的url
    QString info;//!important:用于传递信息
    int type;//type:0=>get,1=>put


    /*下面的两个变量用于存放post的信息*/
    QString post_data_url_;
    QMap<QString,QVariant> post_datas_;
     //拷贝构造函数
    FtpTask(const FtpTask &other);
    FtpTask();
    ~FtpTask();
};
/**
  两个运算符重载函数 处理FtpTask类的输入输出流。
  @date 2012.4.13
*/

   QDataStream & operator<< (QDataStream& stream, const FtpTask& task);
   QDataStream & operator>> (QDataStream& stream, FtpTask& task);


/*
* TaskManager 类用于管理ftp批量上、下传任务。此类对象应该是
FtpPlugin类的成员，被FtpPlugin类调用。（与js代码的交互）
*/
class TaskManager:public QObject
{
    Q_OBJECT

public:
    TaskManager(QObject *parent=0);
    ~TaskManager();
    void addGetTask(const QString &info,const QString &url, const QString &des_url);

    void addPutTask(const QString &info,
                    const QString &post_data_url,
                    const QMap<QString,QVariant> &post_datas,
                    const QString &url,const QString &file_name);
    /*删除序列为i的任务*/
    void deleteTask(int i);
    /*如果有任务完成，那么List的首元素应该shift*/
    void finishTask();
    /*获取队列顶端的task*/
    FtpTask getTopTask();
    bool isQueueEmpty() const;
signals:
    void fatalError(const QString &);
    void taskQueueChanged();
private slots:
   /**
     writeTaskInfoToFile 每次任务队列改变的时候调用此接口,
     用于记录任务队列。
    */
    void writeTaskInfoToFile();
private:

    QFile *task_store_file_;

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
    /*下面方法调用TaskManager类的同名函数*/

    void addGetTask(const QString &info,const QString &url, const QString &des_url);
   /**
    *为了避免post数据导致ftp上传被挂起，我把post的相关参数直接放在FtpTask中；
    *所以，在这里我需要利用addPutTask传入这些post参数:post的data_url地址，post的数据datas
    @author Ma Xiao
    @date 2012.4.5
    */

    void addPutTask(const QString &data_url,
                    const QMap<QString,QVariant> &datas,
                    const QString &url,
                    const QString &file_name);

    void deleteTask(int i);

    /*isQueueEmpty:判断任务队列是否为空*/
    bool isQueueEmpty() ;

    int	cd ( const QString & dir );

    /*调用FtpDataHelper的同名函数*/
    void postFtpData(const QString &url,
                     const QString &tag,
                     const QString &dir,
                     const QString &filename,
                     const QString &filedescription, const QString &file_start_put_time,const QString &uid);

    /*重载一下postFtpData成员函数，使用回方便些*/
    void postFtpData(const QString &url,const QMap<QString, QVariant>& obj);

    int	close ();
    int	connectToHost( const QString & host, QString  port = "21" );
    int mkdir(const QString & foldername);

    int    get ( const QString & srcfileName, const QString &fileName);
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
    void listFinished();
    void signal_PostDataFinished(const QString &);
private slots:

    /**
    slot_ftpDataFinished 这个槽做两件事情：
    1、开启任务管理器里的下一次任务。
    2、再次发送一个http post完成的信号，javaScript部分应该能够捕捉到此信号，并作出相因的处理

    @date 2012.4.5
    */
    void slot_ftpDataFinished(const QString &msg);
    /**
    slot_startNextTask 当上一次的任务完成的时候，检查是否为put类型；
    若是，则触发post任务。

    @date 2012.4.5
    */
    void slot_startNextTask();

    /**
    handleFatalError 处理致命错误的槽
   @date 2012.4.13
    */
    void handleFatalError(const QString &);
private://私有成员函数：
    /*开始下一个任务*/
    void startNextTask();

private:
    QFtp *ftp;
    QFile *file_;
    QFile *saver_file_;//记录当前任务的进度，断点续传使用


    QString uploadfilename_;
    QString host_;
    QString port_;
    QString user_;
    QString pwd_;
    TaskManager *manager_;

    FtpDataHelper *data_helper_;
};


#endif // FTPPLUGIN_H
