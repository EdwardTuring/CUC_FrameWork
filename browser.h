#ifndef BROWESER_H
#define BROWESER_H

#include "qt4.h"
#include "gui/mainwindow.h"
#include "gui/splashscreen.h"
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
protected slots:
    void finishLoad();
private:

    void handleConfig();
    void readConfig(const QSettings &config);
    void writeDefualtConfig(QSettings &config);

private:



    QString title_;
    QString url_;
    int width_;
    int height_;

    bool ismaxsize_;//是否最大化显示；

     MainWindow *browser_;
     UIC::SplashScreen *splash_; //程序的载入页面
};
} //namespace UIC
#endif // BROWESER_H
