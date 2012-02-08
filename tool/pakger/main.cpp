#include <QtCore>
#include "quazip/quazipfile.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QuaZipFile zip();

    return a.exec();
}
