#include <QtCore>
#include "zconf.h"
#include "zlib.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString aa("aa");
    QString b("ss");
    gzopen(aa.toStdString().c_str(),b.toStdString().c_str());
    return a.exec();
}
