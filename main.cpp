

#include "qt4.h"

#include "browser.h"
#include "gui/splashscreen.h"
#include <QSettings>
using namespace UIC;
Browser *BROWSER=NULL;


//���ñ����ʽ����ʾ����
void setCodec();



void setCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    setCodec();

    QString sPath = app.applicationDirPath();
    sPath += QString("/plugins");
   app.addLibraryPath(sPath);

   Browser b;

   BROWSER=&b;

    return app.exec();
}
