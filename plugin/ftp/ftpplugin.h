#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QtCore>
#include <QFtp>
#include "../CUCplugininterface.h"
namespace Plugin
{
    class FtpPlugin;
    class CUCPluginInterface;
}
class FtpPlugin:public QObject,public CUCPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CUCPluginInterface)
public:
     FtpPlugin(QObject *parent=0);
public slots:

    virtual void debug() const;
    virtual QString pluginName() const;
    virtual QString pluginVersion() const;
    virtual QString pluginIcoUrl() const;

     void ftpCommandFinished(int, bool error);


     int	cd ( const QString & dir );

     int	close ();
     int	connectToHost( const QString & host, QString  port = "21" );
     int mkdir(const QString & foldername);

     int	get ( const QString & srcfileName, const QString &fileName);
     int    put(const QString & choosed_files_dir_);
     int	list ( const QString & dir = QString() );
     int	login ( const QString & user = QString(), const QString & password = QString() );
protected slots:
     void updateDataTransferProgress(qint64 readBytes,qint64 totalBytes);
     void getListInfo(const QUrlInfo & i);
signals:
     void getFinished();
     void putfinished();
     void mkdirFinished();
     void listInfo( QString,bool);
     void dataTransferProgress(QString,QString);

private:
     QFtp *ftp;
     QFile *file_;
     QString uploadfilename_;
};


#endif // FTPPLUGIN_H
