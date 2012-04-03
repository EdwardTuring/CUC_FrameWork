#ifdef CUC_TEST
#ifndef TST_FTPDATAHELPER_H
#define TST_FTPDATAHELPER_H

#include <QObject>
#include "ftpplugin.h"
class Tst_FtpDataHelper : public QObject
{
    Q_OBJECT
public:
    explicit Tst_FtpDataHelper(FtpPlugin *ftpplugin,QObject *parent = 0);
    
signals:
    
public slots:
    void slot_getfinished(const QString &info);
};

#endif // TST_FTPDATAHELPER_H
#endif
