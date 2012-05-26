#include "tool.h"

bool confirm(const QString &text)
{
    int res=QMessageBox::information(NULL,"ÌבÊ¾",text,QMessageBox::Yes,QMessageBox::No);
    if(res==QMessageBox::Yes)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool deletePatchDir(const QString &dirName)
{
    QDir directory(dirName);

    if (!directory.exists())
    {
        return true;
    }

    QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);


    QList<QString>::iterator f = files.begin();

    bool error = false;

    for (; f != files.end(); ++f)
    {
        QString filePath = QDir::convertSeparators(directory.path() + '/' + (*f));
        QFileInfo fi(filePath);
        if (fi.isFile() || fi.isSymLink())
        {
            QFile::setPermissions(filePath, QFile::WriteOwner);
            if (!QFile::remove(filePath))
            {

                error = true;
            }
        }
        else if (fi.isDir())
        {
            if (!deletePatchDir(filePath));
            {
                error = true;
            }
        }
    }

    if(!directory.rmdir(QDir::convertSeparators(directory.path())))
    {

        error = true;
    }

    return !error;

}
