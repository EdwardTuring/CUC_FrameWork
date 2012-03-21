#ifndef BROWESER_H
#define BROWESER_H

#include "qt4.h"
#include "gui/mainwindow.h"
#include "gui/splashscreen.h"
#include "core/configparser.h"
namespace UIC {

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
protected slots:
    void finishLoad();
    void startTimeCount();
    void fatalErroroccurred(CUCCore::ConfigParser::parserError error_code);
private:

    void handleConfig();
    void readConfig();
    void writeDefualtConfig(QSettings &config);
    void handleConnectEvents();
private:



    QString title_;
    QString url_;
    int width_;
    int height_;

    bool ismaxsize_;//�Ƿ������ʾ��

     MainWindow *browser_;
    SplashScreen *splash_; //���������ҳ��

     QTimer *time_openwindow_;//�򿪴��ڵĶ�ʱ��

     CUCCore::ConfigParser *config_parser_;
};
} //namespace UIC
#endif // BROWESER_H
