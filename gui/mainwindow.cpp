#include <QtGui>
#include <QtWebKit>
#include "mainwindow.h"

#include "qt4.h"
namespace UIC {
MainWindow::MainWindow(const QUrl& url,const QString &_title,QWidget *parent):QMainWindow(parent)
{
    progress_ = 0;
    title_=_title;
    wnd_inspector_=NULL;
 QNetworkProxyFactory::setUseSystemConfiguration(true);

    view_ = new WebView(this);
    view_->load(url);


    setActionsAndShortCuts();
    connect(view_, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view_, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));

    CONNECT(this,MaxSizeChanged(),this,setMaxSize());
    CONNECT(this,sizeChanged(),this,changeSize());
     CONNECT(this,posChanged(),this,changePos());


    availableGeometry = qApp->desktop()->availableGeometry(this);
    QSize size(800,600);
    this->normalsize_=size;
    QPoint p((qApp->desktop()->width() - this->width())/2,
             (qApp->desktop()->height() - this->height())/2);
    this->window_pos_=p;
    this->ismaxsize_=false;
    setCentralWidget(view_);
    setUnifiedTitleAndToolBarOnMac(true);

    childwnd_=new  QMap<QString,UIC::PopupWindow*>();

    //child widget init:
//    this->slidewidget_=new SlideWidget(this);
//    slidewidget_->move(0,100);
//    this->slidewidget_->show();
}



void MainWindow::adjustTitle()
{
    if (progress_ <= 0 || progress_ >= 100)
        setWindowTitle(this->title_);
    else
        setWindowTitle(QString("%1 (%2%)").arg(this->title_).arg(progress_));
}

void MainWindow::setProgress(int p)
{
    progress_ = p;
    adjustTitle();
}

void MainWindow::toFullScreen()
{

    if(!this->isFullScreen()){
    if(!this->ismaxsize_){
        this->normalsize_=this->size();
        QPoint p(pos().x(),pos().y()+30);
        this->window_pos_=p;
    }
    this->showFullScreen();

    }
}
void MainWindow::toNormalScreen()
{
    if(this->isFullScreen()){
    if(this->ismaxsize_)
    {
        move(this->window_pos_);
        this->resize(this->normalsize_);
        this->showMaximized();

    }
    else{
        this->ismaxsize_=false;
        move(this->window_pos_);
        this->resize(this->normalsize_);


    }
    }
}
void MainWindow::toFullOrNormalScreen()
{
    if(this->isFullScreen())
    {
        this->toNormalScreen();
    }
    else
    {
        this->toFullScreen();
    }
}

void MainWindow::showMaxSizeScreen()
{
    this->toMaxSizeScreen();
}

void MainWindow::toMaxSizeScreen()
{
    this->setWindowState(Qt::WindowMaximized);
    emit MaxSizeChanged();
}
void MainWindow::setMaxSize()
{
    this->ismaxsize_=true;
}

void MainWindow::setActionsAndShortCuts()
{

   sht_fullscreen_= new QShortcut(QKeySequence(tr("ESC")), this);
     sht_reload_= new QShortcut(QKeySequence(tr("F5")), this);
     sht_normalscreen_=new QShortcut(QKeySequence(tr("F11")),this);
     sht_inspector_=new QShortcut(QKeySequence(tr("Ctrl+Shift+i")),this);
    connect( sht_fullscreen_, SIGNAL( activated() ), this, SLOT( toNormalScreen() ) );
    CONNECT(sht_reload_,activated(),view_,reload());
     CONNECT(sht_normalscreen_,activated(),this,toFullOrNormalScreen());
        CONNECT(sht_inspector_,activated(),this,showInspector());
    view_->setContextMenuPolicy(Qt::NoContextMenu);
}


void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::WindowStateChange)
    {
        if(this->windowState()==Qt::WindowNoState){


            this->ismaxsize_=false;
        }
         if(this->windowState()==Qt::WindowMaximized)
             this->ismaxsize_=true;
    }


}

void MainWindow::Move(float x,float y)
{
    QMainWindow::move(x,y);
    emit posChanged();
}

void MainWindow::ReSize(float width,float height)
{
    QMainWindow::resize(width,height);
    emit sizeChanged();
}
void MainWindow::changeSize()
{
    this->normalsize_=size();
}

void MainWindow::changePos()
{
    window_pos_=this->pos();
}

void MainWindow::setTitle(const QString title)
{
    this->title_=title;

}

void MainWindow::showInspector()
{
    if(wnd_inspector_==NULL)
    {
        wnd_inspector_=new QMainWindow();
        wnd_inspector_->setCentralWidget(this->view()->getInspector());
    }
    wnd_inspector_->resize(800,600);
    wnd_inspector_->show();
}
}//namespace UIC
