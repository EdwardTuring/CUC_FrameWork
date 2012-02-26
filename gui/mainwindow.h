#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtGui>
 #include <QKeyEvent>
#include <QAction>

#include <QShortcut>
#include "qt4.h"
#include "web/webview.h"
#include "gui/ui/slidewidget.h"
using namespace Web;
namespace UIC
{
class PopupWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QUrl& url,const QString &_title,QWidget *parent=0);
    ~MainWindow(){delete this->childwnd_;}
    void showMaxSizeScreen();
    void showNormalSCreen(){this->toNormalScreen();}
    void showFullScreenX(){this->toFullScreen();}
    QRect getAvailableGeometry(){return this->availableGeometry;}
    QWebView *view(){return view_;}
    void Move(float x,float y);
    void ReSize(float width,float height);
    void setTitle(const QString title);
    QMap<QString ,UIC::PopupWindow*> *getChildWindow(){return childwnd_;}
    int getProcess(){return this->progress_;}//获取当前的载入进度

public slots:

 signals:
    void MaxSizeChanged();
    void sizeChanged();
    void posChanged();

protected:
    void setActionsAndShortCuts();
    void changeEvent(QEvent *event);

protected slots:
    void toFullScreen();
    void toNormalScreen();
    void toMaxSizeScreen();
    void adjustTitle();
    void changeSize();
    void changePos();
    void setProgress(int p);
    void setMaxSize();
    void toFullOrNormalScreen();


protected:
    WebView *view_;
    QString title_;
    int progress_;
    QSize normalsize_;
    bool ismaxsize_; //在全屏前是否为最大化状态
    QPoint window_pos_;


private:
    QRect availableGeometry;

     QShortcut  *sht_fullscreen_;
     QShortcut  *sht_reload_;
     QShortcut *sht_normalscreen_;
      QShortcut *sh_inspector_;
     QMap<QString ,UIC::PopupWindow*> *childwnd_;

     //child window:
    SlideWidget *slidewidget_;




};
}//namespace UIC
#endif
