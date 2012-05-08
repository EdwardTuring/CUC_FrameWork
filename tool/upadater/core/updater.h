#ifndef UPDATER_H
#define UPDATER_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QFtp>


namespace CUCTool
{
class RunMainProThread:public QThread
{
    Q_OBJECT
public:
    RunMainProThread(const QStringList &,const QString &);
     void run();
private:
     QStringList arg_;
     QString app_name_;
};
class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(const QString &host,
                     const QString &port,
                     const QString user_name,
                     const QString pwd,const QString &patch_file_name,
                     const QString &main_pro_name,
                      const QString &version,
                      const QString &app_name,
                     QObject *parent);
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
     void  closeMainProcess();
     void runMainPro();
      QString getReadMe() const;
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
    void connectServer();
private://˽�г�Ա����
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
    QString app_name_;
    QString main_pro_name_;
    QString version_;
    int count_logintime_;//ͳ�Ƶ�¼�ĳ��Դ���
    QTimer *timer_connect_again_;
    QString text_readme_;//�����ĵ�
RunMainProThread *run_mainpro_thread_;


};
}//end namespace CUCTool
#endif // UPDATER_H
