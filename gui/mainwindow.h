#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtGui>
#include <QKeyEvent>
#include <QAction>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QMenu>
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
    WebView *view(){return view_;}
    void Move(float x,float y);
    void ReSize(float width,float height);
    void setTitle(const QString title);
    QMap<QString ,UIC::PopupWindow*> *getChildWindow(){return childwnd_;}
    int getProcess(){return this->progress_;}//获取当前的载入进度
    void debug(){qDebug()<<"works \n";}
    QVector<UIC::PopupWindow *> *getPopWindows(){return this->popwindows_;}
public slots:

signals:
    void MaxSizeChanged();
    void sizeChanged();
    void posChanged();
    void hideMe();
protected:
    void setActionsAndShortCuts();
    void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *);
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
    void showInspector();

protected:
    WebView *view_;
    QString title_;
    int progress_;
    QSize normalsize_;
    bool ismaxsize_; //在全屏前是否为最大化状态
    QPoint window_pos_;
private slots:
    void quit();

private://私有的成员函数：
    void initSystemTrayIcon();
    void initTrayIconAction();

private:
    QRect availableGeometry;

    QShortcut  *sht_fullscreen_;
    QShortcut  *sht_reload_;
    QShortcut *sht_normalscreen_;
    QShortcut *sht_inspector_;
    QMainWindow *wnd_inspector_;
    QMap<QString ,UIC::PopupWindow*> *childwnd_;
    QVector<UIC::PopupWindow*> *popwindows_;
    //child window:
    /*slidewidget_已经废弃*/
    SlideWidget *slidewidget_;

    /*下面两个对像用于处理托盘图标*/
    QSystemTrayIcon *tray_icon_;
    QMenu *tray_icon_menu_;
    QAction *act_show_normal_;
    QAction *act_quit_;
    QAction *act_full_screen_;


};
}//namespace UIC
#endif
