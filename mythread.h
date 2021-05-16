#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QDir>
#include "ui_sniffout.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QString dirPath, QString w);
    void run();
    QFileInfoList getFilesRecursive(QDir *dir);
    QString open_document(QString);
    bool sniffing(QString text);
signals:
    void read(QListWidgetItem*);
    void showInformation(QString);
    void isDone();
public slots:
private:
    QString directory;
    QString words;


};

#endif // MYTHREAD_H
