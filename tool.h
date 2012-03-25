#ifndef TOOL_H
#define TOOL_H
#include "qt4.h"
inline void information(const QString &text)
{
     QMessageBox::information(NULL,"ÌבÊ¾",text,QMessageBox::Close);
}

bool confirm(const QString &text);

inline  QString getCurrentDir()
{
    return qApp->applicationDirPath()+"/";
}


#endif // TOOL_H
