#include "jsobjwindowx.h"
#include "browser.h"
#include <QFile>
#include "gui/popupwindow.h"
#include "gui/ui/aboutdialog.h"
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

void JSOBJWindowx::closeChildWindow(const QString title)
{
    QMap<QString ,UIC::PopupWindow*> *children=BROWSER->getMainWindow()->getChildWindow();
    if(children->contains(title))
    {
        children->value(title)->close();

    }
}
void JSOBJWindowx::showAboutDialog(const QString &msg)
{

    qDebug()<<"showAboutDialog works";
    AboutDialog about(qobject_cast<QWidget *>(parent()->parent()));
    about.exec();

}
void JSOBJWindowx::debug(const QString &msg)
{
    qDebug()<<"---------------WINDOWX DEBUG-----------------------\n";
    qDebug()<<msg+"\n";
    qDebug()<<"---------------"+QTime::currentTime().toString()+"--------------------------\n";
}
void JSOBJWindowx::log(const QString &msg)
{

    QDir logdir = QDir(qApp->applicationDirPath());
     logdir.mkdir("log");
     logdir.cd("log");
     QFile log_file(logdir.absoluteFilePath("log.txt"));
     if(log_file.open(QFile::Append| QIODevice::Text)){
           QTextStream out(&log_file);
       out<<"---------------WINDOWX LOG-----------------------\n";
       out<<msg+"\n";
      out<<"---------------"+QTime::currentTime().toString()+"--------------------------\n";
       log_file.close();
     }
     else
           return;
}
}//namespace Web

