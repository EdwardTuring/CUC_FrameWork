#include "browser.h"
#include <QSettings>
#include <iostream>
#include "web/webview.h"
#include "gui/ui/repodialog.h"
#include "gui/ui/guidedialog.h"
UIC::Browser *BROWSER=NULL;
namespace UIC {
Browser::Browser(QObject *parent) :
    QObject(parent)

{

    config_parser_=new CUCCore::ConfigParser(this);//初始化配置文件解析对象
    CONNECT(config_parser_,fatalError(int),this,fatalErroroccurred(int));
    handleConfig();
    time_openwindow_ = new QTimer(this); //初始化定时器
    QPixmap pixmap(":/init_pic.jpg");
    BROWSER=this;
    splash_ = new SplashScreen(pixmap);
    qApp->processEvents();
    splash_->show();


    splash_->showMessage("正在为你载入UICreator...");



    this->browser_=new MainWindow(url_,title_);
    browser_->setWindowIcon(QIcon(":icon.png"));

    if(this->ismaxsize_)
    {

        browser_->showMaxSizeScreen();

    }
    else
    {
        browser_->resize(this->width_,this->height_);
        QDesktopWidget* desktopWidget = QApplication::desktop();
        //获取可用桌面大小
        QRect d = desktopWidget->availableGeometry();
        browser_->move((d.width()-width_)/2,(d.height()-height_)/2);
    }

    handleConnectEvents();//注意：必须在所连接的对象构造完成之后调用！所以一般放在构造函数的最后





}
void Browser::fatalErroroccurred(int error_code)
{
#ifdef CUC_DEBUG
    qDebug()<<"Browser::fatalErroroccurred():called";
#endif
    std::cerr<<"Browser::fatalErroroccurred()：致命错误：CUCCore::ConfigParser::parserError("<<error_code<<")"<<std::endl;

    //弹出错误提示框
    QMessageBox::warning(NULL,"错误","无法打开配置文件！",QMessageBox::Close);
    //无法进行下去，立即退出
    exit(0);
}

void Browser::handleConnectEvents()
{
#ifdef CUC_DEBUG
    qDebug()<<"Browser::handleConnectEvents():called";
#endif

    CONNECT(this->browser_->view()->page(),loadFinished(bool),this,startTimeCount());
    CONNECT(this->time_openwindow_,timeout(),this,finishLoad());
}

void Browser::handleConfig()
{


    if(config_parser_->parse())
    {
        //读取配置文件的内容：
        readConfig();
    }
    else
    {
        /*
        *
        TODO:连接bcont服务器以获取目标服务器的ip地址。
        这里应该开辟一个http线程来做这个工作。
        */
        GuideDialog gdialog;
        CONNECT(&gdialog,quit(),this,doExit());

        gdialog.exec();//执行

    }
}
/*
*Browser::exit():利用系统调用exit退出进程
*/
void Browser::doExit()
{
    exit(0);
}

Browser::~Browser()
{

}

void Browser::readConfig()
{


    url_=config_parser_->getPlatformSetting()->getHostUrl();
    title_=config_parser_->getPlatformSetting()->getWindowTitle();
    width_=config_parser_->getPlatformSetting()->getWindowWidth();
    height_=config_parser_->getPlatformSetting()->getWindowHeight();
    ismaxsize_=config_parser_->getPlatformSetting()->getWindowISMaxsize();
}

void Browser::writeDefualtConfig(QSettings &config)
{
//已废弃。
    //    QString keys[]={"url","title","width","height","maxsize"};
//    QMap<QString,QString> attribute;
//    attribute["url"]="http://www.baidu.com";
//    attribute["title"]="基于Webkit的Web App支撑平台";
//    attribute["width"]="800";
//    attribute["height"]="600";
//    attribute["maxsize"]="0";
//    for(int i=0;i<attribute.size();i++)
//    {
//        config.setValue("CUC_FrameWork/"+keys[i], attribute.value(keys[i]));

//    }
}
void Browser::startTimeCount()
{
    this->time_openwindow_->start(2500);

}

void Browser::finishLoad()
{


    if(splash_)
    {
        delete splash_;
        splash_=NULL;
    }
    this->show();

    // connectToPluginRepository(); //当载入画面完成之后，进行插件仓库的连接


}

void Browser::connectToPluginRepository()
{
    qDebug()<<"function connectToPluginRepository works";
    RepoDialog repodialog;
    repodialog.setRepoUrl("http://127.0.0.1/ui_client/repository");
    repodialog.exec();
    this->time_openwindow_->stop();
}
void Browser::closeChildWindow(QWidget *child_wnd)
{
    if( child_wnd->close())
        qDebug()<<"closed \n";
    else
    {
        qDebug()<<"error when closed \n";
    }
}
} //namespace UIC
