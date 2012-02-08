#include "ftpplugin.h"
FtpPlugin::FtpPlugin(QObject *parent)
{
    ftp=new QFtp(this);

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
    ftp->connectToHost(host,atoi(port.ascii()));
}

Q_EXPORT_PLUGIN2(ftp, FtpPlugin)
