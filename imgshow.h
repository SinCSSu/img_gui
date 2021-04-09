#ifndef IMGSHOW_H
#define IMGSHOW_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <Img.h>

namespace Ui {
class ImgShow;
}

class ImgShow : public QWidget,public Img
{
    Q_OBJECT

public:
    explicit ImgShow(QWidget *parent = nullptr);
    ~ImgShow();
    QByteArray formatPix();

private:
    Ui::ImgShow *ui;
};

#endif // IMGSHOW_H
