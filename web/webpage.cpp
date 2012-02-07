#include "webpage.h"
#include "qt4.h"
#include <QWebFrame>
#include "browser.h"
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

    QMessageBox::information(BROWSER->getMainWindow(), "��ʾ", Qt::escape(msg), QMessageBox::Ok);

}
bool WebPage::javaScriptConfirm( QWebFrame * frame, const QString & msg )
{
    return QMessageBox::Yes == QMessageBox::information(BROWSER->getMainWindow(), "����ȷ��",Qt::escape(msg), "ȷ��","ȡ��");

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
    if(this->windowx_)
    {
        this->mainFrame()->addToJavaScriptWindowObject("windowx",this->windowx_);
    }
}
}//namespace Web
