#ifndef REPODIALOG_H
#define REPODIALOG_H

#include <QDialog>
#include "web/webview.h"
namespace Ui {
class RepoDialog;
}

class RepoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RepoDialog(QWidget *parent = 0);
    ~RepoDialog();
    void setRepoUrl(QString url);
private:
    Ui::RepoDialog *ui;
    Web::WebView *webview_;

};

#endif // REPODIALOG_H
