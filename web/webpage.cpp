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
WebPage::WebPage(QWidget *parent):QWebPage(parent)
{
    this->windowx_=new JSOBJWindowx(this);
    getPluginsFromDll();
    //plugins=new QVector<Plugin::CUCPluginInterface* >;
    CONNECT(this->mainFrame(),javaScriptWindowObjectCleared(),this,addJSOBJ());


}
void WebPage::getPluginsFromDll()
{
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
               plugins_.insert(fileName,plugin);
               qDebug()<<(fileName+" loaded");
        }
    }
}

WebPage::~WebPage()
{

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


QWebPage *WebPage::createWindow(WebWindowType type)
{

    QUrl url("");
    UIC::PopupWindow *wnd=new  UIC::PopupWindow(BROWSER->getMainWindow());
    QMap<QString ,UIC::PopupWindow *> *children= BROWSER->getMainWindow()->getChildWindow();

    //����Ϊ��������������ӹ����࣬���cookie��һ�µ�����
    wnd->view()->page()->setNetworkAccessManager(networkAccessManager());

    return wnd->view()->page();

}
void WebPage::addJSOBJ()
{
    QList<QString> plugin_names=plugins_.keys();
    for(int i=0;i<plugins_.size();i++)
    {
        QString plugin_name=plugin_names[i];

            //TODO:����QMap plugins����QObject�������뵽javaScript��������
            this->mainFrame()->addToJavaScriptWindowObject(plugin_name,plugins_[plugin_name]);
    }
    if(this->windowx_)
    {
        this->mainFrame()->addToJavaScriptWindowObject("windowx",this->windowx_);
    }
}
}//namespace Web
