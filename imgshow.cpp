#include "imgshow.h"
#include "ui_imgshow.h"

ImgShow::ImgShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImgShow)
{
    ui->setupUi(this);
}

ImgShow::~ImgShow()
{
    delete ui;
}

QByteArray ImgShow::formatPix()
{
    QByteArray * data = new QByteArray(info->biHeight * info->biWidth * 4,0);
    if(info->biBitCount == 8)
    {
        for(int i = 0 ;i < info->biHeight;i++)
        {
            for(int j = 0; j < info->biWidth;j++)
            {
                int pos_raw = (info->biHeight - i - 1) * byteInLine + j;
                int pos_now = (i * info->biWidth+ j) * 4;
                (*data)[pos_now] = colormap[pixmap[pos_raw]].rgbRed;
                (*data)[pos_now +1] = colormap[pixmap[pos_raw]].rgbGreen;
                (*data)[pos_now+2] = colormap[pixmap[pos_raw]].rgbBlue;
                (*data)[pos_now+3] = ~0;
            }
        }
    }
    else
    {
        for(int i = 0 ;i < info->biHeight;i++)
        {
            for(int j = 0; j < info->biWidth;j++)
            {
                int pos_raw = (info->biHeight - i - 1) * byteInLine + j * 3;
                int pos_now = (i * info->biWidth + j) * 4;
                (*data)[pos_now] = pixmap[pos_raw];
                (*data)[pos_now +1] =pixmap[pos_raw+1];
                (*data)[pos_now+2] = pixmap[pos_raw + 2];
                (*data)[pos_now+3] = ~0;
            }
        }
    }
    return (*data);
}
