#ifndef QT4_H
#define QT4_H

#include <QtGui>
#include <QtCore>
/*
���ͷ�ļ������˻����ĺ�

*/
#define CONNECT(a,b,c,d) connect(a,SIGNAL(b),c,SLOT(d))

#define MOVETOBROWSERCENTER(a) QWidget *mainwnd=BROWSER->getMainWindow();\
        a.move(mainwnd->x()+mainwnd->height()/2,mainwnd->y()+mainwnd->height()/2);//�ƶ��������ڵ��м�λ��


#endif // QT4_H
