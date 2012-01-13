#include "webpage.h"
#include "qt4.h"
#include <QWebFrame>
#include "browser.h"
#include "../gui/confirmbox.h"
#include "../gui/mainwindow.h"
#include "../gui/popupwindow.h"

extern UIC::Browser *BROWSER;
namespace Web
{
WebPage::WebPage(QWidget *parent):QWebPage(parent)
{
    this->windowx_=new JSOBJWindowx(this);
    CONNECT(this->mainFrame(),javaScriptWindowObjectCleared(),this,addJSOBJ());
}

void WebPage::javaScriptAlert(QWebFrame *frame, const QString &msg)
{


        QMessageBox msgBox;
    msgBox.setWindowTitle("ÌבÊ¾");
    msgBox.setIcon(QMessageBox::Information);
     msgBox.setText(msg);

        MOVETOBROWSERCENTER(msgBox);
     msgBox.exec();
}
bool WebPage::javaScriptConfirm( QWebFrame * frame, const QString & msg )
{
    bool value=true;
    UIC::ConfirmBox msgBox(msg,&value);
        MOVETOBROWSERCENTER(msgBox);
        msgBox.exec();
        return value;

}

QWebPage *WebPage::createWindow(WindowFeaturesQt feature)
{

    QUrl url("");
     UIC::PopupWindow *wnd=new  UIC::PopupWindow(BROWSER->getMainWindow());
   QMap<QString ,UIC::PopupWindow *> *children= BROWSER->getMainWindow()->getChildWindow();
   QString title=feature.title;
    if(children->contains(title))
    {

        int i=1;

        while (children->contains(title+QString::number(i)))
       {
            i++;

       }
               title+=QString::number(i);

    }
    children->insert(title,wnd);
    wnd->setTitle(title);

    wnd->Move(feature.x,feature.y);
    wnd->ReSize(feature.width,feature.height);
    if(feature.maxsize)
        wnd->showMaxsize();
     if(feature.fullscreen)
        wnd->showFullScreen();
    wnd->view()->page()->setNetworkAccessManager(networkAccessManager());
    return wnd->view()->page();

}
void WebPage::addJSOBJ()
{
        if(this->windowx_)
        {
                this->mainFrame()->addToJavaScriptWindowObject("windowx",this->windowx_);
       }
}
}//namespace Web
