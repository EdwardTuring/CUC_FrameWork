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
    enum UpdateError
    {
        LoginInFtpServerFailed = 0,
        DownloadPatchFileFailed,
        CompressPatchFileFailed
    };
    enum UpdateState
    {
        ConnectingToServer = 0,
        ConnectedToServer
    };
      void downloadPatch();
signals:
    void fateError(const QString &);
    void downloading(qint64,qint64);
    void compressPatchFileStatus(bool);
    void errorOccurred(Updater::UpdateError);
    void updateStateChanged(Updater::UpdateState);
private slots:
    void ftpCommandFinished(int,bool);
    void ftpStateChanged(int);
    void downloadProgress(qint64,qint64);
private://私有成员函数
    void connectToPatchServer();
  //  void readHostIpFromSettingsFile();
    void parsePatchList();
     void ftpRest(qint64);

     void compressPatchFile();
private:
    QString patch_server_url_;
    QString patch_file_name_;
    QString patch_dir_;
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
