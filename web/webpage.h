#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QWebPage>
#include "object/jsobjwindowx.h"
namespace Web
{
class WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit WebPage(QWidget *parent = 0);

protected:
    void javaScriptAlert ( QWebFrame * frame, const QString & msg );
    bool javaScriptConfirm( QWebFrame * frame, const QString & msg );
    QWebPage *createWindow(WindowFeaturesQt feature);

public slots:
    void addJSOBJ();
private:
    JSOBJWindowx *windowx_;
};
}// namespace Web
#endif // WEBPAGE_H
