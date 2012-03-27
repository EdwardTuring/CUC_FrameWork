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
    //���������¼���
    CONNECT(windowx_,closeMe(QWidget *),BROWSER,closeChildWindow(QWidget *));
    //plugins=new QVector<Plugin::CUCPluginInterface* >;
    CONNECT(this->mainFrame(),javaScriptWindowObjectCleared(),this,addJSOBJ());
}
WebPage::WebPage(QWidget *parent):QWebPage(parent)
{
    //����
}
void WebPage::javaScriptAlert(QWebFrame *frame, const QString &msg)
{

    QMessageBox::information(qobject_cast<QWidget *>(this->parent()), "��ʾ", Qt::escape(msg), QMessageBox::Ok);

}
bool WebPage::javaScriptConfirm( QWebFrame * frame, const QString & msg )
{
    QMessageBox confirm_msg(qobject_cast<QWidget *>(this->parent()));
    confirm_msg.setWindowTitle("����ȷ��");
    confirm_msg.setText(msg);
   QAbstractButton *btn_yes=confirm_msg.addButton("ȷ��",QMessageBox::ActionRole);
    QAbstractButton *btn_no=confirm_msg.addButton("ȡ��",QMessageBox::ActionRole);
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
    //��ȡ���������С
    QRect d = desktopWidget->availableGeometry();
    //�ƶ�����Ļ����
    wnd->move((d.width()-feature.width)/2,(d.height()-feature.height)/2);
    //���贰�ڴ�С
    wnd->ReSize(feature.width,feature.height);

    if(feature.maxsize)
        wnd->showMaxsize();
    if(feature.fullscreen)
        wnd->showFullScreen();
    //����Ϊ��������������ӹ����࣬���cookie��һ�µ�����
    wnd->view()->page()->setNetworkAccessManager(networkAccessManager());

    return wnd->view()->page();

}
void WebPage::addJSOBJ()
{
    //������{
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
