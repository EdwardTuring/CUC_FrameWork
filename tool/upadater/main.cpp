#include <QtCore>
#include <QtGui>
#include <QApplication>
#include "core/updater.h"
void setCodec();
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    setCodec();
   #ifdef _CUC_TEST_
    CUCTool::Updater updater("127.0.0.1","58021",
                             "updater","mx",
                             "mm_client_206.cucp");

    #endif
    return app.exec();
}

void setCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}
