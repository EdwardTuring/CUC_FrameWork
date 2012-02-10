#include "repodialog.h"
#include "ui_repodialog.h"
#include <QUrl>
RepoDialog::RepoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepoDialog)
{
    ui->setupUi(this);
    this->webview_=new Web::WebView(this);
    ui->horizontalLayout->setMargin(0);
    ui->horizontalLayout->addWidget(webview_);
    setFixedSize(400,550);
}
void RepoDialog::setRepoUrl(QString url)
{
    webview_->load(QUrl(url));
    qDebug()<<"the repo is loaded";
}

RepoDialog::~RepoDialog()
{
    delete ui;
}
