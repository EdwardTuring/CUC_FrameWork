#ifdef CUC_TEST

#include "ftpplugin.h"
#include "tst_ftpplugin.h"
#include "tst_ftpdatahelper.h"
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
    FtpDataHelper helper;
    QNetworkAccessManager ma;
    helper.setNetWorkManager(&ma);
    Tst_FtpDataHelper tst_ftpdatahelper(&helper);
    QMap<QString,QVariant> obj;
    QList<QVariant> tags;
    QVariant var("Âíæç");
    tags.append(var);

    obj.insert("tags",QVariant(tags));
    obj.insert("dir",QVariant("12"));
      obj.insert("filename",QVariant("°¡°¡"));
        obj.insert("filedescription",QVariant("°¡°¡"));
          obj.insert("file_start_put_time",QVariant("1243"));
          obj.insert("uid","1");
    helper.postFtpData("http://localhost/ftp_client/index.php/welcome/addtag",
                       tags,
                       obj["dir"].toString(),
                       obj["filename"].toString(),
                       obj["filedescription"].toString(),
                       obj["file_start_put_time"].toString(),
                        obj["uid"].toString());
    return app.exec();
}

#endif
