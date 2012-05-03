#include "updater.h"
#include "configparser.h"
#include "qt4.h"
#include <QFile>
#include "quazipfile.h"
#include "JlCompress.h"
using namespace CUCTool;
Updater::Updater(const QString &host,
                 const QString &port,
                 const QString user_name,
                 const QString pwd,
                 const QString &patch_file_name,
                 QObject *parent) :
    QObject(parent),
    patch_file_name_(patch_file_name),
    host_ip_(host),
    port_(port.toInt()),
    user_name_(user_name),
    pwd_(pwd)
{
    ftp_ = NULL;
    QDir tmp_patch_dir(QApplication::applicationDirPath());
    if(!tmp_patch_dir.exists("patch"))
    {
        tmp_patch_dir.mkdir("patch");
    }
    patch_dir_ = QApplication::applicationDirPath() + "/patch/";

    patch_file_ = NULL;
    download_pos_file_=NULL;
    download_pos_ =0;

}

bool Updater::patchFileHasDownloaded()
{
    QFileInfo tmp_patch_file_info(patch_dir_+patch_file_name_);
    QFileInfo tmp_download_pos_file_info(patch_dir_ + ".download_pos");
    if(tmp_patch_file_info.exists() && !tmp_download_pos_file_info.exists())
    {

            //补丁文件存在而状态文件不存在，那说明补丁文件已经下载完成或者是状态文件被误删，且没有完成下载。
            //如果是前一种情况，则跳过补丁文件的下载部分，直接安装补丁：
        patch_file_ = new QFile(patch_dir_+patch_file_name_,this);
         emit updateStateChanged(Updater::DownloadPatchFinished);
        installPatch();
            return true;
            //若果是后一种情况，那么在安装补丁的过程中会失败，安装程序会自动删除补丁文件。所以
            //下一次的补丁下载可以顺利的进行。

    }
    return false;
}

void Updater::connectToPatchServer()
{
 //   readHostIpFromSettingsFile();
//    network_manager_ = new QNetworkAccessManager(this);
//    QNetworkRequest request(QUrl(patch_server_url_));
//    request.setRawHeader("User-Agent", "CUC_FrameWork 1.4");
//    download_patch_reply_ =network_manager_->get(request);
//    CONNECT(download_patch_reply_,finished(*,
//            this,downloadPatchFinished());
    ftp_ = new QFtp(this);
    connect(ftp_,SIGNAL(commandFinished(int,bool)),
            this,SLOT(ftpCommandFinished(int,bool)));
    connect(ftp_,SIGNAL(dataTransferProgress(qint64,qint64)),
            this,SLOT(downloadProgress(qint64,qint64)));
    connect(ftp_,SIGNAL(stateChanged(int)),
            this,SLOT(ftpStateChanged(int)));
    ftp_ ->connectToHost(host_ip_,port_);

    ftp_->login(user_name_,pwd_);

}
void Updater::ftpStateChanged(int state)
{
    switch(state)
    {
        case QFtp::Connecting:
        qDebug()<<"Updater::ftpStateChanged():正在连接到ftp服务器";
            emit updateStateChanged(Updater::ConnectingToServer);
            break;
        case QFtp::LoggedIn:
          qDebug()<<"Updater::ftpStateChanged():已连接到ftp服务器";
             emit updateStateChanged(Updater::ConnectedToServer);
             break;
        default:break;
    }
}
void Updater::downloadPatch()
{
    if(!patchFileHasDownloaded())
        connectToPatchServer();

}

void Updater::downloadProgress(qint64 readbytes, qint64 allbytes)
{
    readbytes += download_pos_;

    if(!download_pos_file_->isOpen())
    {
        if(!download_pos_file_->open(QFile::WriteOnly))
        {
            emit fateError("下载状态文件打开失败");
            return ;
        }
    }
    QDataStream in(download_pos_file_);
    download_pos_file_->reset();
    in<<readbytes;
    //TODO:将这个消息抛给gui
    int tmp_num = (readbytes/allbytes)*100; //以100为标准
    emit downloading(tmp_num);

}

void Updater::downloadPatchPrivate()
{



    patch_file_ =new QFile(patch_dir_+patch_file_name_,this);
    download_pos_file_ = new QFile(patch_dir_ + ".download_pos",this);
    if(patch_file_ ->exists())
    {
        //断点续传

        if(download_pos_file_->exists())
        {
            if(!download_pos_file_->open(QFile::ReadOnly))
            {
                emit fateError("下载状态文件打开失败");
                return ;
            }
            QDataStream out(download_pos_file_);
            out>>download_pos_;
            download_pos_file_->close();
            if(!patch_file_->open(QFile::ReadWrite))
            {
                emit fateError("补丁文件打开失败");
                return ;
            }
            patch_file_ ->seek(download_pos_);
            ftpRest(download_pos_);
        }

    }
    else
    {
        if(!patch_file_->open(QFile::WriteOnly))
        {
            emit fateError("补丁文件打开失败");
            return ;
        }
    }
     qDebug()<<"Updater::downloadPatch():开始下载补丁文件："<<patch_file_name_;
     emit updateStateChanged(Updater::DownloadingPatch);
    ftp_->get(QString::fromLatin1((patch_file_name_).toUtf8()),patch_file_);
}
void Updater::ftpRest(qint64 pos)
{
    qDebug()<<"Updater::ftpRest():REST "<<pos;
    ftp_->rawCommand("REST "+QString::number(pos));
}
void Updater::installPatch()
{
    emit updateStateChanged(Updater::InstallingPatch);
    QString extr_dir;
    QStringList  tmp_extr_files=extractPatchFile(extr_dir);
    parsePatchList(tmp_extr_files,extr_dir);
    //删除相关的文件和文件夹
    emit updateStateChanged(Updater::CleaningPatchFiles);
    QFileInfo tmp_patch_file_info(*patch_file_);
    QString tmp_patch_file_basename=tmp_patch_file_info.baseName();
    deletePatchDir(patch_dir_+"/"+tmp_patch_file_basename);
    patch_file_->remove();
     emit updateStateChanged(Updater::UpdateFinished);

}

QStringList Updater::extractPatchFile(QString &extr_dir)
{
    QFileInfo tmp_patch_file_info(*patch_file_);
    QString tmp_patch_file_basename=tmp_patch_file_info.baseName();
    extr_dir = patch_dir_+ tmp_patch_file_basename;
    QStringList  tmp_extr_files =JlCompress::extractDir(patch_dir_+patch_file_name_, extr_dir);
    qDebug()<<"Updater::ftpCommandFinished():补丁文件解压状况：文件数量："<<tmp_extr_files.size();
    if(tmp_extr_files.size() == 0)
    {
        emit errorOccurred(Updater::ExtractPatchFileFailed);
        //然后删除补丁文件：
        patch_file_->remove();

    }
    return tmp_extr_files;

}
void Updater::parsePatchList(const QStringList &extr_files,const QString &extr_dir)
{
    //TODO:解析压缩文件中的补丁文件清单

    if(!extr_files.contains(extr_dir + "/.list"))
    {
        qDebug()<<"Updater::parsePatchList():没有找到.list文件，请确认在提交补丁的过程中加入了.list文件";
        emit errorOccurred(Updater::BadPatchFile);
         patch_file_->remove();
        return;
    }
    else
    {
         QSettings tmp_patch_list(extr_dir+"/.list",QSettings::IniFormat);
         tmp_patch_list.setIniCodec(QTextCodec::codecForName("System"));
         for(int i=0;i<extr_files.size();i++)
         {
             QString tmp_extr_file_dir=extr_files.at(i);
             QFile tmp_extr_file(tmp_extr_file_dir);
             if(!tmp_extr_file.exists())
             {
                     qDebug()<<"Updater::parsePatchList():没有找到指定的补丁文件："<<tmp_extr_file_dir;
                     emit errorOccurred(Updater::BadPatchFile);
                     return;
             }
             QFileInfo tmp_extr_file_info(tmp_extr_file);
             QString tmp_extr_file_name = tmp_extr_file_info.fileName();
          //   QString tmp_extr_file_basename = tmp_extr_file_info.baseName();
             if(tmp_extr_file_name != ".list")//避免“安装”.list文件
             {
                 QString tmp_patch_dir = tmp_patch_list.value("patch/"+tmp_extr_file_name).toString();
                 QString tmp_install_dir(QApplication::applicationDirPath() + tmp_patch_dir);
                 QFileInfo tmp_old_path_file_info(tmp_install_dir+ tmp_extr_file_name+".cpch");
                 if(!tmp_old_path_file_info.exists())
                 {
                     tmp_extr_file.copy( tmp_install_dir+ tmp_extr_file_name+".cpch");

                 }

                 qDebug()<<"Updater::parsePatchList():补丁文件 "<<tmp_extr_file_name+".cpch" <<"已复制到指定目录";
                 QFile tmp_old_file(tmp_install_dir + tmp_extr_file_name);
                 QFile tmp_new_file(tmp_install_dir+tmp_extr_file_name+".cpch");
                 if(tmp_old_file.exists())
                     tmp_old_file.remove();
                 if(tmp_new_file.rename(tmp_install_dir + tmp_extr_file_name))
                     qDebug()<<"Updater::parsePatchList():补丁文件 "<<tmp_extr_file_name<<"已成功安装";

             }
         }

    }
}

void Updater::ftpCommandFinished(int, bool error)
{
    if (ftp_->currentCommand() == QFtp::ConnectToHost) {
            return;
    }
    if (ftp_->currentCommand() == QFtp::Login){
        if (error) {
             qDebug()<<("Updater::ftpCommandFinished():无法完成ftp服务器的登录");
             emit errorOccurred(Updater::LoginInFtpServerFailed);
            return;
        }
        //登录成功，开始下载补丁
        downloadPatchPrivate();
    }
    if (ftp_->currentCommand() == QFtp::Get)
    {
        if (error)
        {
            qDebug()<<("Updater::ftpCommandFinished():无法完成补丁的下载");
             emit errorOccurred(Updater::DownloadPatchFileFailed);
            return;
        }
        else
        {
              qDebug()<<"Updater::ftpCommandFinished():"<<patch_file_name_<<"补丁下载完成";
            download_pos_file_->remove();
             download_pos_file_->deleteLater();
            patch_file_->close();
            emit updateStateChanged(Updater::DownloadPatchFinished);
            installPatch();
        }
    }
}
bool Updater::deletePatchDir(const QString &dirName)
{
    QDir directory(dirName);

    if (!directory.exists())
    {
        return true;
    }

    QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);


    QList<QString>::iterator f = files.begin();

    bool error = false;

    for (; f != files.end(); ++f)
    {
        QString filePath = QDir::convertSeparators(directory.path() + '/' + (*f));
        QFileInfo fi(filePath);
        if (fi.isFile() || fi.isSymLink())
        {
            QFile::setPermissions(filePath, QFile::WriteOwner);
            if (!QFile::remove(filePath))
            {

                error = true;
            }
        }
        else if (fi.isDir())
        {
            if (!deletePatchDir(filePath));
            {
                error = true;
            }
        }
    }

    if(!directory.rmdir(QDir::convertSeparators(directory.path())))
    {

        error = true;
    }

    return !error;

}
