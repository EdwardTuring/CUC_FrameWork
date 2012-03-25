#ifndef TST_NETWORKMANAGER_H
#define TST_NETWORKMANAGER_H

#include "qt4.h"
#include "core/networkmanager.h"
class Tst_NetWorkManager : public QObject
{
    Q_OBJECT
public:
    explicit Tst_NetWorkManager(QObject *parent = 0);
    
signals:
    
public slots:
private slots:
    void testFaild(QString msg);
private:
    NetWorkManager *manager_;
};

#endif // TST_NETWORKMANAGER_H
