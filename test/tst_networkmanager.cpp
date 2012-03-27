#ifdef CUC_TEST
#include "tst_networkmanager.h"
#include "core/networkmanager.h"
#include <iostream>
Tst_NetWorkManager::Tst_NetWorkManager(QObject *parent) :
    QObject(parent)
{
    manager_=new CUCCore::NetWorkManager("http://bcont.cuc.edu.cn/customer/index.php/welcome/index/123",this);
    CONNECT(manager_,testFaild(QString),this,testFaild(QString));
    manager_->get();

}
void Tst_NetWorkManager::testFaild(QString msg)
{
    std::cerr<<"Tst_NetWorkManager faild:"<<qPrintable(msg)<<endl;
}
#endif
