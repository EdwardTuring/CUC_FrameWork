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

        information("无法连接到更新服务器，请检查您的网络设置。");

        break;
    case CUCTool::Updater::DownloadPatchFileFailed:

        information("无法下载补丁。");

        break;


    default:break;

    }

}

void UpdateDialog::setLabelStatus(int state)
{
    switch(state)
    {
        case CUCTool::Updater::ConnectingToServer:

            ui->download_status_->setText("连接到更新服务器...");
            break;

         case   CUCTool::Updater::ConnectedToServer:

             ui->download_status_->setText("更新服务器连接成功");
             break;

        case CUCTool::Updater::DownloadingPatch:

             ui->download_status_->setText("下载补丁...");
             break;

       case CUCTool::Updater::DownloadPatchFinished:

            ui->download_status_->setText("补丁下载完成");
            break;

        case CUCTool::Updater::InstallingPatch:

            ui->download_status_->setText("安装补丁...");
            break;
    case CUCTool::Updater::CleaningPatchFiles:

        ui->download_status_->setText("清理补丁文件...");
        break;
    case CUCTool::Updater::UpdateFinished:

        ui->download_status_->setText("更新完成");
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
         ui->btn_getpatch_->setText("完成");
         return;
    }
    else if(flag_btn_getpatch_ == 1)
    {
        QApplication::exit(0);
    }
}
