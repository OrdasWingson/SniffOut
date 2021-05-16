#include "sniffout.h"
#include "ui_sniffout.h"
#include "mythread.h"
#include <shlobj.h>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QAxObject>
#include <QDebug>
#include <QListWidget>


MyThread::MyThread(QString dirPath, QString w) :
    directory(dirPath), words(w)
{
}

void MyThread::run()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    QString icon;
    QDir *dir = new QDir(directory);
    QListWidgetItem *item;
    QFileInfoList listFiles;
    listFiles.append(getFilesRecursive(dir));
    foreach (QFileInfo mIteTemp, listFiles)
    {
        if(sniffing(open_document(mIteTemp.absoluteFilePath())))
        {
           icon = ":/img/img/";
           icon.append(mIteTemp.completeSuffix());
           icon.append(".png");
           item = new QListWidgetItem(QIcon(icon),mIteTemp.fileName());
           read(item);
        }

    }
    isDone();
}


QString MyThread::open_document(QString filename)
{
    showInformation(filename);
    QString path = QDir::toNativeSeparators(filename);
    QAxObject wordApplication("Word.Application");
    QAxObject *documents = wordApplication.querySubObject("Documents");
    QAxObject *document = documents->querySubObject("Open(const QString&, bool)", path, true);
    QAxObject *words = document->querySubObject("Words");
    QString TextResume;
    int countWord = words->dynamicCall("Count()").toInt();
    for (int a = 1; a <= countWord; a++){
       TextResume.append(words->querySubObject("Item(int)", a)->dynamicCall("Text()").toString());
    }
    document->dynamicCall("Close (boolean)", false);
    //qDebug()<<TextResume;
    return TextResume;
}


 bool MyThread::sniffing(QString text)
 {
     QString sWords = words;
     QStringList  findWordsList = sWords.split(',');
     findWordsList.replaceInStrings(QRegExp("^\\s*"),"");
     QString findReg = '(' +findWordsList.join('|') + ')';
     QRegularExpression re(findReg);
     QString endStr = re.match(text).capturedTexts().join(' ');
     if(endStr != "")  return true;
     return false;

 }

 QFileInfoList MyThread::getFilesRecursive(QDir *dir)
 {
     QFileInfoList list;
     foreach (QFileInfo mIteTemp, dir->entryInfoList()) //QStringList(ui->mask_textEdit->toPlainText()
     {
         if(mIteTemp.fileName() == "." || mIteTemp.fileName() == "..")
         {
             continue;
         }
         if(mIteTemp.isDir())
         {
            QDir *newDir = new QDir(mIteTemp.absoluteFilePath());
            list.append(getFilesRecursive(newDir));
         }
         else
         {
            list.append(mIteTemp);
         }

     }
     return list;
 }
