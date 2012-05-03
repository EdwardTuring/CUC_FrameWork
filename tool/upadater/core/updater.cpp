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
    patch_dir_ = QApplication::applicationDirPath() + "/patch/";

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
        qDebug()<<"Updater::ftpStateChanged():�������ӵ�ftp������";
            emit updateStateChanged(Updater::ConnectingToServer);
            break;
        case QFtp::LoggedIn:
          qDebug()<<"Updater::ftpStateChanged():�����ӵ�ftp������";
             emit updateStateChanged(Updater::ConnectedToServer);
             break;
        default:break;
    }
}

void Updater::downloadProgress(qint64 readbytes, qint64 allbytes)
{
    readbytes += download_pos_;
    if(!download_pos_file_->isOpen())
    {
        if(!download_pos_file_->open(QFile::WriteOnly))
        {
            emit fateError("����״̬�ļ���ʧ��");
            return ;
        }
    }
    QDataStream in(download_pos_file_);
    download_pos_file_->reset();
    in<<readbytes;
    //TODO:�������Ϣ�׸�gui
    qDebug()<<readbytes;
    emit downloading(readbytes,allbytes);

}

//void Updater::readHostIpFromSettingsFile()
//{
//    QString tmp_url = CUCCore::ConfigParser::getSettingsHostUrl();
//    if (tmp_url == "")
//    {
//        emit fateError("������ַʧ��");
//        return ;
//    }
//    else
//    {
//        patch_server_url_ = tmp_url + "/download/patch/"+patch_file_name_;

//    }
//}
void Updater::downloadPatch()
{

    qDebug()<<"Updater::downloadPatch():��ʼ���ز����ļ���"<<patch_file_name_;

    patch_file_ =new QFile(patch_dir_+patch_file_name_);
    download_pos_file_ = new QFile(patch_dir_ + ".download_pos");
    if(patch_file_ ->exists())
    {
        //�ϵ�����
        if(!patch_file_->open(QFile::ReadWrite))
        {
            emit fateError("�����ļ���ʧ��");
            return ;
        }
        if(download_pos_file_->exists())
        {
            if(!download_pos_file_->open(QFile::ReadOnly))
            {
                emit fateError("����״̬�ļ���ʧ��");
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
            emit fateError("�����ļ���ʧ��");
            return ;
        }
    }
    ftp_->get(QString::fromLatin1((patch_file_name_).toUtf8()),patch_file_);
}
void Updater::ftpRest(qint64 pos)
{
    qDebug()<<"Updater::ftpRest():REST "<<pos;
    ftp_->rawCommand("REST "+QString::number(pos));
}
void Updater::compressPatchFile()
{
    QFileInfo tmp_patch_file_info(*patch_file_);
    QString tmp_patch_file_basename=tmp_patch_file_info.baseName();
    QStringList  tmp_extr_files =JlCompress::extractDir(patch_dir_+patch_file_name_,
                                                     patch_dir_+ tmp_patch_file_basename);
    qDebug()<<"Updater::ftpCommandFinished():�����ļ���ѹ״�����ļ�������"<<tmp_extr_files.size();
    if(tmp_extr_files.size() == 0)
        emit errorOccurred(Updater::CompressPatchFileFailed);

}
void Updater::parsePatchList()
{
    //TODO:����ѹ���ļ��еĲ����ļ��嵥
}

void Updater::ftpCommandFinished(int, bool error)
{
    if (ftp_->currentCommand() == QFtp::ConnectToHost) {
            return;
    }
    if (ftp_->currentCommand() == QFtp::Login){
        if (error) {
             qDebug()<<("Updater::ftpCommandFinished():�޷����ftp�������ĵ�¼");
             emit errorOccurred(Updater::LoginInFtpServerFailed);
            return;
        }
        //��¼�ɹ�����ʼ���ز���
        downloadPatch();
    }
    if (ftp_->currentCommand() == QFtp::Get)
    {
        if (error)
        {
            qDebug()<<("Updater::ftpCommandFinished():�޷���ɲ���������");
             emit errorOccurred(Updater::DownloadPatchFileFailed);
            return;
        }
        else
        {
              qDebug()<<"Updater::ftpCommandFinished():"<<patch_file_name_<<"�����������";
            download_pos_file_->remove();
             download_pos_file_->deleteLater();
            patch_file_->close();
              //TODO:��������װ����
            compressPatchFile();
            parsePatchList();
        }
    }
}
