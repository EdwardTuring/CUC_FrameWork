#ifndef QT4_H
#define QT4_H

#include <QtGui>
#include <QtCore>
/*
这个头文件定义了基本的宏

*/
#define CONNECT(a,b,c,d) connect(a,SIGNAL(b),c,SLOT(d))

#define MOVETOBROWSERCENTER(a) QWidget *mainwnd=BROWSER->getMainWindow();\
        a.move(mainwnd->x()+mainwnd->height()/2,mainwnd->y()+mainwnd->height()/2);//移动到主窗口的中间位置


#endif // QT4_H
