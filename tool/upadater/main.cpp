#include <QtCore>
#include <QtGui>
#include <QApplication>
#include "core/updater.h"
#include "ui/updatedialog.h"

void setCodec();
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    setCodec();
    app.setOrganizationName("CUC");
   app.setApplicationName("CUC_BCont Updater");
   app.setApplicationVersion("1.0");
    QTranslator translator;
    translator.load(":/qt_zh_CN.qm");
    app.installTranslator(&translator);
    if(argc != 0)
    {

        QString host(argv[1]);
        if(host == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数:主机地址";
            return -1;
        }
        QString port(argv[2]);
        if(port == "" )
        {
              qDebug()<<"CUC_BCont_updater:无效的参数：端口";
              return -1;
        }
        QString user_name(argv[3]);
        if(user_name == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数：用户名";
            return -1;
        }
        QString pwd(argv[4]);
        if(user_name == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数：密码";
            return -1;
        }
        QString patch_file_name(argv[5]);
        if(user_name == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数：补丁文件名";
            return -1;
        }
        QString main_pro_name(argv[6]);

        if(main_pro_name == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数：主进程名无效";
            return -1;
        }
        QString v(argv[7]);

        if(v == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数：版本号无效";
            return -1;
        }
          QString app_name(argv[8]);
        if(app_name == "")
        {
            qDebug()<<"CUC_BCont_updater:无效的参数：无效的应用程序名";
            return -1;
        }
        UpdateDialog d(host,port,user_name,pwd,patch_file_name,main_pro_name,v,app_name);
        d.show();
        return app.exec();
    }

}

void setCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}
