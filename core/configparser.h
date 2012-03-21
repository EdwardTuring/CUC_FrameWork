#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include "qt4.h"
#include "coreerror.h"
namespace CUCCore{
class PlatformSetting:public QObject{

    Q_OBJECT
public:
    /*
     *���캯�����������������Ϣ���������ӷ�������������ַ��
     �����ڵı��⡢��ʼ״̬�Ƿ���󻯺ͳ�ʼ���ڵĸ߶ȺͿ�ȣ�
     ������Ϊ���ʱ��������ᱻ���ԣ���
     */
    PlatformSetting(const QString &host_url,
                    const QString &title,
                    bool ismaxsize,
                    int width,
                    int height,
                    QObject *parent=0
                    );
    //PlatformSetting(const PlatformSetting &other);
    /*
     *����Ĳ����ܹ���ȡ������ĳ�Ա���������������ķ�ʽ���Ч�ʡ�
     */
    QString getHostUrl()    const{return host_url_;}
    QString getWindowTitle()    const{return title_;}
    bool getWindowISMaxsize()   const{return ismaxsize_;}
    int getWindowWidth()    const{return width_;}
    int getWindowHeight()   const{return height_;}
#ifdef CUC_TEST
    void tst_print() const{qDebug()<<"PlatformSetting::tst_print():��Ա������ֵ��"<<host_url_
                                  <<" "<<title_<<" "<<QString::number((int)ismaxsize_)
                                 <<" "<<QString::number(width_)
                                <<" "<<QString::number(height_);}
#endif
 private:
    QString host_url_;
    QString title_;
    bool ismaxsize_;
    int width_;
    int height_;
};

class ConfigParser:public QObject
{
    Q_OBJECT
public:
    ConfigParser(QObject *parent=0);
    PlatformSetting *getPlatformSetting()   const{return setting_;}

    enum  parserError
    {

        OPEN_SETTINGS_FILE_FAILD //�������ļ�ʧ��

    };
     bool parse();
#ifdef CUC_TEST
    void test();
#endif
signals:
    void fatalError(parserError);

protected:
    /*
      * pareserConfigFile()�����������������ļ������ļ������ɹ�����true���ļ������ڷ���false��
      ���ļ����ڵ�������޷��򿪣��򴥷���������
      */
    bool parserConfigFile();
private:
    PlatformSetting *setting_;


};

}

#endif // CONFIGPARSER_H
