#ifndef JSOBJOS_H
#define JSOBJOS_H

#include "../../qt4.h"
namespace Web{
class JSOBJWindowx : public QObject
{
    Q_OBJECT
public:
    explicit JSOBJWindowx(QWidget *parent = 0);

signals:
    void closeMe(QWidget*);
public slots:
    void showMaxSize();
    void showNormal();
    void showFullScreen();
    void close();
    void closeChildWindow(const QString title);
    QString getChildWindow();

    void showAboutDialog(const QString &msg="这是关于...");
private:
    QWidget *parent_;
};
}//namespace Web
#endif // JSOBJOS_H
