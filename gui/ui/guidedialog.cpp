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

    //��ʼ��http����ģ���ָ�룺
    networkmanager_=NULL;
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
    QString verf_code=ui->ledt_verif_code_->text();
    if(!(verf_code==""))
    {
        toNextPage();

        //���濪ʼ���ӵ�bcont������������֤����:

        //����NetWorkManager�������������new������ڴ��ԭ���ǣ�http��get�����������̣߳�
        //���networkmanager_�ɲ���ϵͳ�Զ�������ջ�ռ�Ļ����ᵼ��get�߳�δ��ɵ�����£�networkmanager_
        //���Ѿ�������ϵͳ��ա�
        networkmanager_=new CUCCore::NetWorkManager(BCONT_CUSTOMER_URL+verf_code,this);

        //�ڿ�ʼget����֮ǰ��������Ҫ����һЩ׼��������
        CONNECT(networkmanager_,httpProessFinished(QString,bool),this,slot_httpProessFinished(QString,bool));
        CONNECT(networkmanager_,downloadProgress(qint64,qint64),this,slot_downloadProgress(qint64,qint64));
        //�ı�label��ֵ��
        ui->lbl_prompt_->setText("������֤...");
        //���ڿ�ʼget����
        networkmanager_->get();

    }
    else
    {
        information("��������֤��Ϣ");
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
        ui->lbl_prompt_->setText("��֤�ɹ�");
        information("�ͻ�:"+customer_name+",��֤ͨ��");
        ispassed_=true;
        ui->btn_quit_->setText("ȷ��");

    }
    else
    {
        ui->lbl_prompt_->setText("��֤ʧ��");

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
        if(isfinishedget_)
        {
            //ֱ���˳�
            this->close();
            //�������̷����˳��źš�
            emit quit(ispassed_);
            return;
        }
        if(confirm("�����������ӵ�������������֤������ʱ�˳������ܵ�������\n��֤ʧ�ܡ�ȷ���˳���"))
        {
            this->close();
            //�������̷����˳��źš�
            emit quit(ispassed_);
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
        emit quit(ispassed_);
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
    if(confirm("�����������ӵ�������������֤������ʱ�˳������ܵ�������\n��֤ʧ�ܡ�ȷ���˳���"))
    {
        toForwardPage();
    }
    else
    {
        //ɶҲ����
    }
}

void GuideDialog::on_http_prss_bar__valueChanged(int value)
{
    //��ʱ����
}
