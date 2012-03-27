#include "guidedialog.h"
#include "ui_guidedialog.h"
#include "qt4.h"
#include "tool.h"
#include "config.h"
GuideDialog::GuideDialog(QWidget *parent) :
    QDialog(parent),ispassed_(false),isfinishedget_(false),
    ui(new Ui::GuideDialog)
{
    ui->setupUi(this);
    ui->http_prss_bar_->hide();
    ui->btn_forward_->setEnabled(false);

    //初始化http连接模块的指针：
    networkmanager_=NULL;
}

GuideDialog::~GuideDialog()
{
    delete ui;
}

void GuideDialog::on_btn_next__clicked()
{
    /*
       * TODO:先判断输入的验证信息是否合法，然后进行连接bcont服务器的操作（
        调用相关的类完成，做到呈现层和功能实现的分离）
    */
    QString verf_code=ui->ledt_verif_code_->text();
    if(!(verf_code==""))
    {
        toNextPage();

        //下面开始连接到bcont服务器进行验证工作:

        //构建NetWorkManager对象。这里必须用new分配堆内存的原因是：http的get请求是另开的线程，
        //如果networkmanager_由操作系统自动分配在栈空间的话，会导致get线程未完成的情况下，networkmanager_
        //就已经被操作系统清空。
        networkmanager_=new CUCCore::NetWorkManager(BCONT_CUSTOMER_URL+verf_code,this);

        //在开始get请求之前，我们需要先做一些准备工作：
        CONNECT(networkmanager_,httpProessFinished(QString,bool),this,slot_httpProessFinished(QString,bool));
        CONNECT(networkmanager_,downloadProgress(qint64,qint64),this,slot_downloadProgress(qint64,qint64));
        //改变label的值：
        ui->lbl_prompt_->setText("正在验证...");
        //现在开始get请求：
        networkmanager_->get();

    }
    else
    {
        information("请输入验证信息");
    }
}

void GuideDialog::slot_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->http_prss_bar_->setMaximum(bytesTotal);
    ui->http_prss_bar_->setValue(bytesReceived);
}
void GuideDialog::slot_httpProessFinished(QString customer_name,bool ispass)
{
    isfinishedget_=true;
    if(ispass)
    {
        ui->lbl_prompt_->setText("验证成功");
        information("客户:"+customer_name+",验证通过");
        ispassed_=true;
        ui->btn_quit_->setText("确定");

    }
    else
    {
        ui->lbl_prompt_->setText("验证失败");

    }

}

void GuideDialog::on_btn_quit__clicked()
{
    if(!(ui->http_prss_bar_->isHidden()))
    {
        /*
            当http_prss_bar_出现时（即已经开始连接服务器的操作的时候），
            提示用户关闭此对话框的风险。
        */
        if(isfinishedget_)
        {
            //直接退出
            this->close();
            //向主进程发送退出信号。
            emit quit(ispassed_);
            return;
        }
        if(confirm("您现在正连接到服务器进行验证，若此时退出将可能导致您的\n验证失败。确认退出吗？"))
        {
            this->close();
            //向主进程发送退出信号。
            emit quit(ispassed_);
        }
        else
        {
            //啥也不做
        }

    }
    else
    {   //直接退出
        this->close();
        //向主进程发送退出信号。
        emit quit(ispassed_);
    }

}
void GuideDialog::toNextPage()
{
    ui->lbl_prompt_->setText("正在连接服务器...");
    ui->ledt_verif_code_->hide();
    ui->btn_forward_->setEnabled(true);
    ui->btn_next_->setEnabled(false);
    ui->http_prss_bar_->show();
}
void GuideDialog::toForwardPage()
{
    ui->lbl_prompt_->setText("请输入验证信息：");
    ui->http_prss_bar_->hide();
    ui->btn_forward_->setEnabled(false);
    ui->btn_next_->setEnabled(true);
    ui->ledt_verif_code_->show();
}

void GuideDialog::closeEvent(QCloseEvent *e)
{


    emit quit(ispassed_);
}

void GuideDialog::on_progressBar_valueChanged(int value)
{

}

void GuideDialog::on_btn_forward__clicked()
{

    if(isfinishedget_)
    {
        toForwardPage();
        return;
    }
    if(confirm("您现在正连接到服务器进行验证，若此时退出将可能导致您的\n验证失败。确认退出吗？"))
    {
        toForwardPage();
    }
    else
    {
        //啥也不做
    }
}

void GuideDialog::on_http_prss_bar__valueChanged(int value)
{
    //暂时留空
}
