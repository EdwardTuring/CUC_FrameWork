#include "updater.h"
#include "configparser.h"
#include "qt4.h"
#include <QFile>
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
    patch_file_ = NULL;
    download_pos_file_=NULL;
    download_pos_ =0;
    connectToPatchServer();
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
    ftp_ ->connectToHost(host_ip_,port_);
    ftp_->login(user_name_,pwd_);

}
void Updater::downloadProgress(qint64 readbytes, qint64 allbytes)
{
    readbytes += download_pos_;
    //TODO:将这个消息抛给gui
}

//void Updater::readHostIpFromSettingsFile()
//{
//    QString tmp_url = CUCCore::ConfigParser::getSettingsHostUrl();
//    if (tmp_url == "")
//    {
//        emit fateError("解析地址失败");
//        return ;
//    }
//    else
//    {
//        patch_server_url_ = tmp_url + "/download/patch/"+patch_file_name_;

//    }
//}
void Updater::downloadPatch()
{
    QString patch_dir = QApplication::applicationDirPath() + "/patch/";
    patch_file_ =new QFile(patch_dir+patch_file_name_);
    download_pos_file_ = new QFile(patch_dir + ".download_pos");
    if(patch_file_ ->exists())
    {
        //断点续传
        if(!patch_file_->open(QFile::ReadWrite))
        {
            emit fateError("补丁文件打开失败");
            return ;
        }
        if(download_pos_file_->exists())
        {
            if(!patch_file_->open(QFile::ReadOnly))
            {
                emit fateError("下载状态文件打开失败");
                return ;
            }
            QDataStream out(download_pos_file_);

            out>>download_pos_;
            download_pos_file_->close();
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
    ftp_->get(patch_file_name_,patch_file_);
}
void Updater::ftpRest(qint64 pos)
{
    qDebug()<<"FtpPlugin::rest():REST "<<pos;
    ftp_->rawCommand("REST "+QString::number(pos));
}

void Updater::ftpCommandFinished(int, bool error)
{
    if (ftp_->currentCommand() == QFtp::ConnectToHost) {
            return;
    }
    if (ftp_->currentCommand() == QFtp::Login){
        if (error) {
            QMessageBox::information(NULL, tr("FTP 模块"),
                                     "无法登录到FTP服务器 ");
            return;
        }
        //登录成功，开始下载补丁
        downloadPatch();
    }
    if (ftp_->currentCommand() == QFtp::Get)
    {
        if (error)
        {
            qDebug()<<("FtpPlugin::ftpCommandFinished():无法完成补丁的下载");
        }
        else
        {
            //TODO:处理补丁安装过程
        }
    }


}
