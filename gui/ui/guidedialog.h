#ifndef GUIDEDIALOG_H
#define GUIDEDIALOG_H

#include <QDialog>
#include "core/networkmanager.h"
namespace Ui {
class GuideDialog;
}

class GuideDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GuideDialog(QWidget *parent = 0);
    ~GuideDialog();
signals:
    //������trueʱ��������źŵĽ����ߣ�ͨ����Browser�ࣩ����֤�Ѿ�ͨ��������ִ������Ĵ����ˣ�
    void quit(bool);
protected:
    virtual void closeEvent(QCloseEvent *);
private slots:
    void on_btn_next__clicked();

    void on_btn_quit__clicked();

    void on_progressBar_valueChanged(int value);

    void on_btn_forward__clicked();

    void slot_downloadProgress(qint64,qint64);
    void slot_httpProessFinished(QString, bool ispass);

    void on_http_prss_bar__valueChanged(int value);

private:
    void toNextPage();
    void toForwardPage();
    bool ispassed_;
    bool isfinishedget_;
    Ui::GuideDialog *ui;
    CUCCore::NetWorkManager *networkmanager_;

};

#endif // GUIDEDIALOG_H
