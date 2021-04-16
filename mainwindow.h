#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QMap>
#include <QPixmap>
#include <QStack>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QtCharts>
#include <QInputDialog>
#include "imgshow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    void init();
    void show_img(ImgShow *);
    void open();
    void save();
    void getHistogram(int channal);
    void undo();
    void redo();
    void additem(QString mess);
    void action();
    void getPattle();

    bool changed = false;
    QStack<ImgShow *> undo_list;
    QStack<ImgShow *> redo_list;
    ImgShow *show_now;

};
#endif // MAINWINDOW_H

