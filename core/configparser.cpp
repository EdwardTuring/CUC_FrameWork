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

    qDebug()<<"ConfigParser::ConfigParser()：ConfigParser created";
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
    //构建存放设置信息的二进制文件的QFile对象;配置文件存放在本程序的data目录中
    QFile file(qApp->applicationDirPath()+"/data/settings.dat");
    if(!file.exists())
    {

        //若文件不存在，则返回false
        return false;

    }
    else if(!file.open(QIODevice::ReadOnly))
    {
        FILE_OPEN_ERROR(file);
        //在文件存在的情况下打不开，则发出致命错误信号，由Browser类处理
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
        //注意：下面的读入顺序不可改变！
        in>>host_url>>title>>ismaxsize>>width>>height;
        this->setting_=new PlatformSetting(host_url,title,ismaxsize,width,height,this);
        setting_->tst_print();
        return true;
    }
}

#ifdef CUC_TEST
    /*
    *test():这个测试函数生成一个供pareserConfigFile使用的settings.dat文件
    *
    */
    void ConfigParser::test()
    {
         qDebug()<<"ConfigParser::test()：ConfigParser test begin";
        PlatformSetting t_setting("http://www.baidu.com","测试窗口",true,800,600);
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
            qDebug()<<"ConfigParser::test()：write test data completed";
        }
    }

#endif
}//namespace CUCCore
