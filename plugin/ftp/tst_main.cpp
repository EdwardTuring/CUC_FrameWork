#ifdef CUC_TEST

#include "ftpplugin.h"
#include "tst_ftpplugin.h"
#include "tst_ftpdatahelper.h"
#include <QApplication>
#include <iostream>
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
    FtpPlugin ftp;
    Tst_FtpDataHelper tst_ftpdatahelper(&helper);
    QMap<QString,QVariant> obj;
    QList<QVariant> tags;
    QVariant var("啊啊");
    tags.append(var);

    obj.insert("tags",QVariant(tags));
    obj.insert("dir",QVariant("1"));
      obj.insert("filename",QVariant("啊啊"));
        obj.insert("filedescription",QVariant("啊啊"));
          obj.insert("file_start_put_time",QVariant("1243"));
          obj.insert("uid","1");

          QMap<QString,QVariant> obj2;
          QList<QVariant> tags2;
          QVariant var2("啊啊");
          tags2.append(var2);

          obj2.insert("tags",QVariant(tags2));
          obj2.insert("dir",QVariant("2"));
            obj2.insert("filename",QVariant("啊啊2"));
              obj2.insert("filedescription",QVariant("啊啊2"));
                obj2.insert("file_start_put_time",QVariant("12432"));
                obj2.insert("uid","1");


          ftp.connectToHost("127.0.0.1","58021");
          ftp.login("mx","11");
          std::cout<<"obj size:"<<obj.size()<<std::endl;
          ftp.addPutTask("http://localhost/ftp_client/index.php/welcome/addtag",obj,
                         "F:/temp/新闻发布系统视频/qw.mp4","hhh.mp4");
          ftp.addPutTask("http://localhost/ftp_client/index.php/welcome/addtag",obj2,
                         "F:/temp/新闻发布系统视频/rtfg.mp4","rtfg.mp4");
    //      ftp.addPutTask("http://localhost/ftp_client/index.php/welcome/addtag",obj2,
      //                   "F:/cn_office_professional_plus_2007_dvd_with_sp2_updated_to_2011-04-30.iso","cn_office_professional_plus_2007_dvd_with_sp2_updated_to_2011-04-30.flv");
    return app.exec();
}

#endif
