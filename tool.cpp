#include "tool.h"

bool confirm(const QString &text)
{
     int res=QMessageBox::information(NULL,"��ʾ",text,QMessageBox::Yes,QMessageBox::No);
     if(res==QMessageBox::Yes)
     {
         return true;
     }
     else
     {
        return false;
     }
}

