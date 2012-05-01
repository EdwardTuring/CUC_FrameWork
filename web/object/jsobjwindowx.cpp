#include "jsobjwindowx.h"
#include "browser.h"
#include "gui/popupwindow.h"
#include "gui/ui/aboutdialog.h"
extern UIC::Browser *BROWSER;

namespace Web{
JSOBJWindowx::JSOBJWindowx(QWidget *parent) :
    QObject(parent)
{
    this->parent_=parent;
    qDebug()<<"windowx \n";
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
void JSOBJWindowx::close()
{
    qobject_cast<UIC::PopupWindow *>(parent_)->debug();
    emit closeMe(parent_);
   // BROWSER->getMainWindow()->close();

}

void JSOBJWindowx::closeChildWindow(const QString title)
{

}
void JSOBJWindowx::prompt(const QString &title, const QString &msg, int time_to_close)
{
    BROWSER->getMainWindow()->showTrayIconMessage(title,msg,time_to_close);
}

void JSOBJWindowx::showAboutDialog(const QString &msg)
{

    qDebug()<<"showAboutDialog works";
    AboutDialog about(qobject_cast<QWidget *>(parent()->parent()));
    about.exec();

}
}//namespace Web

