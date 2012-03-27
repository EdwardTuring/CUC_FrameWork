#include "webpage.h"
#include "qt4.h"
#include <QWebFrame>
#include <QDir>
#include "browser.h"
#include "../gui/mainwindow.h"
#include "../gui/popupwindow.h"

extern UIC::Browser *BROWSER;
namespace Web
{
WebPage::WebPage(QWidget *parent, QWidget *mainwindow):QWebPage(parent)
{
    this->windowx_=new JSOBJWindowx(mainwindow);
    //下面连结事件：
    CONNECT(windowx_,closeMe(QWidget *),BROWSER,closeChildWindow(QWidget *));
    //plugins=new QVector<Plugin::CUCPluginInterface* >;
    CONNECT(this->mainFrame(),javaScriptWindowObjectCleared(),this,addJSOBJ());
}
WebPage::WebPage(QWidget *parent):QWebPage(parent)
{
    //留补
}
void WebPage::javaScriptAlert(QWebFrame *frame, const QString &msg)
{

    QMessageBox::information(qobject_cast<QWidget *>(this->parent()), "提示", Qt::escape(msg), QMessageBox::Ok);

}
bool WebPage::javaScriptConfirm( QWebFrame * frame, const QString & msg )
{
    QMessageBox confirm_msg(qobject_cast<QWidget *>(this->parent()));
    confirm_msg.setWindowTitle("操作确认");
    confirm_msg.setText(msg);
   QAbstractButton *btn_yes=confirm_msg.addButton("确定",QMessageBox::ActionRole);
    QAbstractButton *btn_no=confirm_msg.addButton("取消",QMessageBox::ActionRole);
    confirm_msg.exec();
    if(confirm_msg.clickedButton()==btn_yes)
    {
        return true;
    }
    else
    {
        return false;
    }

}


QWebPage *WebPage::createWindow(WindowFeaturesQt feature)
{

    QUrl url("");
    UIC::MainWindow *mainwindow=BROWSER->getMainWindow();
    UIC::PopupWindow *wnd;
    if(!feature.dialog)
    {
        wnd=new  UIC::PopupWindow(mainwindow);
    }

    else
    {

        wnd=new  UIC::PopupWindow;
    }
    QVector<UIC::PopupWindow *> *popwnds=mainwindow->getPopWindows();
    popwnds->push_back(wnd);
   // QMap<QString ,UIC::PopupWindow *> *children= BROWSER->getMainWindow()->getChildWindow();
    QString title=feature.title;
//    if(children->contains(title))
//    {

//        int i=1;

//        while (children->contains(title+QString::number(i)))
//        {
//            i++;

//        }
//        title+=QString::number(i);

//    }
  //  children->insert(title,wnd);
    wnd->setTitle(title);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect d = desktopWidget->availableGeometry();
    //移动至屏幕中央
    wnd->move((d.width()-feature.width)/2,(d.height()-feature.height)/2);
    //重设窗口大小
    wnd->ReSize(feature.width,feature.height);

    if(feature.maxsize)
        wnd->showMaxsize();
    if(feature.fullscreen)
        wnd->showFullScreen();
    //设置为主窗体的网络连接管理类，解决cookie不一致的问题
    wnd->view()->page()->setNetworkAccessManager(networkAccessManager());

    return wnd->view()->page();

}
void WebPage::addJSOBJ()
{
    //载入插件{
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins/cuc_plugins");
    QStringList filters;
      filters << "*.dll" ;
       pluginsDir.setNameFilters(filters);
    foreach (QString fileName,pluginsDir.entryList(filters))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        fileName.truncate(fileName.lastIndexOf("."));
        qDebug()<<"loading plugin:"+fileName;
        if (plugin)
        {
               this->mainFrame()->addToJavaScriptWindowObject(fileName,plugin);
               qDebug()<<(fileName+"loaded");
        }
    }
    if(this->windowx_)
    {
        this->mainFrame()->addToJavaScriptWindowObject("windowx",this->windowx_);
    }
}
}//namespace Web
