#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include "qt4.h"
namespace UIC
{

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit SplashScreen(const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0);

signals:

public slots:

};
}//namespace UIC
#endif // SPLASHSCREEN_H
