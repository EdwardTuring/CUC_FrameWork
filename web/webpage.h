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
     *getPluginsFromDll:��dll��̬���ӿ��е���javascript���,��������
       ��Ա����QMapӳ�����
      @return void
      @author Ma Xiao
      @date 2012.4.2
    */
    void getPluginsFromDll();

private:

    JSOBJWindowx *windowx_;
    /**
    *���ڣ��ҽ�FtpPluginֱ��Ƕ��������ǣ�����Դ���Ŀ��
    @author Ma Xiao
    @date 2012.4.4
    */


    QMap<QString,QObject *> plugins_;
};
}// namespace Web
#endif // WEBPAGE_H
