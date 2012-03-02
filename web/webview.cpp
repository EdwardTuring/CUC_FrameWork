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
    this->page()->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);

    this->page()->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
     this->page()->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled,true);
    this->page()->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled,true);
   this->page()->settings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
  // this->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
   // QWebSettings::enablePersistentStorage();
    setContextMenuPolicy (Qt::NoContextMenu);

    //Ê¹ÓÃ»º´æ:
    QNetworkDiskCache *diskCache=new QNetworkDiskCache(this);
    QString location=QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    diskCache->setCacheDirectory(location);
    page()->networkAccessManager()->setCache(diskCache);


}
}//namespace Web




