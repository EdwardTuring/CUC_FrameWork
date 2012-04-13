#include "ftpplugin.h"
#include <QMessageBox>

#include <string.h>
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

TaskManager::TaskManager(QObject *parent):QObject(parent)
{
    qDebug()<<"TaskManager::TaskManager():TaskManager created";
    this->list_=new QList<FtpTask>;
    //�����洢������е��ļ�����:
    QString tmp_path = qApp->applicationDirPath() + "/data/tasks.dat";
    task_store_file_=new QFile(tmp_path,this);
}
TaskManager::~TaskManager()
{
    delete list_;
}
void TaskManager::writeTaskInfoToFile(const FtpTask &task)
{
    if(!task_store_file_->open(QIODevice::Append))
    {
        FILE_OPEN_ERROR(task_store_file_);
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
    list_->append(task_tmp);

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
     qDebug()<<"TaskManager::addPutTask():datas size:"<<task_tmp.post_datas_.size();
    list_->append(task_tmp);

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

void TaskManager::deleteTask(int i)
{
     if(!list_->isEmpty())
        list_->removeAt(i);
}
void TaskManager::finishTask()
{
    if(!list_->isEmpty())
        list_->removeFirst();
}

FtpPlugin::FtpPlugin(QObject *parent):QObject(parent)
{
    qDebug()<<"������ftpģ��.";
    ftp=new QFtp(this);

    manager_=new TaskManager(this);
    data_helper_=new FtpDataHelper(this);

    /*����FtpDataHelper���źŲ�*/
    connect(data_helper_,SIGNAL(ftpDataFinished(const QString &)),
            this,SLOT(slot_ftpDataFinished(const QString &)));
    connect(ftp, SIGNAL(commandFinished(int,bool)),
                this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)),
                this, SLOT(getListInfo(QUrlInfo)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
        this, SLOT(updateDataTransferProgress(qint64,qint64)));

    connect(this,SIGNAL(signal_startNextTask()),this,SLOT(slot_startNextTask()));
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
void FtpPlugin::postFtpData(const QString &url, const QMap<QString, QVariant> &obj)
{
     qDebug()<<"FtpPlugin::postFtpData():called";
     QString tags_str;
     if(obj["tags"].type()==QVariant::List)
     {
         qDebug()<<"FtpPlugin::postFtpData():obj['tags'] is a array";
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

void FtpPlugin::getListInfo(const QUrlInfo &i)
{
    qDebug()<<"getListInfo works";

    emit listInfo(QString::fromUtf8(i.name().toLatin1()),i.isFile());
}

void FtpPlugin::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    //TODO:���¼�ļ������ǰ�����ر��������Ա�ʹ�öϵ�����

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
int FtpPlugin::connectToHost(const QString &host, QString port)
{

    qDebug()<<"connect:�������ӵ� "+host+",�˿ںţ�"<<(unsigned short)(port.toInt());
    if(ftp->state()==QFtp::Unconnected)
    {
        ftp->setTransferMode(QFtp::Active);
        return ftp->connectToHost(host,( unsigned short)(port.toInt()));
    }
}
int FtpPlugin::login(const QString &user, const QString &password)
{
    qDebug()<<"login:�û� "+user+"���ڵ�½...";
     if(ftp->state()!=QFtp::LoggedIn) return ftp->login(user,password);
}
int FtpPlugin::list(const QString &dir)
{
    qDebug()<<"list:�����г���ǰĿ¼...";
    return ftp->list(dir);
}
int FtpPlugin::get(const QString &srcfileName,const QString &fileName)
{

    file_ = new QFile(fileName);

    bool b=file_->open(QIODevice::WriteOnly);

    qDebug()<<b;
    if (!b) {
        qDebug()<<"�޷������ļ� "+fileName;
        delete file_;
        return -1;
    }
     qDebug()<<"get:�������� "+srcfileName+" ��Ŀ¼ "+fileName;
    int state= ftp->get(QString::fromLatin1((srcfileName).toUtf8()), file_);
     qDebug()<<"ftp.get ���У�"+QString::number(state);

     return state;

}
int FtpPlugin::put(const QString &choosed_files_dir_,const QString file_name)
{
    QFile *remoteFileName=new QFile(choosed_files_dir_);
    uploadfilename_=remoteFileName->fileName();
    qDebug()<<"���ڴ���Ҫ�ϴ����ļ�...";
    qDebug()<< remoteFileName->open(QIODevice::ReadOnly);

     qDebug()<<"put:�����ϴ�...";
    int state=ftp->put(remoteFileName,QString::fromLatin1(file_name.toUtf8()));

    remoteFileName->close();
    qDebug()<<"ftp.put ���У�"+QString::number(state);

    return state;

}
void FtpPlugin::addGetTask(const QString &info,const QString &url, const QString &des_url)
{
   qDebug()<<"FtpPlugin::addGetTask():called";

    //���list��ǰΪ�գ���ô������ʼ����
    if(manager_->isQueueEmpty())
    {
        this->get(url,des_url);
    }
    manager_->addGetTask(info,url,des_url);
}
void FtpPlugin::addPutTask(const QString &data_url,
                const QMap<QString,QVariant> &datas,
                const QString &url,
                const QString &file_name)
{
    qDebug()<<"FtpPlugin::addPutTask():called";

    if(manager_->isQueueEmpty())
    {
        this->put(url,file_name);
    }

    qDebug()<<"FtpPlugin::addPutTask():datas size:"<<datas.size();
    //���������������������Ϣ(info��ʱ���հɣ�Ҳ�ò���)
    manager_->addPutTask(" ",data_url,datas,url,file_name);
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

int FtpPlugin::put(const QString &choosed_files_dir_)
{
    QFile *remoteFileName=new QFile(choosed_files_dir_);
    uploadfilename_=remoteFileName->fileName();
    qDebug()<<"���ڴ���Ҫ�ϴ����ļ�...";
    qDebug()<< remoteFileName->open(QIODevice::ReadOnly);
    QString fileName = QFileInfo(choosed_files_dir_).fileName();
     qDebug()<<"put:�����ϴ�...";
    int state=ftp->put(remoteFileName,QString::fromLatin1(fileName.toUtf8()));

    remoteFileName->close();
    qDebug()<<"ftp.put ���У�"+QString::number(state);

    return state;

}
int FtpPlugin::cd(const QString &dir)
{
    qDebug()<<"cd:���ڸı�Ŀ¼...";

    return ftp->cd( QString::fromLatin1(dir.toUtf8()));
}
int FtpPlugin::close()
{
    qDebug()<<"close:���ڹر�����...";
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
        qDebug()<<"������";



        return;
    }

    if (ftp->currentCommand() == QFtp::Login){
        if (error) {
            QMessageBox::information(NULL, tr("FTP ģ��"),
                                     "�޷���¼��FTP������ ");

            return;
        }
        qDebug()<<"�ѵ�¼";
        ftp->list();


}
    if (ftp->currentCommand() == QFtp::Get)
    {
        if (error) {
           qDebug()<<("�޷����� "+(file_->fileName()));
           QMessageBox::information(NULL, tr("FTP ģ��"),
                                    "�޷����� "+(file_->fileName()) );
            ftp->close();
            file_->close();
            file_->remove();
        } else {
            qDebug()<<file_->fileName()+" ���������";
            file_->close();

            //��ȡ���ϴεĸ�����Ϣ��info����Ȼ���ͳ�ȥ
            QString info=manager_->getTopTask().info;
            emit getFinished(info);
            /*��ʼ��һ������������еĻ���*/
            emit signal_startNextTask();
        }


       // delete file_;


    }
    else if (ftp->currentCommand() == QFtp::List)
    {
        emit listFinished();
    }
    else if(ftp->currentCommand()==QFtp::Put)
    {

        if (error) {
           qDebug()<<("�ϴ�����");
           QMessageBox::information(NULL, tr("FTP ģ��"),
                                    "�޷��ϴ� "+uploadfilename_ );
           ftp->close();

        } else {

            //��ȡ���ϴεĸ�����Ϣ��info����Ȼ���ͳ�ȥ
            QString info=manager_->getTopTask().info;
            emit putFinished(info);
           /*��ʼ��һ������������еĻ���*/

           emit signal_startNextTask();

        }


    }
    else if(ftp->currentCommand()==QFtp::Mkdir){
        if(error){
            qDebug()<<("Mkdir:error");
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
    if(!manager_->isQueueEmpty())
    {
        FtpTask tmp=manager_->getTopTask();

        if(tmp.type==0)
        {
            //��ʼget����
            get(tmp.url,tmp.des_url);
        }
        else if(tmp.type==1)
        {
            //��ʼput����

            put(tmp.url,tmp.filename);
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
    qDebug()<<"mkdir:"+foldername;
    return ftp->mkdir(foldername);
}
#ifndef CUC_TEST
Q_EXPORT_PLUGIN2(ftp, FtpPlugin)
#endif
