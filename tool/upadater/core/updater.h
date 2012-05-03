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
        ExtractPatchFileFailed,
        BadPatchFile,
        InstallPatchFiled
    };
    enum UpdateState
    {
        ConnectingToServer = 0,
        ConnectedToServer,
        DownloadingPatch,
        DownloadPatchFinished,
        InstallingPatch,
        CleaningPatchFiles,
        UpdateFinished
    };
      void downloadPatch();
signals:
    void fateError(const QString &);

    void downloading(int);
    void extractPatchFileStatus(bool);
    void errorOccurred(int);
    void updateStateChanged(int);
private slots:
    void ftpCommandFinished(int,bool);
    void ftpStateChanged(int);
    void downloadProgress(qint64,qint64);
private://私有成员函数
    void connectToPatchServer();
  //  void readHostIpFromSettingsFile();
    void parsePatchList(const QStringList &, const QString &);
     void ftpRest(qint64);

     QStringList extractPatchFile(QString &extr_dir);
     void installPatch();
     bool patchFileHasDownloaded();
    bool deletePatchDir(const QString &dirName);

    void downloadPatchPrivate();
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
