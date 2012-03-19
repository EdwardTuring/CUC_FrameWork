#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QtCore>

#include "../CUCplugininterface.h"
namespace Plugin
{
    class FilePlugin;
    class CUCPluginInterface;
}
class FilePlugin:public QObject,public CUCPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CUCPluginInterface)
public:
     FilePlugin(QObject *parent=0);
public slots:

    virtual void debug() const;
    virtual QString pluginName() const;
    virtual QString pluginVersion() const;
    virtual QString pluginIcoUrl() const;

     QString chooseFile();
     QString saveAs(const QString &filename);
     bool exists(const QString &filename);

signals:


private:

     QFile *file;
};


#endif // FTPPLUGIN_H
