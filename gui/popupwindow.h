#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "qt4.h"
#include "mainwindow.h"

namespace UIC
{
class MainWindow;
class PopupWindow: public MainWindow
{
    Q_OBJECT
public:
    PopupWindow(QWidget *parent=0,const QString title="UICreator",const QUrl url=QUrl(""));
    void showMaxsize();
    void setTitle(const QString title);
signals:
    void titleChanged();
protected:
    void closeEvent ( QCloseEvent * event ) ;

public slots:
     void changeTitle();
};
} //namespace UIC
#endif // POPUPWINDOW_H
