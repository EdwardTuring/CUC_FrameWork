#include <QtCore>
#include <QtGui>
#include <QApplication>
void setCodec();
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    setCodec();
    QMainWindow m;
    m.show();
    return app.exec();
}

void setCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}
