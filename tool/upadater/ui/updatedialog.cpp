#include "updatedialog.h"
#include "ui_updatedialog.h"
#include "tool.h"
UpdateDialog::UpdateDialog(const QString &host,
                           const QString &port,
                           const QString user_name,
                           const QString pwd,
                           const QString &patch_file_name,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{

    updater_= new CUCTool::Updater(host,port,user_name,pwd,patch_file_name,this);
    ui->setupUi(this);
    setFixedSize(600,414);
    ui->frame_download_->hide();

    ui->download_progress_->setMaximum(100);
    ui->download_progress_->setValue(0);
    flag_btn_getpatch_ = 0;
    connect(updater_ ,SIGNAL(downloading(int)),ui->download_progress_,SLOT(setValue(int)));
    connect(updater_, SIGNAL(updateStateChanged(int)),
            this,SLOT(setLabelStatus(int)));
    connect(updater_,SIGNAL(errorOccurred(int)),this,SLOT(handleUpdateError(int)));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}
void UpdateDialog::handleUpdateError(int state)
{
    switch(state)
    {
    case CUCTool::Updater::LoginInFtpServerFailed:

        information("�޷����ӵ����·����������������������á�");

        break;
    case CUCTool::Updater::DownloadPatchFileFailed:

        information("�޷����ز�����");

        break;


    default:break;

    }

}

void UpdateDialog::setLabelStatus(int state)
{
    switch(state)
    {
        case CUCTool::Updater::ConnectingToServer:

            ui->download_status_->setText("���ӵ����·�����...");
            break;

         case   CUCTool::Updater::ConnectedToServer:

             ui->download_status_->setText("���·��������ӳɹ�");
             break;

        case CUCTool::Updater::DownloadingPatch:

             ui->download_status_->setText("���ز���...");
             break;

       case CUCTool::Updater::DownloadPatchFinished:

            ui->download_status_->setText("�����������");
            break;

        case CUCTool::Updater::InstallingPatch:

            ui->download_status_->setText("��װ����...");
            break;
    case CUCTool::Updater::CleaningPatchFiles:

        ui->download_status_->setText("�������ļ�...");
        break;
    case CUCTool::Updater::UpdateFinished:

        ui->download_status_->setText("�������");
        flag_btn_getpatch_ = 1;
        ui->btn_getpatch_->setDisabled(false);
        ui->text_readme_->setText(updater_->getReadMe());
        break;
    default:break;

    }


}

void UpdateDialog::on_btn_getpatch__clicked()
{
    if(flag_btn_getpatch_ == 0)
    {
        ui->text_welcome_->hide();
        ui->frame_download_->show();
        ui->btn_getpatch_->setDisabled(true);

        updater_->downloadPatch();
         ui->btn_getpatch_->setText("���");
         return;
    }
    else if(flag_btn_getpatch_ == 1)
    {
        QApplication::exit(0);
    }
}
