#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    ui->label->setScaledContents(true);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList header;
    header << "key" << "value";
    ui->tableWidget->setHorizontalHeaderLabels(header);


    show_now = new ImgShow;

    connect(ui->actionopen_2,&QAction::triggered,[&](){
        if(changed)
        {
            QMessageBox::StandardButton result = QMessageBox::warning(NULL,"警告","仍有修改尚未保存，是否保存",QMessageBox::Save|QMessageBox::No|QMessageBox::Cancel);
            switch (result)
            {
            case QMessageBox::Save:
            {
                QString save_path = QFileDialog::getSaveFileName(this,"请选择保存路径","","位图文件(*.bmp)");
                show_now->save(save_path.toStdString());
                break;
            }
            case QMessageBox::No:
                break;
            case QMessageBox::Cancel:
                return;
                break;
            default:
                return;
            }
            for(auto it = undo_list.begin();it != undo_list.end();it++)
            {
                delete (*it);
            }
            for(auto it = redo_list.begin();it != redo_list.end();it++)
            {
                delete (*it);
            }
            undo_list.clear();
            redo_list.clear();
            delete show_now;
        }
        show_now = new ImgShow;
        QString open_path= QFileDialog::getOpenFileName(this,"选择打开的文件","","位图文件(*.bmp)");
        if(!show_now->read(open_path.toStdString()))
        {
            QMessageBox::warning(NULL,"错误","打开失败");
            ui->label->clear();
        }
        show_img(show_now);
    });
}


void MainWindow::show_img(ImgShow *img)
{
    QByteArray data = img->formatPix();
    QImage img_raw((unsigned char *)data.constData(),img->info->biWidth,img->info->biHeight,QImage::Format_ARGB32_Premultiplied);
    QPixmap pix = QPixmap::fromImage(img_raw);
    pix = pix.scaled(1024,1024,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label->setPixmap(pix);
    ui->tableWidget->clear();
    ui->listWidget->clear();
    QTableWidgetItem *temp = new QTableWidgetItem;
    temp->setText("位图大小");
    ui->tableWidget->setItem(0,0,temp);
    temp->setText(QString::number(img->info->biSize));
    ui->tableWidget->setItem(0,1,temp);
    temp->setText("位图宽");
    ui->tableWidget->setItem(1,0,temp);
    ui->tableWidget->setRowCount(2);
}
