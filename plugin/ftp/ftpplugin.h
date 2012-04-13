#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QtCore>
#include <QFtp>
#include "ftpdatahelper.h"
#include "../CUCplugininterface.h"
#include "../qt4.h"
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

    /*����������������ڴ��post����Ϣ*/
    QString post_data_url_;
    QMap<QString,QVariant> post_datas_;

    FtpTask(const FtpTask &other);
    FtpTask();
    ~FtpTask();
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

    void addPutTask(const QString &info,
                    const QString &post_data_url,
                    const QMap<QString,QVariant> &post_datas,
                    const QString &url,const QString &file_name);
    /*ɾ������Ϊi������*/
    void deleteTask(int i);
    /*�����������ɣ���ôList����Ԫ��Ӧ��shift*/
    void finishTask();
    /*��ȡ���ж��˵�task*/
    FtpTask getTopTask();
    bool isQueueEmpty() const;
private:
    void writeTaskInfoToFile(const FtpTask &);
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
    /*���淽������TaskManager���ͬ������*/

    void addGetTask(const QString &info,const QString &url, const QString &des_url);
   /**
    *Ϊ�˱���post���ݵ���ftp�ϴ��������Ұ�post����ز���ֱ�ӷ���FtpTask�У�
    *���ԣ�����������Ҫ����addPutTask������Щpost����:post��data_url��ַ��post������datas
    @author Ma Xiao
    @date 2012.4.5
    */

    void addPutTask(const QString &data_url,
                    const QMap<QString,QVariant> &datas,
                    const QString &url,
                    const QString &file_name);

    void deleteTask(int i);

    /*isQueueEmpty:�ж���������Ƿ�Ϊ��*/
    bool isQueueEmpty() ;

    int	cd ( const QString & dir );

    /*����FtpDataHelper��ͬ������*/
    void postFtpData(const QString &url,
                     const QString &tag,
                     const QString &dir,
                     const QString &filename,
                     const QString &filedescription, const QString &file_start_put_time,const QString &uid);

    /*����һ��postFtpData��Ա������ʹ�ûط���Щ*/
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
    slot_ftpDataFinished ��������������飺
    1��������������������һ������
    2���ٴη���һ��http post��ɵ��źţ�javaScript����Ӧ���ܹ���׽�����źţ�����������Ĵ���
    @author Ma Xiao(mxturing@yeah.net)
    @date 2012.4.5
    */
    void slot_ftpDataFinished(const QString &msg);
    /**
    slot_startNextTask ����һ�ε�������ɵ�ʱ�򣬼���Ƿ�Ϊput���ͣ�
    ���ǣ��򴥷�post����
    @author Ma Xiao(mxturing@yeah.net)
    @date 2012.4.5
    */
    void slot_startNextTask();

private://˽�г�Ա������
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

    FtpDataHelper *data_helper_;
};


#endif // FTPPLUGIN_H
