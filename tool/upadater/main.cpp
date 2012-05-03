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
   #ifdef _CUC_TEST_
//    CUCTool::Updater updater("127.0.0.1","58021",
//                             "updater","mx",
//                             "mm_client_206.cucp");
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
        UpdateDialog d(host,port,user_name,pwd,patch_file_name);
        d.show();
        return app.exec();
    }

    #endif

}

void setCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}
