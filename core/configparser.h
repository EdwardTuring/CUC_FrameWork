#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include "qt4.h"
#include "coreerror.h"
namespace CUCCore{
class PlatformSetting:public QObject{

    Q_OBJECT
public:
    /*
     *构造函数传入基本的设置信息。包括连接服务器的主机地址、
     主窗口的标题、初始状态是否最大化和初始窗口的高度和宽度（
     当设置为最大化时，这两项会被忽略）。
     */
    PlatformSetting(const QString &host_url,
                    const QString &title,
                    bool ismaxsize,
                    int width,
                    int height,
                    QObject *parent=0
                    );
    //PlatformSetting(const PlatformSetting &other);
    /*
     *下面的操作能够获取到此类的成员变量，采用内联的方式提高效率。
     */
    QString getHostUrl()    const{return host_url_;}
    QString getWindowTitle()    const{return title_;}
    bool getWindowISMaxsize()   const{return ismaxsize_;}
    int getWindowWidth()    const{return width_;}
    int getWindowHeight()   const{return height_;}
#ifdef CUC_TEST
    void tst_print() const{qDebug()<<"PlatformSetting::tst_print():成员变量的值："<<host_url_
                                  <<" "<<title_<<" "<<QString::number((int)ismaxsize_)
                                 <<" "<<QString::number(width_)
                                <<" "<<QString::number(height_);}
#endif
 private:
    QString host_url_;
    QString title_;
    bool ismaxsize_;
    int width_;
    int height_;
};

class ConfigParser:public QObject
{
    Q_OBJECT
public:
    ConfigParser(QObject *parent=0);
    PlatformSetting *getPlatformSetting()   const{return setting_;}

    enum  parserError
    {

        OPEN_SETTINGS_FILE_FAILD //打开配置文件失败

    };
     bool parse();
#ifdef CUC_TEST
    void test();
#endif
signals:
    void fatalError(parserError);

protected:
    /*
      * pareserConfigFile()：解析二进制配置文件，若文件解析成功返回true，文件不存在返回false；
      在文件存在的情况下无法打开，则触发致命错误。
      */
    bool parserConfigFile();
private:
    PlatformSetting *setting_;


};

}

#endif // CONFIGPARSER_H
