#ifdef CUC_TEST
#ifndef TST_FTPDATAHELPER_H
#define TST_FTPDATAHELPER_H

#include <QObject>
#include "ftpdatahelper.h"
class Tst_FtpDataHelper : public QObject
{
    Q_OBJECT
public:
    explicit Tst_FtpDataHelper(FtpDataHelper *helper,QObject *parent = 0);
    
signals:
    
public slots:
    void slot_tst_postFinished(const QString &);
private:
    FtpDataHelper *helper_;
};

#endif // TST_FTPDATAHELPER_H
#endif
