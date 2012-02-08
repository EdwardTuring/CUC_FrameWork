#include <QtCore>
#include "quazip/quazipfile.h"
#include "quazip/JlCompress.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<JlCompress::compressDir("aa","ss");
    return a.exec();
}
