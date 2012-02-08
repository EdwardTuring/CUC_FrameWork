#include "ftpplugin.h"
FtpPlugin::FtpPlugin(QObject *parent)
{


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


Q_EXPORT_PLUGIN2(ftp, FtpPlugin)
