#ifndef JSOBJOS_H
#define JSOBJOS_H

#include "../../qt4.h"
namespace Web{
class JSOBJWindowx : public QObject
{
    Q_OBJECT
public:
    explicit JSOBJWindowx(QObject *parent = 0);

signals:

public slots:
    void showMaxSize();
    void showNormal();
    void showFullScreen();
    void quit();
    void closeChildWindow(const QString title);
    QString getChildWindow();

    void showAboutDialog(const QString &msg="这是关于...");
    void debug(const QString &msg);
    void log(const QString &msg);

};
}//namespace Web
#endif // JSOBJOS_H
