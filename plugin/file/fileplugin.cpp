#include "fileplugin.h"
#include <QFileDialog>
FilePlugin::FilePlugin(QObject *parent)
{

}

void FilePlugin::debug() const
{
    qDebug()<<"this is a file plugin for CUC_FrameWork.";
}
QString FilePlugin::pluginName() const
{
    return "file object";
}
QString FilePlugin::pluginVersion() const
{
    return "0.1";
}
QString FilePlugin::pluginIcoUrl() const
{
    return "file.png";
}
QString FilePlugin::chooseFile()
{
    QString path=QFileDialog::getOpenFileName(NULL,tr("打开需要上传的文件"),
                                              QDir::homePath(),
                                              tr("Multimedia files(*)"));
      return path;

}
bool FilePlugin::exists(const QString &filename)
{
    return QFile::exists(filename);
}

Q_EXPORT_PLUGIN2(file, FilePlugin)
