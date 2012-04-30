#include "popupwindow.h"
#include "browser.h"
extern UIC::Browser *BROWSER;
namespace UIC {
PopupWindow::PopupWindow(QWidget *parent,const QString title,const QUrl url) :
    MainWindow(url,title,parent)
{
    CONNECT(this,titleChanged(),this,changeTitle());

}
void PopupWindow::showMaxsize()
{
    this->toMaxSizeScreen();
}

void PopupWindow::closeEvent(QCloseEvent *event)
{


    delete this;

}
void PopupWindow::setTitle(const QString title)
{
    this->title_=title;
    emit titleChanged();
}

void PopupWindow::changeTitle()
{

    this->setWindowTitle(this->title_);
}
}//namespace UIC
