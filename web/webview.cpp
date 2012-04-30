#include "webview.h"
 #include <QMessageBox>
 #include <QNetworkDiskCache>
 #include <QDesktopServices>
#include <windows.h>
namespace Web
{
WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
    this->page_=new WebPage(this,parent);


    this->setPage(page_);
    inspector_=new QWebInspector();
    inspector_->setPage(page_);
    this->page()->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    this->page()->settings()->setAttribute(QWebSettings::PluginsEnabled,true);

    #ifdef _CUC_DEBUG_
    this->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
    QWebSettings::enablePersistentStorage();
    #endif
}
QWebInspector *WebView::getInspector() const
{
    return this->inspector_;
}

}//namespace Web




