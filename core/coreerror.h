#ifndef COREERROR_H
#define COREERROR_H
#include <iostream>
#define FILE_OPEN_ERROR(file)  std::cerr<<"�޷����ļ�:"<<qPrintable((file).fileName()+" ������룺"+QString::number((file).error())+"\n")<<endl
namespace CUCCore
{


}//namespace CUCError

#endif // COREERROR_H
