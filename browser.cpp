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

    config_parser_=new CUCCore::ConfigParser(this);//��ʼ�������ļ���������
    CONNECT(config_parser_,fatalError(int),this,fatalErroroccurred(int));
    handleConfig();
    time_openwindow_ = new QTimer(this); //��ʼ����ʱ��
    QPixmap pixmap(":/init_pic.jpg");
    BROWSER=this;
    splash_ = new SplashScreen(pixmap);
    qApp->processEvents();
    splash_->show();


    splash_->showMessage("����Ϊ������UICreator...");



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
        //��ȡ���������С
        QRect d = desktopWidget->availableGeometry();
        browser_->move((d.width()-width_)/2,(d.height()-height_)/2);
    }

    handleConnectEvents();//ע�⣺�����������ӵĶ��������֮����ã�����һ����ڹ��캯�������





}
void Browser::fatalErroroccurred(int error_code)
{
#ifdef CUC_DEBUG
    qDebug()<<"Browser::fatalErroroccurred():called";
#endif
    std::cerr<<"Browser::fatalErroroccurred()����������CUCCore::ConfigParser::parserError("<<error_code<<")"<<std::endl;

    //����������ʾ��
    QMessageBox::warning(NULL,"����","�޷��������ļ���",QMessageBox::Close);
    //�޷�������ȥ�������˳�
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
        //��ȡ�����ļ������ݣ�
        readConfig();
    }
    else
    {
        /*
        *
        TODO:����bcont�������Ի�ȡĿ���������ip��ַ��
        ����Ӧ�ÿ���һ��http�߳��������������
        */
        GuideDialog gdialog;
        CONNECT(&gdialog,quit(),this,doExit());

        gdialog.exec();//ִ��

    }
}
/*
*Browser::exit():����ϵͳ����exit�˳�����
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
//�ѷ�����
    //    QString keys[]={"url","title","width","height","maxsize"};
//    QMap<QString,QString> attribute;
//    attribute["url"]="http://www.baidu.com";
//    attribute["title"]="����Webkit��Web App֧��ƽ̨";
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

    // connectToPluginRepository(); //�����뻭�����֮�󣬽��в���ֿ������


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
