#ifndef COREERROR_H
#define COREERROR_H
#include <iostream>
#define FILE_OPEN_ERROR(file)  std::cerr<<"无法打开文件:"<<qPrintable((file).fileName()+" 错误代码："+QString::number((file).error())+"\n")<<endl
namespace CUCCore
{


}//namespace CUCError

#endif // COREERROR_H
