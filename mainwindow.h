#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QStack>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
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

    bool changed = false;
    QStack<ImgShow *> undo_list;
    QStack<ImgShow *> redo_list;
    ImgShow *show_now;

};
#endif // MAINWINDOW_H
