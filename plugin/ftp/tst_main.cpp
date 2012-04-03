#ifdef CUC_TEST

#include "ftpplugin.h"
#include "tst_ftpplugin.h"
#include <QApplication>
#include "tst_ftpdatahelper.h"
#include "ftpdatahelper.h"
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
    FtpDataHelper helper;
   Tst_FtpDataHelper tst_helper(&helper);
    QNetworkAccessManager manager;
    helper.setNetWorkManager(&manager);
    helper.postFtpData("http://localhost/ftp_client/index.php/welcome/test",
                       " ‘ ‘","20120403","∫Ï”£Ã“.flv"," ‘ ‘","_20120403_20_16_30");
    return app.exec();
}

#endif
