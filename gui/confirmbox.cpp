#include "confirmbox.h"
namespace UIC {
ConfirmBox::ConfirmBox(const QString &msg, bool *value_ptr,Icon icon, QWidget *parent):QMessageBox(parent)
{
   value_ptr_=value_ptr;
    setWindowTitle("����ȷ��");
   setIcon(QMessageBox::Question);
   setText(msg);
    btn_yes_= addButton("ȷ��",QMessageBox::YesRole);
   btn_no_= addButton("ȡ��",QMessageBox::NoRole);
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
