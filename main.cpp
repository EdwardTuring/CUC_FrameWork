

#include "qt4.h"

#include "browser.h"
#include "gui/splashscreen.h"
#include <QSettings>
using namespace UIC;
Browser *BROWSER=NULL;


//设置编码格式以显示中文
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
 #ifndef CUC_TEST
    app.setOrganizationName("CUC");
    app.setApplicationName("Ftp_Client");
    app.setApplicationVersion("2.0");

    QTranslator translator;
     translator.load(":/qt_zh_CN.qm");
     app.installTranslator(&translator);
    QString sPath = app.applicationDirPath();
    sPath += QString("/plugins");
   app.addLibraryPath(sPath);

   Browser b;

   BROWSER=&b;
#else

#endif
    return app.exec();
}
