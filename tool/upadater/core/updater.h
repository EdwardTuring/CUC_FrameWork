#ifndef UPDATER_H
#define UPDATER_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QFtp>

namespace CUCTool
{
class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(const QString &host,
                     const QString &port,
                     const QString user_name,
                     const QString pwd,const QString &patch_file_name,
                     QObject *parent = 0);
    
signals:
    void fateError(const QString &);
private slots:
    void ftpCommandFinished(int,bool);
    void downloadProgress(qint64,qint64);
private://私有成员函数
    void connectToPatchServer();
  //  void readHostIpFromSettingsFile();

     void ftpRest(qint64);
     void downloadPatch();
private:
    QString patch_server_url_;
    QString patch_file_name_;
    QFtp *ftp_;
    QFile *patch_file_;
    QFile *download_pos_file_;
    qint64 download_pos_;
    QString host_ip_;
    int port_;
    QString user_name_;
    QString pwd_;

};
}//end namespace CUCTool
#endif // UPDATER_H
