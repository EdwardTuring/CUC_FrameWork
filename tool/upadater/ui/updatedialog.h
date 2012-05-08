#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include "../core/updater.h"
namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(const QString &host,
                           const QString &port,
                           const QString user_name,
                           const QString pwd,const QString &patch_file_name,
                          const QString &main_pro_name,
                             const QString &version,
                             const QString &app_name,
                           QWidget *parent = 0);
    ~UpdateDialog();
    
private slots:
    void on_btn_getpatch__clicked();
private slots:
    void setLabelStatus(int);
    void handleUpdateError(int);

private:
    Ui::UpdateDialog *ui;
    CUCTool::Updater *updater_;
    QString main_pro_name_;
    unsigned char flag_btn_getpatch_;
};

#endif // UPDATEDIALOG_H
