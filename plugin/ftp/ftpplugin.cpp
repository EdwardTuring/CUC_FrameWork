#include "ftpplugin.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <assert.h>

FtpTask::FtpTask(const FtpTask &other)
{
    this->des_url=other.des_url;
    this->url=other.url;
    this->type=other.type;
    this->filename=other.filename;
    this->info=other.info;
    this->post_data_url_=other.post_data_url_;
    this->post_datas_=other.post_datas_;
}
FtpTask::~FtpTask()
{

}

FtpTask::FtpTask()
{
    url="";
    des_url="";
    filename="";
    type=0;
    info="";
    post_data_url_="";


}


inline QDataStream & operator << (QDataStream& stream,
                                  const FtpTask& task)
{

    return stream<<task.des_url<<task.filename
                <<task.info<<task.post_datas_
               <<task.post_data_url_<<task.type
              <<task.url;
}
inline QDataStream & operator >>(QDataStream& stream,
                                 FtpTask& task)
{
    return stream>>task.des_url>>task.filename
                >>task.info>>task.post_datas_
               >>task.post_data_url_>>task.type
              >>task.url;
}
SqlHelper::SqlHelper(QObject *parent):QObject(parent)
{

}
bool SqlHelper::connectDB()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/mm_client.db");
    if(!db.open())
    {
        return false;
    }
    return true;

}
void SqlHelper::closeDB()
{
    db.close();
}

TaskManager::TaskManager(QObject *parent):QObject(parent)
{
    qDebug()<<"TaskManager::TaskManager():TaskManager created";
    this->list_=new QList<FtpTask>;
    //�����洢������е��ļ�����:
    task_store_file_= NULL;

    connect(this,SIGNAL(taskQueueChanged()),
            this,SLOT(writeTaskInfoToFile()));
    //��ʼ��sqlite������
    initSqlHelper();
}
void TaskManager::initSqlHelper()
{
    sql_helper_=new SqlHelper(this);
    if(!sql_helper_->connectDB())
    {
        //����sqliteʧ�ܡ�
        qDebug()<<"TaskManager::TaskManager():����sqliteʧ��";

        emit fatalError("�û���Ϣ����ʧ��(�޷����ӵ�sqlite)");
        return;
    }

}

TaskManager::~TaskManager()
{
    delete list_;
}
void TaskManager::writeTaskInfoToFile()
{
    if(!task_store_file_)
    {
        qDebug()<<"TaskManager::writeTaskInfoToFile():task_store_file_û�г�ʼ�����ǲ���û�е���FtpPlugin::ftpTaskInit()?";
    }
    else
    {
        if(!task_store_file_->isOpen())
        {
            if(!task_store_file_->open(QIODevice::WriteOnly))
            {
                FILE_OPEN_ERROR(*task_store_file_);
                emit fatalError("�޷����ļ���"+task_store_file_->fileName());
                return ;
            }
        }
        task_store_file_->reset();//ע�⣺����һ���ļ�ָ��

        QDataStream in(task_store_file_);
        // qDebug()<<"TaskManager::writeTaskInfoToFile():"<<list_->size();
        if(!list_->isEmpty())
            in<<*list_;
        else
        {
            QList<FtpTask> tmp_list;
            in<<tmp_list;
        }
        this->task_store_file_->close();
    }
}

void TaskManager::addGetTask(const QString &info,const QString &url, const QString &des_url)
{
    qDebug()<<"TaskManager::addGetTask():called";
    FtpTask task_tmp;
    task_tmp.des_url=des_url;
    task_tmp.url=url;
    task_tmp.type=0;
    task_tmp.info=info;
    QFileInfo tmp_fileinfo(des_url);
    task_tmp.filename=tmp_fileinfo.fileName();
    list_->append(task_tmp);
    emit taskQueueChanged();

}
void TaskManager::addPutTask(const QString &info,
                             const QString &post_data_url,
                             const QMap<QString,QVariant> &post_datas,
                             const QString &url, const QString &file_name)
{
    qDebug()<<"TaskManager::addPutTask():called";
    FtpTask task_tmp;
    task_tmp.filename=file_name;
    task_tmp.url=url;
    task_tmp.type=1;
    task_tmp.info=info;
    task_tmp.post_data_url_=post_data_url;
    task_tmp.post_datas_=post_datas;

    list_->append(task_tmp);
    emit taskQueueChanged();

}
bool TaskManager::isQueueEmpty() const
{
    return list_->isEmpty();
}
FtpTask TaskManager::getTopTask()
{
    if(!list_->isEmpty())
        return list_->first();
    // else
    //  return NULL;
}
void TaskManager::setTaskStoreFile(const QString &url)
{
    qDebug()<<"TaskManager::setTaskStoreFile():called";
    task_store_file_=new QFile(url,this);

    if(task_store_file_->exists())
    {

        if(!task_store_file_->open(QIODevice::ReadOnly))
        {
            FILE_OPEN_ERROR(*task_store_file_);
            emit fatalError("�޷����ļ���"+task_store_file_->fileName());
            return ;
        }

        QDataStream out(task_store_file_);
        QList<FtpTask> tmp_list;
        out>>tmp_list;
        this->task_store_file_->close();

        if(tmp_list.isEmpty())
        {

        }
        else
        {
            qDebug()<<"TaskManager::setTaskStoreFile():list_->size():"<<tmp_list.size();
            emit taskUnfinished(tmp_list);

        }

    }
    else
    {
        if(!task_store_file_->open(QIODevice::WriteOnly))
        {
            FILE_OPEN_ERROR(*task_store_file_);
            emit fatalError("�޷����ļ���"+task_store_file_->fileName());
            return ;
        }
    }

}

void TaskManager::deleteTask(int i)
{
    if(!list_->isEmpty())
    {
        list_->removeAt(i);
        emit taskQueueChanged();
    }
}
void TaskManager::finishTask()
{
    if(!list_->isEmpty())
    {
        list_->removeFirst();
        qDebug()<<"TaskManager::finishTask(): called";
        emit taskQueueChanged();
    }

}

FtpPlugin::FtpPlugin(QObject *parent):QObject(parent)
{

    ftp=new QFtp(this);

    manager_=new TaskManager(this);
    data_helper_=new FtpDataHelper(this);

    /*��ʼ������״̬��¼���ļ�*/
    QString tmp_saver_path= qApp->applicationDirPath() + "/data/saver";
    saver_file_=new QFile(tmp_saver_path,this);

    choosed_file_ = NULL;
    flag_cont_trans_ =false;
    ftp_state_ = 0;
    /*����FtpDataHelper���źŲ�*/
    connect(data_helper_,SIGNAL(ftpDataFinished(const QString &)),
            this,SLOT(slot_ftpDataFinished(const QString &)));
    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)),
            this, SLOT(getListInfo(QUrlInfo)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));
    /*���Ӵ��������Ϣ���źŲ�*/
    connect(manager_,SIGNAL(fatalError(const QString &)),
            this,SLOT(handleFatalError(const QString &)));
    connect(manager_,SIGNAL(taskUnfinished(const  QList<FtpTask>  &)),
            this,SLOT(slot_continueLastTask(const  QList<FtpTask>  &)));
    connect(this,SIGNAL(signal_startNextTask()),this,SLOT(slot_startNextTask()));

    connect(ftp,SIGNAL(stateChanged(int)),this,SLOT(slot_stateChanged(int)));
}

void FtpPlugin::postFtpData(const QString &url,
                            const QString &tag,
                            const QString &dir,
                            const QString &filename,
                            const QString &filedescription, const QString &file_start_put_time,const QString &uid)
{
    qDebug()<<"FtpPlugin::postFtpData():called";
    data_helper_->postFtpData(url,tag,dir,filename,filedescription,file_start_put_time,uid);
}
void FtpPlugin::handleFatalError(const QString &msg)
{
    qDebug()<<"FtpPlugin::handleFatalError():��������";
    QMessageBox::warning(NULL,"ftpģ��",msg);
}

void FtpPlugin::postFtpData(const QString &url, const QMap<QString, QVariant> &obj)
{
    qDebug()<<"FtpPlugin::postFtpData():called";
    QString tags_str;
    if(obj["tags"].type()==QVariant::List)
    {

        QList<QVariant> list=obj["tags"].toList();


        data_helper_->postFtpData(url,
                                  list,
                                  obj["dir"].toString(),
                                  obj["filename"].toString(),
                                  obj["filedescription"].toString(),
                                  obj["file_start_put_time"].toString(),
                                  obj["uid"].toString());
    }
    else
    {

        data_helper_->postFtpData(url,
                                  obj["tags"].toString(),
                                  obj["dir"].toString(),
                                  obj["filename"].toString(),
                                  obj["filedescription"].toString(),
                                  obj["file_start_put_time"].toString(),
                                  obj["uid"].toString());
    }


}
void FtpPlugin::slot_continueLastTask(const QList<FtpTask> &tmp_list)
{
    qDebug()<<"FtpPlugin::slot_continueLastTask():called";


    qDebug()<<"FtpPlugin::slot_continueLastTask():list_->size():"<<tmp_list.size();
    if(!tmp_list.isEmpty())
    {

        int i=0;
        for(;i<tmp_list.size();i++)
        {
            qDebug()<<"FtpPlugin::slot_continueLastTask():�� "<<i<<"��";
            FtpTask tmp_task=tmp_list.at(i);
            int type=tmp_task.type;
            if(type == 0)
            {

                if(i==0)
                {
                    flag_cont_trans_ = true;//���������ʶ������Ϊtrue
                    base_readbytes_ =this->getUnfinesedTaskPosqint64();
                    emit hasGetTaskUnfinished(tmp_task.info,tmp_task.url,tmp_task.des_url,tmp_task.filename,true);
                }
                else
                {
                    emit hasGetTaskUnfinished(tmp_task.info,tmp_task.url,tmp_task.des_url,tmp_task.filename);
                }

            }
            else  if(type == 1)
            {
                if(i == 0)
                {
                    flag_cont_trans_ = true;//���������ʶ������Ϊtrue
                    base_readbytes_ =this->getUnfinesedTaskPosqint64();
                    emit hasPutTaskUnfinished(tmp_task.post_data_url_,tmp_task.post_datas_,tmp_task.url,tmp_task.filename,true);
                }
                else
                {
                    emit hasPutTaskUnfinished(tmp_task.post_data_url_,tmp_task.post_datas_,tmp_task.url,tmp_task.filename);
                }

            }
        }
        if(i != 0)
            emit checkUnfinishedTaskOver();
    }
    else
    {

    }
}

void FtpPlugin::ftpTaskInit(const QString &uid,const QString &user_name)
{
    QSqlQuery query;
    query.exec("select data_file_name from task where uid="+uid);

    if(!query.next())
    {
        qDebug()<<" FtpPlugin::ftpTaskInit():����Ϊ��";
        QString tmp_qu_str="insert into task values( NULL,' "+user_name+" ', "+uid+ "  )";
        qDebug()<<"FtpPlugin::ftpTaskInit():ִ�в������:"<<query.exec(tmp_qu_str);

    }
    else{
        query.previous();//����һ��
        if(query.next())
        {

            QString tmp_file_name = query.value(0).toString();
            tmp_file_name=tmp_file_name.simplified();
            qDebug()<<" FtpPlugin::ftpTaskInit():"<<tmp_file_name<<" �м�¼";
            manager_->setTaskStoreFile("data/"+uid+"_"+tmp_file_name+".dat");


        }
    }
}
void FtpPlugin::rest(const QString &pos)
{
    qDebug()<<"FtpPlugin::rest():REST "<<pos;
    ftp->rawCommand("REST "+pos);
}
QString  FtpPlugin::getUnfinesedTaskPos()
{

    saver_file_->open(QIODevice::ReadOnly);
    QDataStream out(saver_file_);
    qint64 tmp_num;
    out>>tmp_num;
    saver_file_->close();
    qDebug()<<" FtpPlugin::getUnfinesedTaskPos():��һ����ɵ���"<<tmp_num;
    return QString::number(tmp_num);
}
qint64 FtpPlugin::getUnfinesedTaskPosqint64()
{
    saver_file_->open(QIODevice::ReadOnly);
    QDataStream out(saver_file_);
    qint64 tmp_num;
    out>>tmp_num;
    saver_file_->close();
    qDebug()<<" FtpPlugin::getUnfinesedTaskPos():��һ����ɵ���"<<tmp_num;
    return tmp_num;
}

void FtpPlugin::getListInfo(const QUrlInfo &i)
{
    qDebug()<<"FtpPlugin::getListInfo():List�ļ���"<<i.name();

    emit listInfo(QString::fromUtf8(i.name().toLatin1()),i.isFile());
}

void FtpPlugin::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    //TODO:���¼�ļ������ǰ�����ر��������Ա�ʹ�öϵ�����
    if(flag_cont_trans_)
    {
        readBytes += base_readbytes_;
    }
    if(!saver_file_->isOpen())
    {
        if(!saver_file_->open(QIODevice::WriteOnly))
        {
            FILE_OPEN_ERROR(*saver_file_);
            //   handleFatalError("�޷����ļ���"+saver_file_->fileName());
            return;
        }
    }
    saver_file_->reset();
    QDataStream in(saver_file_);
    in<<readBytes;
    emit dataTransferProgress(QString::number(readBytes),QString::number(totalBytes));
}

void FtpPlugin::debug() const
{
    qDebug()<<"this is a ftp plugin for CUC_FrameWork.";
}
QString FtpPlugin::pluginName() const
{
    return "Ftp helper";
}
QString FtpPlugin::pluginVersion() const
{
    return "0.1";
}
QString FtpPlugin::pluginIcoUrl() const
{
    return "ftp.png";
}
void FtpPlugin::slot_stateChanged(int state)
{
    qDebug()<<"slot_stateChanged():ftp state changed :"<<state;
    ftp_state_ = state;
}

int FtpPlugin::connectToHost(const QString &host, QString port)
{

    qDebug() << " FtpPlugin::connectToHost() :ftp_state_ = "<<ftp_state_;
    if(ftp_state_==QFtp::Unconnected )
    {
        ftp->setTransferMode(QFtp::Active);

        int state = ftp->connectToHost(host,( unsigned short)(port.toInt()));
        qDebug()<<"FtpPlugin::connectToHost():�������ӵ� "+host+",�˿ںţ�"<<(unsigned short)(port.toInt());
        return state;
    }
}
int FtpPlugin::login(const QString &user, const QString &password)
{
    qDebug() << " FtpPlugin::login() :ftp_state_ = "<<ftp_state_;
    if(ftp_state_!=QFtp::LoggedIn)
    {

        int state = ftp->login(user,password);
        qDebug()<<"FtpPlugin::login():�û� "<<user<<"���ڵ�½...";
        return state;
    }
}
int FtpPlugin::list(const QString &dir)
{
    qDebug()<<"FtpPlugin::list():�����г���ǰĿ¼...";
    return ftp->list(dir);
}
int FtpPlugin::get(const QString &srcfileName,const QString &fileName, const QString &info)
{

    file_ = new QFile(fileName);
    if(info == "")
    {
        if (!file_->open(QIODevice::WriteOnly))
        {
            qDebug()<<"FtpPlugin::get():�޷������ļ� "<<fileName;
            delete file_;
            return -1;
        }
    }
    else
    {
        bool ok;
        qint64 tmp_dec=info.toLongLong(&ok,10);
        if(ok)
        {
            if (!file_->open(QIODevice::ReadWrite))
            {
                qDebug()<<"FtpPlugin::get():�޷������ļ� "<<fileName;
                delete file_;
                return -1;
            }
            bool tmp_bool=file_->seek(tmp_dec);
            qDebug()<<"FtpPlugin::get():file_->seek()�ķ���ֵΪ��"<<tmp_bool;
        }
        else
        {
            if (!file_->open(QIODevice::WriteOnly))
            {
                qDebug()<<"FtpPlugin::get():�޷������ļ� "<<fileName;
                delete file_;
                return -1;
            }
        }
    }
    qDebug()<<"FtpPlugin::get()::�������� "<<srcfileName<<" ��Ŀ¼ "<<fileName;
    int state= ftp->get(QString::fromLatin1((srcfileName).toUtf8()), file_);
    return state;

}
int FtpPlugin::put(const QString &choosed_files_dir_,const QString file_name, const QString &info)
{
   choosed_file_=new QFile(choosed_files_dir_);
    uploadfilename_=choosed_file_->fileName();
    qDebug()<<"FtpPlugin::put():���ڴ���Ҫ�ϴ����ļ�...";

    if(!choosed_file_->open(QIODevice::ReadOnly))
    {
        qDebug()<<"FtpPlugin::put()::�޷��ϴ��ļ�:"<<choosed_files_dir_;
        choosed_file_->close();
        return -1;
    }

    if(info != "")
    {
        bool ok;
        qint64 tmp_dec=info.toLongLong(&ok,10);
        if(ok)
        {

            bool tmp_bool=choosed_file_->seek(tmp_dec);
            qDebug()<<"FtpPlugin::get():file_->seek()�ķ���ֵΪ��"<<tmp_bool;
        }

    }
    qDebug()<<"FtpPlugin::put()::�����ϴ�...";
    int state=ftp->put(choosed_file_,QString::fromLatin1(file_name.toUtf8()));
    return state;

}
void FtpPlugin::addGetTask(const QString &info,const QString &url, const QString &des_url)
{
    qDebug()<<"FtpPlugin::addGetTask():called";

    //���list��ǰΪ�գ���ô������ʼ����
    if(manager_->isQueueEmpty())
    {
        this->get(url,des_url,info);
    }
    manager_->addGetTask(info,url,des_url);
}
void FtpPlugin::addPutTask(const QString &data_url,
                           const QMap<QString,QVariant> &datas,
                           const QString &url,
                           const QString &file_name, const QString &info)
{
    // qDebug()<<"FtpPlugin::addPutTask():called";

    if(manager_->isQueueEmpty())
    {
        this->put(url,file_name,info);
    }

    //   qDebug()<<"FtpPlugin::addPutTask():datas size:"<<datas.size();
    //���������������������Ϣ(info��ʱ���հɣ�Ҳ�ò���)
    manager_->addPutTask(info,data_url,datas,url,file_name);
}
bool FtpPlugin::isQueueEmpty()
{
    bool tmp=manager_->isQueueEmpty();

    //  qDebug()<<"FtpPlugin::isQueueEmpty()��"<<tmp;
    return tmp;
}
void FtpPlugin::deleteTask(int i)
{
    manager_->deleteTask(i);
}
/*FtpPlugin::put(const QString &choosed_files_dir_):����*/
int FtpPlugin::put(const QString &choosed_files_dir_)
{
    QFile *remoteFileName=new QFile(choosed_files_dir_);
    uploadfilename_=remoteFileName->fileName();
    qDebug()<<"FtpPlugin::put():���ڴ���Ҫ�ϴ����ļ�...";
    qDebug()<< remoteFileName->open(QIODevice::ReadOnly);
    QString fileName = QFileInfo(choosed_files_dir_).fileName();
    qDebug()<<"FtpPlugin::put()::�����ϴ�...";
    int state=ftp->put(remoteFileName,QString::fromLatin1(fileName.toUtf8()));

    remoteFileName->close();
    qDebug()<<"ftp.put ���У�"+QString::number(state);

    return state;

}
int FtpPlugin::cd(const QString &dir)
{
    qDebug()<<"FtpPlugin::cd()::���ڸı�Ŀ¼...";

    return ftp->cd( QString::fromLatin1(dir.toUtf8()));
}
int FtpPlugin::close()
{
    qDebug()<<"FtpPlugin::close()::���ڹر�����...";
    return ftp->close();
}
void FtpPlugin::ftpCommandFinished(int, bool error)
{
    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QMessageBox::information(NULL, tr("FTP ģ��"),
                                     "�޷����ӵ�FTP������ ");

            return;
        }
        qDebug()<<"FtpPlugin::ftpCommandFinished():������";



        return;
    }

    if (ftp->currentCommand() == QFtp::Login){
        if (error) {
            QMessageBox::information(NULL, tr("FTP ģ��"),
                                     "�޷���¼��FTP������ ");

            return;
        }
        qDebug()<<"FtpPlugin::ftpCommandFinished():�ѵ�¼";
        ftp->list();


    }
    if (ftp->currentCommand() == QFtp::Get)
    {
        if (error) {
            qDebug()<<("FtpPlugin::ftpCommandFinished():�޷����� "+(file_->fileName()));
            QMessageBox::information(NULL, tr("FTP ģ��"),
                                     "�޷����� "+(file_->fileName()) );
            ftp->close();
            file_->close();
            file_->remove();
        } else {
            qDebug()<<"FtpPlugin::ftpCommandFinished():"<<file_->fileName()<<" ���������";
            file_->close();
            flag_cont_trans_ = false;
            //��ȡ���ϴεĸ�����Ϣ��info����Ȼ���ͳ�ȥ
            QString info=manager_->getTopTask().info;

            emit getFinished(info);
            /*��ʼ��һ������������еĻ���*/
            emit signal_startNextTask();
        }





    }
    else if (ftp->currentCommand() == QFtp::List)
    {
        emit listFinished();
    }
    else if(ftp->currentCommand()==QFtp::Put)
    {

        if (error) {
            qDebug()<<"FtpPlugin::ftpCommandFinished():�ϴ�����";
            QMessageBox::information(NULL, tr("FTP ģ��"),
                                     "�޷��ϴ� "+uploadfilename_ );
            ftp->close();

        } else {
            choosed_file_->close();
            delete choosed_file_;
            choosed_file_ = NULL;
            flag_cont_trans_ = false;
            //��ȡ���ϴεĸ�����Ϣ��info����Ȼ���ͳ�ȥ
            QString info=manager_->getTopTask().info;
            emit putFinished(info);
            /*��ʼ��һ������������еĻ���*/

            emit signal_startNextTask();

        }


    }
    else if(ftp->currentCommand()==QFtp::Mkdir){
        if(error){
            qDebug()<<("FtpPlugin::ftpCommandFinished():Mkdir����");
            emit mkdirfinished(false);
            ftp->close();

        }
        else {

            emit mkdirfinished(true);

        }
    }
    else if(ftp->currentCommand()==QFtp::None)
    {

    }
}
void FtpPlugin::slot_startNextTask()
{
    qDebug()<<"FtpPlugin::slot_startNextTask()��called";
    if(!manager_->isQueueEmpty()){
        FtpTask tmp=manager_->getTopTask();
        qDebug()<<"FtpPlugin::slot_startNextTask():data url: "<<tmp.post_data_url_;
        qDebug()<<"FtpPlugin::slot_startNextTask():data size "<<tmp.post_datas_.size();
        if(tmp.type==1)
        {
            //�����put�͵Ļ�
            this->postFtpData(tmp.post_data_url_,tmp.post_datas_);
        }
        else if(tmp.type==0)
        {
            //�����get�ͣ���ֱ�ӿ�ʼ�´�����
            this->startNextTask();
        }
    }
    else
    {
        qDebug()<<"FtpPlugin::slot_startNextTask():�������Ϊ��";
    }
}

void FtpPlugin::startNextTask()
{
    qDebug()<<" FtpPlugin::startNextTask()��called";

    //Ȼ�������һ�ε�����
    manager_->finishTask();
    //��saver��0

    if(!saver_file_->isOpen())
    {
        if(!saver_file_->open(QIODevice::WriteOnly))
        {
            FILE_OPEN_ERROR(*saver_file_);
            //   handleFatalError("�޷����ļ���"+saver_file_->fileName());
            return;
        }
    }
    qDebug()<<saver_file_->reset();
    QDataStream in(saver_file_);
    in<<qint64(0);
    saver_file_->close();

    if(!manager_->isQueueEmpty())
    {
        FtpTask tmp=manager_->getTopTask();

        if(tmp.type==0)
        {
            //��ʼget����
            get(tmp.url,tmp.des_url,tmp.info);
        }
        else if(tmp.type==1)
        {
            //��ʼput����

            put(tmp.url,tmp.filename,tmp.info);
        }
    }
    else
    {

        qDebug()<<"FtpPlugin::startNextTask()��������п�";
    }

}
void FtpPlugin::slot_ftpDataFinished(const QString &msg)
{

    qDebug()<<"FtpPlugin::slot_ftpDataFinished :called";

    /*post����ˣ����Կ�ʼ�´�����:)*/
    this->startNextTask();
    /*����Ĳ���Ӧ�ý���javascript��ȥ������������ֻ�Ǽ򵥵�
      ת����FtpDataHelper����źš���javascript������ȥ��׽�����
      �źţ�signal_PostDataFinished����
    */
    emit signal_PostDataFinished(msg);
}

int FtpPlugin::mkdir(const QString &foldername)
{
    qDebug()<<"FtpPlugin::mkdir()��mkdir:"+foldername;
    return ftp->mkdir(foldername);
}
#ifndef CUC_TEST
Q_EXPORT_PLUGIN2(ftp, FtpPlugin)
#endif
