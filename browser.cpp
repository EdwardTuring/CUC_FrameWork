#include "browser.h"
#include <QSettings>
#include <iostream>
#include <QDomDocument>
#include "web/webview.h"
#include "gui/ui/repodialog.h"
#include "gui/ui/guidedialog.h"
#include "tool.h"
#include <Windows.h>
UIC::Browser *BROWSER=NULL;
namespace UIC {
Browser::Browser(QObject *parent) :
    QObject(parent)

{

    config_parser_=new CUCCore::ConfigParser(this);//初始化配置文件解析对象
    CONNECT(config_parser_,fatalError(int),this,fatalErroroccurred(int));
    handleConfig();
    time_openwindow_ = new QTimer(this); //初始化定时器
    QPixmap pixmap(":/startup.png");
    BROWSER=this;
    splash_ = new SplashScreen(pixmap);

    checkWorks();

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

    CONNECT(this->browser_,hideMe(),this,hideMainWindow());
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
        CONNECT(&gdialog,quit(bool),this,doExit(bool));

        gdialog.exec();//执行

    }
}
/*
*Browser::exit():利用系统调用exit退出进程
*/
void Browser::doExit(bool ispassed)
{

    if(!ispassed)
        exit(0);
    else
    {
        if(config_parser_->parse())
        {
            //读取配置文件的内容：
            readConfig();
        }
        else
        {
            //处理解析失败
        }
    }

}

Browser::~Browser()
{

}

void Browser::readConfig()
{

    //通常是http协议；我知道这很不优雅。。暂时这样吧
    url_="http://"+config_parser_->getPlatformSetting()->getHostUrl();
    title_=config_parser_->getPlatformSetting()->getWindowTitle();
    width_=config_parser_->getPlatformSetting()->getWindowWidth();
    height_=config_parser_->getPlatformSetting()->getWindowHeight();
    ismaxsize_=config_parser_->getPlatformSetting()->getWindowISMaxsize();
    product_name_ = config_parser_->getPlatformSetting()->getProductName();
}
void Browser::versionCheckReply()
{
    qDebug()<<"Browser::versionCheckReply():called";
    QByteArray tmp_a=version_check_reply_->readAll();
    QString tmp_str=QString::fromUtf8(tmp_a);
    qDebug()<<tmp_str;
    //按照服务器端的php脚本所指定的规则，当返回的是空字符串时，则验证失败；
    if(tmp_str=="")
    {

        qDebug()<<"版本验证失败";
        return ;
    }
    QDomDocument doc;
    doc.setContent(tmp_str);
    QDomElement doc_el = doc.documentElement();

    //从xml字符串tmp_str中获取有关信息
    QString host=doc_el.firstChildElement("host").text();
    qDebug()<<host;
    QString port=doc_el.firstChildElement("port").text();
    QString username=doc_el.firstChildElement("username").text();
    QString pwd=doc_el.firstChildElement("pwd").text();
    QString v=doc_el.firstChildElement("version").text();

    QString patch=doc_el.firstChildElement("patch").text();
    bool ok=false;
    int tmp_version=v.toInt(&ok);
    if(!ok)
    {
        information("无法进行版本验证");
        QApplication::exit(0);
    }
    if(tmp_version > version_)
    {

        QStringList tmp_pram;
        tmp_pram.append(host);
        tmp_pram.append(port);
        tmp_pram.append(username);
        tmp_pram.append(pwd);
        tmp_pram.append(patch+".cucp");
        tmp_pram.append(product_name_);
        tmp_pram.append(v);
        qDebug()<<qApp->applicationName();
        tmp_pram.append(qApp->applicationName());
        update_thread_=new RunUpdateThread(tmp_pram);

        update_thread_->start();

    }
}
void Browser::checkWorks()
{
    check_network_ = new QNetworkAccessManager(this);

    //检查版本
    checkVersion();
    //TODO:在此处进行服务器的连接测;
    checkServerStatus();
}

void Browser::checkVersion()
{

    //如果还有更新进程，那么停掉它
#ifdef Q_WS_WIN

    HWND tmp_hwnd =FindWindow(NULL,L"CUC_BCont updater");

    if (tmp_hwnd!=NULL)
    {

        SendMessage(tmp_hwnd,WM_CLOSE,NULL,NULL);
    }

#endif

    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\Software\\BCont Software\\"+product_name_+"\\",
                                   QSettings::NativeFormat);
    QString tmp_id=reg->value("product_id").toString();
    version_ = reg->value("version").toInt();
    delete reg;
    QNetworkRequest request(QUrl("http://bcont.cuc.edu.cn/customer/index.php/welcome/getversion/"+tmp_id));
    request.setRawHeader("User-Agent", "CUC_FrameWork 1.4");
    version_check_reply_=check_network_->get(request);
    CONNECT(version_check_reply_,finished(),this,versionCheckReply());
    CONNECT(version_check_reply_,error(QNetworkReply::NetworkError),
            this,versionCheckReplyError(QNetworkReply::NetworkError));


}
void Browser::versionCheckReplyError(QNetworkReply::NetworkError)
{

}

void Browser::checkServerStatus()
{
    QUrl url(url_);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "CUC_FrameWork 1.4");
    server_check_reply_=check_network_->get(request);
    CONNECT(server_check_reply_,finished(),this,serverCheckReply());
    CONNECT(server_check_reply_,error(QNetworkReply::NetworkError),
            this,serverCheckReplyError(QNetworkReply::NetworkError));
}
void Browser::serverCheckReply()
{
    //do nothing....
    qDebug()<<"Browser::serverCheckReply() called";
    qApp->processEvents();
    splash_->show();

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
void Browser::serverCheckReplyError(QNetworkReply::NetworkError error)
{
  qDebug()<<"Browser::serverCheckReplyError() called";
    if(error == QNetworkReply :: NoError)
    {

           }

        if(QNetworkReply :: HostNotFoundError)
        {
                //主机未找到，删除验证文件，以重新验证
                deletePatchDir(qApp->applicationDirPath() + "/data");
                handleConfig();
        }



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

    this->time_openwindow_->stop();
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
RunUpdateThread::RunUpdateThread(const QStringList &arg):arg_(arg)
{

}

void RunUpdateThread::run()
{
    QProcess::execute("CUC_BCont_updater",arg_);
}
} //namespace UIC
