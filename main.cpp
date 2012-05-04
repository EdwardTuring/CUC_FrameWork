

#include "qt4.h"

#include "browser.h"
#include "gui/splashscreen.h"
#include <QSettings>
#include "core/configparser.h"
#include "test/test.h"
using namespace UIC;



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
    app.setOrganizationName("CUC");
        app.setApplicationName("UICreator");
        app.setApplicationVersion("1.3");

    QTranslator translator;
     translator.load(":/qt_zh_CN.qm");
     app.installTranslator(&translator);
    QString sPath = app.applicationDirPath();
    sPath += QString("/plugins");
   app.addLibraryPath(sPath);


#ifdef CUC_TEST
Tst_NetWorkManager tn;
#else
   Browser b;


#endif

    return app.exec();
}
