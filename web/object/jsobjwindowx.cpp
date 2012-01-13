#include "jsobjwindowx.h"
#include "browser.h"
#include "gui/popupwindow.h"
extern UIC::Browser *BROWSER;

namespace Web{
JSOBJWindowx::JSOBJWindowx(QObject *parent) :
    QObject(parent)
{
}

void JSOBJWindowx::showFullScreen()
{
    BROWSER->getMainWindow()->showFullScreenX();
}
void JSOBJWindowx::showMaxSize()
{
    BROWSER->getMainWindow()->showMaxSizeScreen();
}
void JSOBJWindowx::showNormal()
{
    BROWSER->getMainWindow()->showNormalSCreen();
}
QString JSOBJWindowx::getChildWindow()
{
    QString str="";
    QList<QString> list=BROWSER->getMainWindow()->getChildWindow()->uniqueKeys();

    int i;
    for(i=0;i<list.length()-1;i++)
    {
        QString temp=list.value(i)+",";
        str+=temp;
    }
    str+=list.value(i);
    return str;
}
void JSOBJWindowx::quit()
{
    qobject_cast<QWidget*>(qobject_cast<QWebPage*>(parent())->view()->parent())->close();
}
void JSOBJWindowx::refresh()
{
    qobject_cast<QWebView*>(qobject_cast<QWebPage*>(parent())->view())->reload();
}
void JSOBJWindowx::closeChildWindow(const QString title)
{
    QMap<QString ,UIC::PopupWindow*> *children=BROWSER->getMainWindow()->getChildWindow();
    if(children->contains(title))
    {
        children->value(title)->close();

    }
}
}//namespace Web
