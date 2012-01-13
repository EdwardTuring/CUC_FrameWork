#include <QtGui>
#include <QtWebKit>
#include "mainwindow.h"

#include "qt4.h"
namespace UIC {
MainWindow::MainWindow(const QUrl& url,const QString &_title,QWidget *parent):QMainWindow(parent)
{
    progress_ = 0;
    title_=_title;
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

        this->window_pos_=this->pos();
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

void MainWindow::showMaxSizeScreen()
{
    this->toMaxSizeScreen();
}

void MainWindow::toMaxSizeScreen()
{
    this->showMaximized();
    emit MaxSizeChanged();
}
void MainWindow::setMaxSize()
{
    this->ismaxsize_=true;
}

void MainWindow::setActionsAndShortCuts()
{
    this->act_fullscreen_=new QAction("全屏显示",view_);
     this->act_normalscreen_=new QAction("退出全屏   (ESC)",view_);
     this->act_back_=new QAction("退后 ",view_);
    this->act_forward_=new QAction("向前",view_);
    this->act_reload_=new QAction("刷新    (F5)",view_);
    connect(act_fullscreen_, SIGNAL(triggered(bool)), this, SLOT(toFullScreen()));
    CONNECT(act_back_,triggered(bool),view_,back());
    CONNECT(act_forward_,triggered(bool),view_ ,forward());
    CONNECT(act_reload_,triggered(bool),view_,reload());
   sht_fullscreen_= new QShortcut(QKeySequence(tr("ESC")), this);
     sht_reload_= new QShortcut(QKeySequence(tr("F5")), this);
    connect( sht_fullscreen_, SIGNAL( activated() ), this, SLOT( toNormalScreen() ) );
    CONNECT(sht_reload_,activated(),view_,reload());
    connect(act_normalscreen_, SIGNAL(triggered(bool)), this, SLOT(toNormalScreen()));
    view_->addAction(act_fullscreen_);
    view_->addAction(act_normalscreen_);
    view_->addAction(act_reload_);
    view_->addAction(act_back_);
    view_->addAction(act_forward_);
    view_->setContextMenuPolicy(Qt::ActionsContextMenu);
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
}//namespace UIC
