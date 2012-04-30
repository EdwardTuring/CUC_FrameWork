#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QtCore>
#include <QFtp>
#include <QSqlDatabase>
#include "ftpdatahelper.h"
#include "../CUCplugininterface.h"
#include "../core/coreerror.h"


/*
*   FtpTask ������ftp�ϡ��´�������ࡣ
*/


class FtpTask
{
public:
    QString filename;
    QString des_url;//����ʱ�õ�
    QString url; //ftp���������ļ���url
    QString info;//!important:���ڴ�����Ϣ
    int type;//type:0=>get,1=>put



    /*����������������ڴ��post����Ϣ*/
    QString post_data_url_;
    QMap<QString,QVariant> post_datas_;
     //�������캯��
    FtpTask(const FtpTask &other);
    FtpTask();
    ~FtpTask();
};
/**
  ������������غ��� ����FtpTask��������������
  @date 2012.4.13
*/

   QDataStream & operator<< (QDataStream& stream, const FtpTask& task);
   QDataStream & operator>> (QDataStream& stream, FtpTask& task);

/**
       SqlHelper Ϊ�˷�������û���������У�ʹ���ļ���ʱ��¼������в���
       ���㣬���Ҿ����Ե��ļ�����Ч��Ҳ�Ǹ����⣩���˴�ʹ��sqlite
      --һ�������͵����ݿ�,��Ϊ�û����еĴ洢��ʽ��
      @date 2012.4.15
 */
class SqlHelper:public QObject
{
  Q_OBJECT
  public:
    SqlHelper(QObject *parent=0);
    /*�������ݿ�*/
    bool connectDB();
    void closeDB();
private:
    QSqlDatabase db;
};

/*
* TaskManager �����ڹ���ftp�����ϡ��´����񡣴������Ӧ����
FtpPlugin��ĳ�Ա����FtpPlugin����á�
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

    QList<FtpTask> getTaskListp() {return *list_;}
    void setTaskStoreFile(const QString &);
signals:
    void fatalError(const QString &);
    void taskQueueChanged();
    void taskUnfinished(const  QList<FtpTask>  &);
private slots:
   /**
     writeTaskInfoToFile ÿ��������иı��ʱ����ô˽ӿ�,
     ���ڼ�¼������С�
    */
    void writeTaskInfoToFile();


private://˽�г�Ա����
    void initSqlHelper();
private:

    QFile *task_store_file_;

    SqlHelper *sql_helper_;

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
                    const QString &file_name,
                    const QString &info = "");

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


    void postFtpData(const QString &url,const QMap<QString, QVariant>& obj);

    int	close ();
    int	connectToHost( const QString & host, QString  port = "21" );
    int mkdir(const QString & foldername);

    int    get ( const QString & srcfileName, const QString &fileName,const QString &info = "");
    int    put(const QString & choosed_files_dir_, const QString file_name,const QString &info = "");
    int    put(const QString & choosed_files_dir_);

    int	list ( const QString & dir = QString() );
    int	login ( const QString & user = QString(), const QString & password = QString() );

    /**
    ftpTaskInit ��ʼ��ʹ��һ��δ��ɵ�����
    @param uid:�û�ID��user_name���û�����������������Ϊ��sqlite�в���
    δ�������ļ�¼�ļ����ļ��������ݡ�
    @date 2012.4.15
    */
    void ftpTaskInit(const QString &uid, const QString &user_name);

    /**
    rest ִ��ftpЭ��� REST����
    @date 2012.4.16
    */
    void rest(const QString &pos);
    /**
      getUnfinesedTaskPos ��ȡδ���������е����ֽ���
      @date 2012.4.16
    */
    QString getUnfinesedTaskPos();


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
    void hasGetTaskUnfinished(const QString &info,
                              const QString &url,
                              const QString &des_url,
                              const QString &file_name,
                              bool is_first_task=false);
    void hasPutTaskUnfinished(const QString &data_url,
                              const QMap<QString,QVariant> &datas,
                              const QString &url,
                              const QString &file_name,
                              bool is_first_task=false);

    void checkUnfinishedTaskOver();
    void AllTaskFinished();
private slots:

    /**
    slot_ftpDataFinished ��������������飺
    1��������������������һ������
    2���ٴη���һ��http post��ɵ��źţ�javaScript����Ӧ���ܹ���׽�����źţ�����������Ĵ���

    @date 2012.4.5
    */
    void slot_ftpDataFinished(const QString &msg);
    /**
    slot_startNextTask ����һ�ε�������ɵ�ʱ�򣬼���Ƿ�Ϊput���ͣ�
    ���ǣ��򴥷�post����

    @date 2012.4.5
    */
    void slot_startNextTask();

    void slot_continueLastTask(const  QList<FtpTask>  &);

    /**
    handleFatalError ������������Ĳ�
   @date 2012.4.13
    */
    void handleFatalError(const QString &);

    void slot_stateChanged(int state);
private://˽�г�Ա������
    /*��ʼ��һ������*/
    void startNextTask();
    qint64 getUnfinesedTaskPosqint64();
private:
    QFtp *ftp;
    QFile *file_;
    QFile *saver_file_;//��¼��ǰ����Ľ��ȣ��ϵ�����ʹ��

    QFile *choosed_file_;

    QString uploadfilename_;
    QString host_;
    QString port_;
    QString user_;
    QString pwd_;
    TaskManager *manager_;

    int ftp_state_;
    bool flag_cont_trans_;//�Ƿ�Ϊ�ϵ���������ı�־λ
    qint64 base_readbytes_;

    FtpDataHelper *data_helper_;
};


#endif // FTPPLUGIN_H
