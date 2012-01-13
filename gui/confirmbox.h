#ifndef CONFIRMBOX_H
#define CONFIRMBOX_H

#include "qt4.h"
namespace UIC
{
class ConfirmBox : public QMessageBox
{
    Q_OBJECT
public:
    ConfirmBox(const QString &msg,bool *value_ptr,Icon icon=QMessageBox::NoIcon,QWidget *parent=0);

signals:

public slots:
    void setValueTrue();
    void setValueFalse();
private:
    QPushButton *btn_yes_;
    QPushButton *btn_no_;

    bool *value_ptr_;

};
} //namespace UIC
#endif // CONFIRMBOX_H
