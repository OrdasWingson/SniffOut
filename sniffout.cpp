#include "sniffout.h"
#include "ui_sniffout.h"
#include "mythread.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <QAxObject>
#include <QDebug>
#include <QListWidget>
#include <QThread>

QString dirPath ="C:\\Users\\Иван\\Desktop\\ольга";

SniffOut::SniffOut(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SniffOut)
{
    ui->setupUi(this);
    ui->path_textEdit->setText(dirPath);
    ui->mask_textEdit->setText("*.*");
}

SniffOut::~SniffOut()
{
    delete ui;
}

void SniffOut::on_openDialog_btn_clicked()
{
    dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    dirPath,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->path_textEdit->setText(dirPath);
}

void SniffOut::on_search_pushButton_clicked()
{
    //open_document("C:\\Users\\Иван\\Desktop\\ольга\\Новая папка\\hell.docx");
    ui->search_pushButton->setEnabled(false);
    // Create an instance of your woker
    MyThread *workerThread = new MyThread(ui->path_textEdit->toPlainText(),ui->searchWord_textEdit->toPlainText());
        // Connect our signal and slot
    connect(workerThread, SIGNAL(showInformation(QString)),
                         SLOT(setInfo(QString)));
    connect(workerThread, SIGNAL(isDone()),
                         SLOT(isDoneThread()));
    connect(workerThread, SIGNAL(read(QListWidgetItem*)),
                         SLOT(onRead(QListWidgetItem*)));
        // Setup callback for cleanup when it finishes
    connect(workerThread, SIGNAL(finished()),
          workerThread, SLOT(deleteLater()));
        // Run, Forest, run!
    workerThread->start(); // This invokes WorkerThread::run in a new thread
}

QString SniffOut::open_document(QString filename)
{
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


 bool SniffOut::sniffing(QString text)
 {
     QString sWords = ui->searchWord_textEdit->toPlainText();
     QStringList  findWordsList = sWords.split(',');
     findWordsList.replaceInStrings(QRegExp("^\\s*"),"");
     QString findReg = '(' +findWordsList.join('|') + ')';
     QRegularExpression re(findReg);
     QString endStr = re.match(text).capturedTexts().join(' ');
     if(endStr != "")  return true;
     return false;

 }

 QFileInfoList SniffOut::getFilesRecursive(QDir *dir)
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


 void SniffOut::onRead(QListWidgetItem *item)
 {
     ui->files_listWidget->addItem(item);
 }

 void SniffOut::setInfo(QString name)
 {
     ui->count_label->setText(name);
 }

 void SniffOut::isDoneThread()
 {
     ui->search_pushButton->setEnabled(true);
     ui->count_label->setText("Готово");
 }


/*
   QStandardItemModel *model = new QStandardItemModel(this);
    ui->listView->setModel(model);
    model->appendRow(new QStandardItem(QIcon(":/img/img/eye.png"), "Test"));
----------------------------------------------------------------------------------
    QStringList files = dir->entryList(QStringList(ui->mask_textEdit->toPlainText()));
    foreach (QString file, files)
    {
        if(file == "." || file == "..")
        {
            continue;
        }

        item = new QListWidgetItem(QIcon(":/img/img/eye.png"),file);
        ui->files_listWidget->addItem(item);
        //open_document("C:\\Users\\Иван\\Desktop\\ольга\\2.docx");
    }
 */
 /*foreach (QFileInfo mIteTemp, dir->entryInfoList())
 {
     mIteTemp.fileName()
     QString strNewPathTemp = mIteTemp.absoluteFilePath();
     ui->files_listWidget->addItem(strNewPathTemp);
 }*/
