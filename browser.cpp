#include "browser.h"
#include <QSettings>
#include <iostream>
#include <QDomDocument>
#include "web/webview.h"
#include "gui/ui/repodialog.h"
#include "gui/ui/guidedialog.h"
#include "tool.h"
UIC::Browser *BROWSER=NULL;
namespace UIC {
Browser::Browser(QObject *parent) :
    QObject(parent)

{

    config_parser_=new CUCCore::ConfigParser(this);//��ʼ�������ļ���������
    CONNECT(config_parser_,fatalError(int),this,fatalErroroccurred(int));
    handleConfig();
    time_openwindow_ = new QTimer(this); //��ʼ����ʱ��
    QPixmap pixmap(":/startup.png");
    BROWSER=this;
    splash_ = new SplashScreen(pixmap);
    //���汾
    checkVersion();

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

    CONNECT(this->browser_,hideMe(),this,hideMainWindow());
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
        CONNECT(&gdialog,quit(bool),this,doExit(bool));

        gdialog.exec();//ִ��

    }
}
/*
*Browser::exit():����ϵͳ����exit�˳�����
*/
void Browser::doExit(bool ispassed)
{

    if(!ispassed)
        exit(0);
    else
    {
        if(config_parser_->parse())
        {
            //��ȡ�����ļ������ݣ�
            readConfig();
        }
        else
        {
            //�������ʧ��
        }
    }

}

Browser::~Browser()
{

}

void Browser::readConfig()
{

    //ͨ����httpЭ�飻��֪����ܲ����š�����ʱ������
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
    //���շ������˵�php�ű���ָ���Ĺ��򣬵����ص��ǿ��ַ���ʱ������֤ʧ�ܣ�
    if(tmp_str=="")
    {

        qDebug()<<"�汾��֤ʧ��";
        return ;
    }
    QDomDocument doc;
    doc.setContent(tmp_str);
    QDomElement doc_el = doc.documentElement();

    //��xml�ַ���tmp_str�л�ȡ�й���Ϣ
    QString host=doc_el.firstChildElement("host").text();
    qDebug()<<host;
    QString port=doc_el.firstChildElement("port").text();
    QString username=doc_el.firstChildElement("username").text();
    QString pwd=doc_el.firstChildElement("pwd").text();
    QString v=doc_el.firstChildElement("version").text();
    qDebug()<<v;
    QString patch=doc_el.firstChildElement("patch").text();
   bool ok=false;
    int tmp_version=v.toInt(&ok);
    if(!ok)
    {
        information("�޷����а汾��֤");
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
     update_thread_=new RunUpdateThread(tmp_pram);

     //   update_thread_->start();

    }
}

void Browser::checkVersion()
{
    //TODO:��bcont���Ͱ汾�������
    version_check_network_ = new QNetworkAccessManager(this);
    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\Software\\BCont Software\\"+product_name_+"\\",
                         QSettings::NativeFormat);
    QString tmp_id=reg->value("product_id").toString();
    version_ = reg->value("version").toInt();
    QNetworkRequest request(QUrl("http://bcont.cuc.edu.cn/customer/index.php/welcome/getversion/"+tmp_id));
    request.setRawHeader("User-Agent", "CUC_FrameWork 1.4");
    version_check_reply_=version_check_network_->get(request);
    CONNECT(version_check_reply_,finished(),this,versionCheckReply());
    CONNECT(version_check_reply_,error(QNetworkReply::NetworkError),this,replyError(QNetworkReply::NetworkError));


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

    this->time_openwindow_->stop();
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
RunUpdateThread::RunUpdateThread(const QStringList &arg):arg_(arg)
{

}

void RunUpdateThread::run()
{
    QProcess::execute("CUC_BCont_updater",arg_);
}
} //namespace UIC
