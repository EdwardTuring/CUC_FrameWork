#include "ftpdatahelper.h"
FtpDataHelper::FtpDataHelper(QObject *parent):QObject(parent)
{

}
void FtpDataHelper::debug()
{
    qDebug()<<"FtpDataHelper::debug()£ºPlugins FtpDataHelper works";
}
QString FtpDataHelper::pluginIcoUrl()
{
    //nothing
}
QString FtpDataHelper::pluginName()
{
    return "CUC_FrameWork Plugins :FtpDataHelper ";
}
QString FtpDataHelper::pluginVersion()
{
    return "v0.1";
}
