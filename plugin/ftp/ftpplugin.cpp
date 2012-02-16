#include "ftpplugin.h"
#include <QMessageBox>

#include <string.h>
FtpPlugin::FtpPlugin(QObject *parent)
{
    qDebug()<<"������ftpģ��.";
    ftp=new QFtp(this);

    connect(ftp, SIGNAL(commandFinished(int,bool)),
                this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)),
                this, SLOT(getListInfo(QUrlInfo)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
        this, SLOT(updateDataTransferProgress(qint64,qint64)));
}
void FtpPlugin::getListInfo(const QUrlInfo &i)
{
    qDebug()<<"getListInfo works";
    emit listInfo(QString::fromUtf8(i.name().toLatin1()),i.isFile());
}

void FtpPlugin::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
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

    qDebug()<<"connect:�������ӵ� "+host;
    return ftp->connectToHost(host,21);
}
int FtpPlugin::login(const QString &user, const QString &password)
{
    qDebug()<<"login:�û� "+user+"���ڵ�½...";
    return ftp->login(user,password);
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
     qDebug()<<"ftp.get ���ص�״̬��"+QString::number(state);

     return state;

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
    qDebug()<<"ftp.put ���ص�״̬��"+QString::number(state);

    return state;

}
int FtpPlugin::cd(const QString &dir)
{
    qDebug()<<"cd:���ڷ�����һ��Ŀ¼...";

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
            emit getFinished();

        }


        delete file_;


    }
    else if (ftp->currentCommand() == QFtp::List)
    {
        //������
    }
    else if(ftp->currentCommand()==QFtp::Put)
    {

        if (error) {
           qDebug()<<("�ϴ�����");
           QMessageBox::information(NULL, tr("FTP ģ��"),
                                    "�޷��ϴ� "+uploadfilename_ );
           ftp->close();

        } else {

           emit putfinished();

        }




    }
    else if(ftp->currentCommand()==QFtp::None)
    {

    }
}
int FtpPlugin::mkdir(const QString &foldername)
{
    qDebug()<<"mkdir:"+foldername;
    return ftp->mkdir(foldername);
}

Q_EXPORT_PLUGIN2(ftp, FtpPlugin)
