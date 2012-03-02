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
    this->page_=new WebPage(this);
    this->setPage(page_);
    inspector_=new QWebInspector();
    inspector_->setPage(page_);
    this->page()->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);

    this->page()->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
     this->page()->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled,true);
    this->page()->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled,true);
    this->page()->settings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
     this->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
       QWebSettings::enablePersistentStorage();
    //使用缓存:
    QNetworkDiskCache *diskCache=new QNetworkDiskCache(this);
    QString location=QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    diskCache->setCacheDirectory(location);
    page()->networkAccessManager()->setCache(diskCache);

   // setContextMenuPolicy (Qt::NoContextMenu);//屏蔽原有右键菜单
}
QWebInspector *WebView::getInspector() const
{
    return this->inspector_;
}

}//namespace Web




