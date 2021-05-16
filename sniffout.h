#ifndef SNIFFOUT_H
#define SNIFFOUT_H

#include <QMainWindow>
#include <QDir>
#include "ui_sniffout.h"

namespace Ui {
class SniffOut;
}

class SniffOut : public QMainWindow
{
    Q_OBJECT

public:
    explicit SniffOut(QWidget *parent = 0);
    ~SniffOut();
    QFileInfoList getFilesRecursive(QDir *dir);
    QString open_document(QString);
    bool sniffing(QString text);
public slots:
    void onRead(QListWidgetItem *item);
    void setInfo(QString);
    void isDoneThread();
private slots:
    void on_openDialog_btn_clicked();

    void on_search_pushButton_clicked();

private:
    Ui::SniffOut *ui;
};

#endif // SNIFFOUT_H
