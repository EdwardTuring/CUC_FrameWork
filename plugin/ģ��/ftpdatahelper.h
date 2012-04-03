#ifndef FTPDATAHELPER_H
#define FTPDATAHELPER_H

#include <QtCore>
#include "../CUCplugininterface.h"
namespace Plugin
{
    class FtpDataHelper;
    class CUCPluginInterface;

}
class FtpDataHelper:public QObject,public CUCPluginInterface
{
    Q_OBJECT
 #ifndef CUC_TEST
    Q_INTERFACES(CUCPluginInterface)
#endif
public:
     FtpDataHelper(QObject *parent=0);
public slots:

    virtual void debug() const;
    virtual QString pluginName() const;
    virtual QString pluginVersion() const;
    virtual QString pluginIcoUrl() const;

private slots:

private:

};


#endif // FTPDATAHELPER_H
