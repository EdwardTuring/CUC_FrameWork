#ifndef GUIDEDIALOG_H
#define GUIDEDIALOG_H

#include <QDialog>

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
    void quit();
protected:
    virtual void closeEvent(QCloseEvent *);
private slots:
    void on_btn_next__clicked();

    void on_btn_quit__clicked();

    void on_progressBar_valueChanged(int value);

    void on_btn_forward__clicked();

private:
    void toNextPage();
    void toForwardPage();

    Ui::GuideDialog *ui;
};

#endif // GUIDEDIALOG_H
