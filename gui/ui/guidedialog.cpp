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
       * TODO:���ж��������֤��Ϣ�Ƿ�Ϸ���Ȼ���������bcont�������Ĳ�����
        ������ص�����ɣ��������ֲ�͹���ʵ�ֵķ��룩
    */
    if(!(ui->ledt_verif_code_->text()==""))
    {
         toNextPage();
    }
    else
    {
        information("��������֤��Ϣ");
    }
}

void GuideDialog::on_btn_quit__clicked()
{
    if(!(ui->http_prss_bar_->isHidden()))
    {
        /*
            ��http_prss_bar_����ʱ�����Ѿ���ʼ���ӷ������Ĳ�����ʱ�򣩣�
            ��ʾ�û��رմ˶Ի���ķ��ա�
        */
        if(confirm("�����������ӵ�������������֤������ʱ�˳������ܵ�������\n��֤ʧ�ܡ�ȷ���˳���"))
        {
            this->close();
           //�������̷����˳��źš�
            emit quit();
        }
        else
        {
            //ɶҲ����
        }

    }
    else
    {   //ֱ���˳�
        this->close();
       //�������̷����˳��źš�
        emit quit();
    }

}
void GuideDialog::toNextPage()
{
    ui->lbl_prompt_->setText("�������ӷ�����...");
    ui->ledt_verif_code_->hide();
    ui->btn_forward_->setEnabled(true);
    ui->btn_next_->setEnabled(false);
    ui->http_prss_bar_->show();
}
void GuideDialog::toForwardPage()
{
    ui->lbl_prompt_->setText("��������֤��Ϣ��");
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

    if(confirm("�����������ӵ�������������֤������ʱ�˳������ܵ�������\n��֤ʧ�ܡ�ȷ���˳���"))
    {
         toForwardPage();
    }
    else
    {
        //ɶҲ����
    }
}
