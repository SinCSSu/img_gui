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

    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setVisible(false);

    show_now = new ImgShow;

    action();
}

void MainWindow::show_img(ImgShow *img)
{
    QByteArray data = img->formatPix();
    QImage img_raw((unsigned char *)data.constData(),img->info->biWidth,img->info->biHeight,QImage::Format_ARGB32_Premultiplied);
    QPixmap pix = QPixmap::fromImage(img_raw);
    pix = pix.scaled(1024,1024,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label->setPixmap(pix);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(4);
    QStringList header;
    header << "key" << "value";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    QTableWidgetItem *temp = new QTableWidgetItem;
    temp->setText("位图大小");
    ui->tableWidget->setItem(0,0,temp);
    temp = new QTableWidgetItem;
    temp->setText(QString::number(img->header->bfSize));
    ui->tableWidget->setItem(0,1,temp);

    temp = new QTableWidgetItem;
    temp->setText("位图宽");
    ui->tableWidget->setItem(1,0,temp);
    temp = new QTableWidgetItem;
    temp->setText(QString::number(img->info->biWidth));
    ui->tableWidget->setItem(1,1,temp);

    temp = new QTableWidgetItem;
    temp->setText("位图高");
    ui->tableWidget->setItem(2,0,temp);
    temp = new QTableWidgetItem;
    temp->setText(QString::number(img->info->biHeight));
    ui->tableWidget->setItem(2,1,temp);

    temp = new QTableWidgetItem;
    temp->setText("位图位数");
    ui->tableWidget->setItem(3,0,temp);
    temp = new QTableWidgetItem;
    temp->setText(QString::number(img->info->biBitCount));
    ui->tableWidget->setItem(3,1,temp);
}

void MainWindow::open()
{
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
    if(open_path.size() == 0)
    {
        return;
    }
    if(!show_now->read(open_path.toStdString()))
    {
        QMessageBox::warning(NULL,"错误","打开失败");
        ui->label->clear();
    }
    additem("打开");
    show_img(show_now);
}

void MainWindow::save()
{
    if(changed)
    {
        QString save_path = QFileDialog::getSaveFileName(this,"请选择保存位置","","位图文件(.bmp)");
        if(save_path.size() == 0)
        {
            return;
        }
        if(show_now->save(save_path.toStdString()))
        {
            QMessageBox::information(NULL,"成功","保存成功！");
        }
        else
        {
            QMessageBox::warning(NULL,"错误","保存失败");
        }
        changed = true;
    }
    else
    {
        QMessageBox::information(NULL,"提示","暂无修改");
    }
}

void MainWindow::getHistogram(int channal)
{
    QMap<int,QString> map = {{0,"Red"},{1,"Green"},{2,"Blue"},{4,"RGB"}};
    QBarSet * temp = new QBarSet("灰度");
    int * his = show_now->GetHisArr(channal);
    for(int i = 0 ; i < 256;i++)
    {
        *temp << his[i];
        qDebug()<<his[i];
    }

    QBarSeries *series = new QBarSeries();
    series->append(temp);
    //series->setVisible(true);
    series->setBarWidth(1.2);

    QStringList categories;
    for(int i=0; i<256; i++)
        categories.push_back("");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);

    QValueAxis *axisY = new QValueAxis;
    //axisY->setLabelFormat("%d");
    axisY->setRange(0,1);

    QChart * chart = new QChart();
    QString title = "Histogram\tchannal " + map[channal];
    chart->setTitle(title);
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTheme(QChart::ChartThemeLight);
    chart->createDefaultAxes();

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);

    chart->setDropShadowEnabled(true);
    //    chart->legend()->setVisible(false);
    //    series->setLabelsVisible(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->resize(980,470);

    chartView->showMaximized();
}

void MainWindow::undo()
{
    if(undo_list.size()>0)
    {
        redo_list.push(show_now);
        show_now = undo_list.pop();
    }
}

void MainWindow::additem(QString mess)
{
    int row = ui->tableWidget_2->rowCount();
    qDebug() << row;
    QTableWidgetItem * item = new QTableWidgetItem(mess);
    ui->tableWidget_2->insertRow(0);
    ui->tableWidget_2->setItem(0,0,item);
    ui->tableWidget_2->setRowCount(row+1);
}

void MainWindow::getPattle()
{
    if(show_now->info->biBitCount == 8)
    {
        QWidget * pattle = new QWidget;
        QLabel * lable;
        int startX,startY;
        startX = 50;
        startY = 50;
        for(int i = 0;i < 256;i++)
        {
            int x = i % 16;
            int y = i / 16;
            int pos[2] = {startX + 50 * x,startY + 30*y};
            lable = new QLabel(pattle);
            lable->setGeometry(pos[0],pos[1],48,29);
            lable->setAutoFillBackground(true);
            QPalette* pale =new QPalette;
            pale->setColor(QPalette::Background,QColor(show_now->colormap[i].rgbRed,show_now->colormap[i].rgbGreen,show_now->colormap[i].rgbBlue));
            lable->setPalette(*pale);

        }
        pattle->resize(900,580);
    }
    else
    {
        QMessageBox::warning(NULL,"错误","该图无调色板");
    }
}

void MainWindow::action()
{
    connect(ui->actionopen_2,&QAction::triggered,this,&MainWindow::open);
    connect(ui->actionsave,&QAction::triggered,this,&MainWindow::save);

    connect(ui->actionR,&QAction::triggered,[=](){
        getHistogram(1);
    });
    connect(ui->actionG_channel,&QAction::triggered,[=](){
        getHistogram(2);
    });
    connect(ui->actionB_channel,&QAction::triggered,[=](){
        getHistogram(3);
    });
    connect(ui->actionGray,&QAction::triggered,[=](){
        getHistogram(4);
    });

    connect(ui->actionpattle,&QAction::triggered,this,&MainWindow::getPattle);
}
