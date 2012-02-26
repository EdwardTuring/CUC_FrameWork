#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>
#include <QWebSettings>
#include "webpage.h"
namespace Web
{


class WebView : public QWebView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);

protected:


public slots:
private:
    WebPage *page_;
};

}//namespace Web

#endif // WEBVIEW_H
