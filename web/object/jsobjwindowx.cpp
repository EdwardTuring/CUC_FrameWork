#include "jsobjwindowx.h"
#include "browser.h"
#include <QFile>
#include <QWebPage>
#include <QWebFrame>
#include "gui/popupwindow.h"
#include "gui/ui/aboutdialog.h"
extern UIC::Browser *BROWSER;
ReplyQueue::ReplyQueue(const QString &func,  QNetworkReply *r)
{
    this->func=func;
    this->reply=r;
}
ReplyQueue::ReplyQueue(const ReplyQueue &other)
{
    func=other.func;
    reply=other.reply;
}

namespace Web{


JSOBJWindowx::JSOBJWindowx(QObject *parent) :
    QObject(parent)
{
    net_manager_=new QNetworkAccessManager(this);
    flag_=true;
    reply_=NULL;

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

void JSOBJWindowx::post(const QString &url, const QMap<QString,QVariant> &data, const QString &func)
{
    qDebug()<<"JSOBJWindowx::post() :called";
     qDebug()<<"JSOBJWindowx::post() :url:"+url;
    QList<QString> keys=data.keys();
    QByteArray data_b;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QUrl tmp_url;
    for(int i=0;i<keys.size();i++)
    {
        QVariant tmp_var=data[keys[i]];
        if( tmp_var.type() == QVariant::String)
        {
             tmp_url.addQueryItem(keys[i],tmp_var.toString());
        }
        else if(tmp_var.type() == QVariant::List)
        {
            QList<QVariant> tmp_list=tmp_var.toList();
            for(int k=0;k<tmp_list.size();k++)
            {
                tmp_url.addQueryItem(keys[i]+"%5B%5D",tmp_list[k].toString());
            }
        }

    }
    qDebug()<<"JSOBJWindowx::post() :data string:"+tmp_url.toString();
    qDebug()<<"JSOBJWindowx::post() :callback:"+func;
    data_b.append(tmp_url.toString().toUtf8());
    data_b.remove(0,1);

       QNetworkReply  *reply=net_manager_->post(request,data_b);
       ReplyQueue tmp(func,reply);
       replys_.push_back(tmp);

        connect(reply,SIGNAL(error(QNetworkReply::NetworkError)),
                  this,SLOT(replyError(QNetworkReply::NetworkError)));
        connect(reply,SIGNAL(finished()),this,SLOT(slot_postFinished()));

}
void JSOBJWindowx::replyError(QNetworkReply::NetworkError code)
{
     qDebug()<<" FtpDataHelper::replyError():ERROR "+QString::number(code);
}
void JSOBJWindowx::slot_postFinished()
{
    if(!replys_.isEmpty())
    {
        ReplyQueue q=replys_.first();
        QNetworkReply *reply=q.reply;
        QByteArray tmp=reply->readAll();
        QString tmp_str(tmp);


        reply->deleteLater();

        qobject_cast<QWebPage*>(parent())->mainFrame()->evaluateJavaScript(q.func +"("+tmp_str+");null");

        if(flag_) flag_=false;
        replys_.pop_front();
 //   qDebug()<<"reply:";
    }
}
}//namespace Web

