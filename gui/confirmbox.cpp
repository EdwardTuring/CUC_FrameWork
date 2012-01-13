#include "confirmbox.h"
namespace UIC {
ConfirmBox::ConfirmBox(const QString &msg, bool *value_ptr,Icon icon, QWidget *parent):QMessageBox(parent)
{
   value_ptr_=value_ptr;
    setWindowTitle("操作确认");
   setIcon(QMessageBox::Question);
   setText(msg);
    btn_yes_= addButton("确认",QMessageBox::YesRole);
   btn_no_= addButton("取消",QMessageBox::NoRole);
   CONNECT(btn_yes_,clicked(),this,setValueTrue());
    CONNECT(btn_no_,clicked(),this,setValueFalse());
  setDefaultButton(btn_yes_);
}


void ConfirmBox::setValueFalse()
{
    *value_ptr_=false;
}
void ConfirmBox::setValueTrue()
{
    *value_ptr_=true;
}
} //namespace UIC
