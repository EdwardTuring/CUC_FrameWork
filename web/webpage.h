#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QWebPage>
#include <QtCore>
#include <QWebInspector>
#include "object/jsobjwindowx.h"

namespace Web
{
class WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit WebPage(QWidget *parent = 0);
    ~WebPage();
protected:
    void javaScriptAlert ( QWebFrame * frame, const QString & msg );
    bool javaScriptConfirm( QWebFrame * frame, const QString & msg );
    QWebPage *createWindow(WindowFeaturesQt feature);
public slots:
    void addJSOBJ();
private:
    /**
     *getPluginsFromDll:从dll动态链接库中导入javascript插件,存入此类的
       成员变量QMap映射表中
      @return void
      @author Ma Xiao
      @date 2012.4.2
    */
    void getPluginsFromDll();

private:

    JSOBJWindowx *windowx_;
    /**
    *现在，我将FtpPlugin直接嵌入浏览器壳；（针对此项目）
    @author Ma Xiao
    @date 2012.4.4
    */


    QMap<QString,QObject *> plugins_;
};
}// namespace Web
#endif // WEBPAGE_H
