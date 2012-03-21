#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QWebPage>
#include <QtCore>
#include "object/jsobjwindowx.h"
#include "../plugin/CUCplugininterface.h"
namespace Web
{
class WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit WebPage(QWidget *parent = 0);

     explicit WebPage(QWidget *parent ,QWidget *mainwindow);

protected:
    void javaScriptAlert ( QWebFrame * frame, const QString & msg );
    bool javaScriptConfirm( QWebFrame * frame, const QString & msg );
    QWebPage *createWindow(WindowFeaturesQt feature);
public slots:
    void addJSOBJ();

private:

    JSOBJWindowx *windowx_;
    QVector<QObject *> *plugins;
};
}// namespace Web
#endif // WEBPAGE_H
