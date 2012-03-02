#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QWebInspector>
#include "webpage.h"
namespace Web
{


class WebView : public QWebView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);

    QWebInspector* getInspector() const;

protected:


public slots:
private:
    WebPage *page_;
    QWebInspector *inspector_;
};

}//namespace Web

#endif // WEBVIEW_H
