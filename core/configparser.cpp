#include "configparser.h"

#include <QDataStream>
#include <iostream>
#include <QFile>
namespace CUCCore {
PlatformSetting::PlatformSetting(const QString &host_url,
                                 const QString &title,
                                 bool ismaxsize,
                                 int width,
                                 int height,
                                 QObject *parent):
    host_url_(host_url),
    title_(title),
    ismaxsize_(ismaxsize),
    width_(width),
    height_(height),
    QObject(parent)

{

    qDebug()<<"PlatformSetting::PlatformSetting():PlatformSetting created";


}

ConfigParser::ConfigParser(QObject *parent):QObject(parent),setting_(NULL)
{

    qDebug()<<"ConfigParser::ConfigParser()��ConfigParser created";
#ifdef CUC_TEST
    test();
#endif




}
bool ConfigParser::parse()
{
    return this->parserConfigFile();
}

bool ConfigParser::parserConfigFile()
{
    //�������������Ϣ�Ķ������ļ���QFile����;�����ļ�����ڱ������dataĿ¼��
    QFile file(qApp->applicationDirPath()+"/data/settings.dat");
    if(!file.exists())
    {

        //���ļ������ڣ��򷵻�false
        return false;

    }
    else if(!file.open(QIODevice::ReadOnly))
    {
        FILE_OPEN_ERROR(file);
        //���ļ����ڵ�����´򲻿����򷢳����������źţ���Browser�ദ��
        emit fatalError(OPEN_SETTINGS_FILE_FAILD);

    }
    else
    {
        QDataStream in(&file);
        QString host_url;
        QString title;
        bool ismaxsize=true;
        int width=0;
        int height=0;
        //ע�⣺����Ķ���˳�򲻿ɸı䣡
        in>>host_url>>title>>ismaxsize>>width>>height;
        this->setting_=new PlatformSetting(host_url,title,ismaxsize,width,height,this);
        setting_->tst_print();
        return true;
    }
}

#ifdef CUC_TEST
    /*
    *test():������Ժ�������һ����pareserConfigFileʹ�õ�settings.dat�ļ�
    *
    */
    void ConfigParser::test()
    {
         qDebug()<<"ConfigParser::test()��ConfigParser test begin";
        PlatformSetting t_setting("http://www.baidu.com","���Դ���",true,800,600);
        QFile file(qApp->applicationDirPath()+"/data/settings.dat");
        if(!file.open(QIODevice::WriteOnly))
        {
            FILE_OPEN_ERROR(file);

        }
        else
        {
            QDataStream out(&file);
            out<<t_setting.getHostUrl();
            out<<t_setting.getWindowTitle();
            out<<t_setting.getWindowISMaxsize();
            out<<t_setting.getWindowWidth();
            out<<t_setting.getWindowHeight();
            out.setVersion(QDataStream::Qt_4_0);
            qDebug()<<"ConfigParser::test()��write test data completed";
        }
    }

#endif
}//namespace CUCCore
