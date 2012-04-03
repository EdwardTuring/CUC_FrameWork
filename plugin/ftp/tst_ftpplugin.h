#ifdef CUC_TEST
#ifndef TST_FTPPLUGIN_H
#define TST_FTPPLUGIN_H

#include <QObject>
#include "ftpplugin.h"
class Tst_FtpPlugin : public QObject
{
    Q_OBJECT
public:
    explicit Tst_FtpPlugin(FtpPlugin *ftpplugin,QObject *parent = 0);
    
signals:
    
public slots:
    void slot_getfinished(const QString &info);
};

#endif // TST_FTPPLUGIN_H
#endif
