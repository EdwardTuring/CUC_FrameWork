#ifndef BROWESER_H
#define BROWESER_H

#include "qt4.h"
#include "gui/mainwindow.h"
#include "gui/splashscreen.h"
#include "core/configparser.h"
namespace UIC {
class RunUpdateThread:public QThread
{
    Q_OBJECT
public:
    RunUpdateThread(const QStringList &);
     void run();
private:
     QStringList arg_;
};
class Browser : public QObject
{
    Q_OBJECT
public:
    explicit Browser(QObject *parent = 0);
    ~Browser();
    bool isMaxSize(){return this->ismaxsize_;}
    int width(){return width_;}
    int height(){return height_;}
    QString title(){return title_;}
    QString url(){return url_;}
    MainWindow *getMainWindow(){return browser_;}
     void show(){this->browser_->show();}
    void setSplashScreen(UIC::SplashScreen *splash){splash_=splash;}
    UIC::SplashScreen *getSplashScreen(){return splash_;}

    void connectToPluginRepository();
public slots:
    void closeChildWindow(QWidget *);

     void showMainWindow(){this->browser_->show();}
    void hideMainWindow(){this->browser_->hide();}
protected slots:
    void finishLoad();
    void startTimeCount();
    void fatalErroroccurred(int error_code);
    void doExit(bool);
private slots:
    void versionCheckReply();
private://私有成员函数

    void handleConfig();
    void readConfig();
    void writeDefualtConfig(QSettings &config);
    void handleConnectEvents();

    void checkVersion();
private:



    QString title_;
    QString url_;
    QString product_name_;
    RunUpdateThread *update_thread_;
     int version_;
    int width_;
    int height_;

    bool ismaxsize_;//是否最大化显示；

     MainWindow *browser_;
    SplashScreen *splash_; //程序的载入页面

     QTimer *time_openwindow_;//打开窗口的定时器

     CUCCore::ConfigParser *config_parser_;

     QNetworkAccessManager *version_check_network_;
     QNetworkReply *version_check_reply_;
};


} //namespace UIC
#endif // BROWESER_H
