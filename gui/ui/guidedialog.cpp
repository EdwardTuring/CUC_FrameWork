#include "guidedialog.h"
#include "ui_guidedialog.h"
#include "qt4.h"
#include "tool.h"
GuideDialog::GuideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuideDialog)
{
    ui->setupUi(this);
    ui->http_prss_bar_->hide();
    ui->btn_forward_->setEnabled(false);

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
    if(!(ui->ledt_verif_code_->text()==""))
    {
         toNextPage();
    }
    else
    {
        information("请输入验证信息");
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
        if(confirm("您现在正连接到服务器进行验证，若此时退出将可能导致您的\n验证失败。确认退出吗？"))
        {
            this->close();
           //向主进程发送退出信号。
            emit quit();
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
        emit quit();
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

    emit quit();
}

void GuideDialog::on_progressBar_valueChanged(int value)
{

}

void GuideDialog::on_btn_forward__clicked()
{

    if(confirm("您现在正连接到服务器进行验证，若此时退出将可能导致您的\n验证失败。确认退出吗？"))
    {
         toForwardPage();
    }
    else
    {
        //啥也不做
    }
}
