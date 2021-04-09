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

int * ImgShow::GetHisArr(int channal)
{
    int * his = new int[256];
    memset(his,0,sizeof (int)*256);
    if (info->biBitCount == 8)
    {
        switch (channal)
        {
        case 1:
        {
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j;
                    his[colormap[pixmap[pos]].rgbRed] ++;
                }
            }
            break;
        }
        case 2:
        {
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j;
                    his[colormap[pixmap[pos]].rgbGreen] ++;
                }
            }
            break;
        }
        case 3:
        {
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j;
                    his[colormap[pixmap[pos]].rgbBlue] ++;
                }
            }
            break;
        }

        case 4:
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j;
                    int gray = (int)(colormap[pixmap[pos]].rgbRed * 19595 + colormap[pixmap[pos]].rgbGreen * 38469 + colormap[pixmap[pos]].rgbBlue * 7472) >> 16;
                    his[gray] ++;
                }
            }
            break;
        default:
            break;
        }
    }
    else
    {
        switch (channal)
        {
        case 1:
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                    int gray = (int)(pixmap[pos + 2]);
                    his[gray] ++;
                }
            }
            break;
        case 2:
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                    int gray = (int)(pixmap[pos + 1]);
                    his[gray] ++;
                }
            }
            break;
        case 3:
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                    int gray = (int)(pixmap[pos]);
                    his[gray] ++;
                }
            }
            break;
        case 4:
            for (int i = 0; i < info->biHeight; i++)
            {
                for (int j = 0; j < info->biWidth; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                    int gray = (int)(pixmap[pos + 2] * 19595 + pixmap[pos + 1] * 38469 + pixmap[pos] * 7472) >> 16;
                    his[gray] ++;
                }
            }
            break;
        default:
            break;
        }
    }
    return his;
}
