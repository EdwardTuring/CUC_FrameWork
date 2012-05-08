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
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ���:������ַ";
            return -1;
        }
        QString port(argv[2]);
        if(port == "" )
        {
              qDebug()<<"CUC_BCont_updater:��Ч�Ĳ������˿�";
              return -1;
        }
        QString user_name(argv[3]);
        if(user_name == "")
        {
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ������û���";
            return -1;
        }
        QString pwd(argv[4]);
        if(user_name == "")
        {
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ���������";
            return -1;
        }
        QString patch_file_name(argv[5]);
        if(user_name == "")
        {
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ����������ļ���";
            return -1;
        }
        QString main_pro_name(argv[6]);

        if(main_pro_name == "")
        {
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ���������������Ч";
            return -1;
        }
        QString v(argv[7]);

        if(v == "")
        {
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ������汾����Ч";
            return -1;
        }
          QString app_name(argv[8]);
        if(app_name == "")
        {
            qDebug()<<"CUC_BCont_updater:��Ч�Ĳ�������Ч��Ӧ�ó�����";
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
