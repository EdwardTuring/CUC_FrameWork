#ifdef CUC_TEST

#include "ftpplugin.h"
#include "tst_ftpplugin.h"
#include <QApplication>
void setCodec();



void setCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}
int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    setCodec();
    FtpPlugin ftp;
    Tst_FtpPlugin tst_ftp(&ftp);

    ftp.connectToHost("127.0.0.1","58021");
    ftp.login("mx","11");
    ftp.list();



    ftp.addGetTask("hello,world", "news2.wmv","f:/news2.wmv");
    return app.exec();
}

#endif
