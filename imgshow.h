#ifndef IMGSHOW_H
#define IMGSHOW_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <Img.h>

class ImgShow : public Img
{
public:
    ImgShow();
    ImgShow(Img img);
    ~ImgShow(){};
    QByteArray formatPix();
    int * GetHisArr(int channal);

private:
};

#endif // IMGSHOW_H
