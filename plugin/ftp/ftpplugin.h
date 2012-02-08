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


     qint64	bytesAvailable () const;
     int	cd ( const QString & dir );
     void	clearPendingCommands ();
     int	close ();
     int	connectToHost ( const QString & host, QString  port = "21" );
     Command	currentCommand () const;
     QIODevice *	currentDevice () const;
     int	currentId () const;
     Error	error () const;
     QString	errorString () const;
     int	get ( const QString & file, QIODevice * dev = 0, TransferType type = Binary );
     bool	hasPendingCommands () const;
     int	list ( const QString & dir = QString() );
     int	login ( const QString & user = QString(), const QString & password = QString() );
     int	mkdir ( const QString & dir );
     int	put ( QIODevice * dev, const QString & file, TransferType type = Binary );
     int	put ( const QByteArray & data, const QString & file, TransferType type = Binary );
     int	rawCommand ( const QString & command );
     qint64	read ( char * data, qint64 maxlen );
     QByteArray	readAll ();
     int	remove ( const QString & file );
     int	rename ( const QString & oldname, const QString & newname );
     int	rmdir ( const QString & dir );
     int	setProxy ( const QString & host, quint16 port );
     int	setTransferMode ( TransferMode mode );
     State	state () const;
private:
     QFtp *ftp;
};


#endif // FTPPLUGIN_H
