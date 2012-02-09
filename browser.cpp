#include "browser.h"
#include <QSettings>
#include <QFile>
#include <QMap>
#include "web/webview.h"
#include "gui/ui/repodialog.h"
namespace UIC {
Browser::Browser(QObject *parent) :
    QObject(parent)
{
    handleConfig();
    time_openwindow_ = new QTimer(this); //��ʼ����ʱ��
    QPixmap pixmap(":/init_pic.jpg");

    splash_ = new SplashScreen(pixmap);
    qApp->processEvents();
    splash_->show();

    splash_->showMessage("����Ϊ������UICreator v1.213...");
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


    CONNECT(this->browser_->view()->page(),loadFinished(bool),this,startTimeCount());
    CONNECT(this->time_openwindow_,timeout(),this,finishLoad());




}
void Browser::handleConfig()
{
    bool config_exist_=QFile::exists("config.ini");

    if(config_exist_)
    {
         QSettings config("config.ini",QSettings::IniFormat);
          config.setIniCodec(QTextCodec::codecForName("System"));

        //��ȡ�����ļ������ݣ�
        readConfig(config);
    }
    else
    {
         QSettings config("config.ini",QSettings::IniFormat);
          config.setIniCodec(QTextCodec::codecForName("System"));
         //д��Ĭ�ϵ����ã�
         writeDefualtConfig(config);
         readConfig(config);
    }
}

Browser::~Browser()
{

}

void Browser::readConfig(const QSettings &config)
{


  url_=config.value("CUC_FrameWork/url").toString();
   title_=config.value("CUC_FrameWork/title").toString();
   this->width_=config.value("CUC_FrameWork/width").toInt();
    this->height_=config.value("CUC_FrameWork/height").toInt();
   if(config.value("CUC_FrameWork/maxsize").toInt()==1)
   {
       this->ismaxsize_=true;
   }
   else
   {
        this->ismaxsize_=false;
   }
}

void Browser::writeDefualtConfig(QSettings &config)
{
    QString keys[]={"url","title","width","height","maxsize"};
    QMap<QString,QString> attribute;
    attribute["url"]="http://www.baidu.com";
    attribute["title"]="����Webkit��Web App֧��ƽ̨";
    attribute["width"]="800";
    attribute["height"]="600";
    attribute["maxsize"]="0";
    for(int i=0;i<attribute.size();i++)
    {
        config.setValue("CUC_FrameWork/"+keys[i], attribute.value(keys[i]));

    }
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
    connectToPluginRepository(); //�����뻭�����֮�󣬽��в���ֿ������

}

void Browser::connectToPluginRepository()
{
    qDebug()<<"function connectToPluginRepository works";
    RepoDialog repodialog;
    repodialog.setRepoUrl("http://www.baidu.com");
    repodialog.exec();
    this->time_openwindow_->stop();
}
} //namespace UIC
