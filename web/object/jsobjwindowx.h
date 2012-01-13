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
    void  refresh();
    QString getChildWindow();

};
}//namespace Web
#endif // JSOBJOS_H
