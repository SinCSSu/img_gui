
#include "Img.h"


#include <cmath>
using namespace std;

Img::Img()
{
    header = new BITMAPFILEHEADER;
    info = new BITMAPINFOHEADER;
}


bool Img::read(string path)
{
    ifstream img;
    img.open(path, ios::in | ios::binary);
    if (img)
    {
        img.read((char*)(header), sizeof(BITMAPFILEHEADER));
        img.read((char*)(info), sizeof(BITMAPINFOHEADER));
        //cout << info->biSize << ' ' << info->biWidth << ' ' << info->biHeight <<' '<< info->biBitCount;
        if (info->biBitCount == 8)
        {
            info->biClrUsed = (int)pow(2, info->biBitCount);
            colormap = new RGBQUAD[info->biClrUsed];
            img.read((char*)colormap, sizeof(RGBQUAD) * info->biClrUsed);
        }
        //cout << info->biBitCount << ' ' << info->biClrUsed;
        byteInLine = (((int)info->biWidth * (int)info->biBitCount) / 8 + 3) / 4 * 4;
        //cout << byteInLine << ' ' << info->biWidth * info->biBitCount;
        pixmap = new unsigned char[info->biHeight * byteInLine];
        memset(pixmap, 0, info->biHeight * byteInLine);
        img.read((char*)pixmap, info->biHeight * byteInLine);
        img.close();
        readed = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool Img::save(string path)
{
    ofstream out_img;
    out_img.open(path, ios::out | ios::binary);
    if (out_img)
    {
        out_img.write((char*)header, sizeof(BITMAPFILEHEADER));
        out_img.write((char*)info, sizeof(BITMAPINFOHEADER));
        if (colormap)
        {
            out_img.write((char*)colormap, sizeof(RGBQUAD) * info->biClrUsed);
        }
        out_img.write((char*)pixmap, info->biHeight * byteInLine);
        out_img.close();
        return true;
    }
    else
    {
        return false;
    }
}
void Img::show()
{
    cout << "图片宽：" << info->biWidth << endl
         << "图片高：" << info->biHeight << endl
         << "图片位数：" << info->biBitCount << endl;
    cout << "图片前10行10列数据为：" << endl;
    if (info->biHeight > 10)
    {
        if (info->biBitCount == 8)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    int pos = (info->biHeight - i - 1) * byteInLine + j;
                    cout << '(' << (int)colormap[pixmap[pos]].rgbRed << "," << (int)colormap[pixmap[pos]].rgbGreen << ',' << (int)colormap[pixmap[pos]].rgbBlue << ")" << ' ';
                }
                cout << endl;
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    int pos;
                    pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                    cout << '(' << (int)pixmap[pos + 2] << "," << (int)pixmap[pos + 1] << ',' << (int)pixmap[pos] << ")" << ' ';
                }
                cout << endl;
            }
        }
    }
}

void Img::getHistogram()
{
    int his[256] = { 0 };
    if (info->biBitCount == 8)
    {
        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos = (info->biHeight - i - 1) * byteInLine + j;
                int gray = (int)(colormap[pixmap[pos]].rgbRed * 19595 + colormap[pixmap[pos]].rgbGreen * 38469 + colormap[pixmap[pos]].rgbBlue * 7472) >> 16;
                his[gray] ++;
                //cout << gray << endl;
            }
        }
    }
    else
    {
        cout << "请选择通道：\n1——R 2——G 3——B 4——Gary" << endl;
        int choice;
        cin >> choice;
        switch (choice)
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
        int sum = 0;
        for (int i = 0; i < 256; i++)
        {
            cout << his[i] << ' ';
            sum += his[i];
        }
        cout << endl;
        float his_f[256] = { 0 };
        float max = 0;
        for (int i = 0; i < 256; i++)
        {
            his_f[i] = his[i] / (float)sum;
            if (his_f[i] > max)
            {
                max = his_f[i];
            }
        }
        for (int i = 0; i < 256; i++)
        {
            if (his[i] != 0)
            {
                cout << i << ":";
                for (float j = 0; j < his_f[i]; j += max / 50.0)
                {
                    cout << '*';
                }
                cout << endl;
            }
        }

    }

}

Img * Img::adjust(int l)
{
    Img* temp = new Img;
    temp->byteInLine = byteInLine;
    temp->header = this->header;
    temp->info = this->info;
    temp->pixmap = new unsigned char[info->biHeight * byteInLine];

    memset(temp->pixmap, 0, info->biHeight * byteInLine);
    if (info->biBitCount == 8)
    {
        temp->colormap = new RGBQUAD[info->biClrUsed];
        RGBpix* map = new RGBpix[info->biHeight * byteInLine];
        vector<RGBpix> sum;
        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos = (info->biHeight - i - 1) * byteInLine + j;
                unsigned char* yuv;
                unsigned char* rgb;
                yuv = getYUV(colormap[pixmap[pos]].rgbRed, colormap[pixmap[pos]].rgbGreen, colormap[pixmap[pos]].rgbBlue);
                if (yuv[0] + l > 235)
                {
                    yuv[0] = 235;
                }
                else if (yuv[0] + l < 16)
                {
                    yuv[0] = 16;
                }
                else
                {
                    yuv[0] += l;
                }

                rgb = getRGB(yuv[0], yuv[1], yuv[2]);
                map[pos].r = rgb[0];
                map[pos].g = rgb[1];
                map[pos].b = rgb[2];
                auto it = find(sum.begin(), sum.end(), RGBpix(rgb[0], rgb[1], rgb[2]));
                if (it != sum.end())
                {
                    (*it).usedcount++;
                }
                else
                {
                    sum.push_back(RGBpix(rgb[0], rgb[1], rgb[2]));
                }
                delete yuv;
                delete rgb;
            }
        }

        sort(sum.begin(), sum.end());

        for (int i = sum.size(); i < 256; i++)
        {
            sum.push_back(RGBpix(0xff, 0xff, 0xff));
        }

        for (int i = 0; i < 256; i++)
        {
            temp->colormap[i].rgbRed = sum[i].r;
            temp->colormap[i].rgbGreen = sum[i].g;
            temp->colormap[i].rgbBlue = sum[i].b;
        }

        vector<RGBpix> colorused(sum.begin(), sum.begin() + 256);

        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos = (info->biHeight - i - 1) * byteInLine + j;
                auto it = find(colorused.begin(), colorused.end(), RGBpix(map[pos].r, map[pos].g, map[pos].b));
                if (it != colorused.end())
                {
                    temp->pixmap[pos] = it - colorused.begin();
                }
                else
                {
                    int dis;
                    int min = 0xffffff;
                    int use;
                    for (int i = 0; i < 256; i++)
                    {
                        dis = pow(colorused[i].r - map[pos].r, 2) + pow(colorused[i].g - map[pos].g, 2) + pow(colorused[i].b - map[pos].b, 2);
                        if (dis < min)
                        {
                            min = dis;
                            use = i;
                        }
                    }
                    temp->pixmap[pos] = i;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                unsigned char* rgb;
                unsigned char* yuv;
                yuv = getYUV(pixmap[pos + 2], pixmap[pos + 1], pixmap[pos]);

                if (yuv[0] + l > 235)
                {
                    yuv[0] = 235;
                }
                else if (yuv[0] + l < 16)
                {
                    yuv[0] = 16;
                }
                else
                {
                    yuv[0] += l;
                }
                rgb = getRGB(yuv[0], yuv[1], yuv[2]);
                temp->pixmap[pos] = rgb[2];
                temp->pixmap[pos + 1] = rgb[1];
                temp->pixmap[pos + 2] = rgb[0];
            }
        }
    }
    return temp;
}

unsigned char* Img::getYUV(int r, int g, int b)
{
    unsigned char Y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
    unsigned char U = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
    unsigned char V = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
    unsigned char* yuv = new unsigned char[3];
    yuv[0] = Y;
    yuv[1] = U;
    yuv[2] = V;
    return yuv;
}

unsigned char* Img::getRGB(int y, int u, int v)
{
    unsigned char* rgb = new unsigned char[3];
    int raw[3] = { 0 };
    int C = y - 16;
    int	D = u - 128;
    int	E = v - 128;
    raw[0] = (298 * C + 409 * E + 128) >> 8;
    raw[1] = (298 * C - 100 * D - 208 * E + 128) >> 8;
    raw[2] = (298 * C + 516 * D + 128) >> 8;
    for (int i = 0; i < 3; i++)
    {
        if (raw[i] > 255)
        {
            raw[i] = 255;
        }
        else if (raw[i] < 0)
        {
            raw[i] = 0;
        }
    }
    rgb[0] = raw[0];
    rgb[1] = raw[1];
    rgb[2] = raw[2];
    return rgb;
}

Img * Img::invertColor()
{

    Img* temp = new Img;
    temp->byteInLine = byteInLine;
    temp->header = this->header;
    temp->info = this->info;
    temp->pixmap = new unsigned char[info->biHeight * byteInLine];
    memcpy(temp->pixmap, pixmap, info->biHeight * byteInLine);

    if (info->biBitCount == 8)
    {
        temp->colormap = new RGBQUAD[256];
        for (int i = 0; i < 256; i++)
        {
            temp->colormap[i].rgbRed = 255 - colormap[i].rgbRed;
            temp->colormap[i].rgbGreen = 255 - colormap[i].rgbGreen;
            temp->colormap[i].rgbBlue = 255 - colormap[i].rgbBlue;
        }
    }
    else
    {
        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                temp->pixmap[pos] = 255 - temp->pixmap[pos];
                temp->pixmap[pos + 1] = 255 - temp->pixmap[pos + 1];
                temp->pixmap[pos + 2] = 255 - temp->pixmap[pos + 2];
            }
        }
    }
    return temp;
}

Img * Img::toGray()
{
    Img* temp = new Img;
    temp->byteInLine = byteInLine;
    temp->header = this->header;
    temp->info = this->info;
    temp->pixmap = new unsigned char[info->biHeight * byteInLine];
    memcpy(temp->pixmap, pixmap, info->biHeight * byteInLine);

    if (info->biBitCount == 8)
    {
        temp->colormap = new RGBQUAD[256];

        for (int i = 0; i < 256; i++)
        {
            unsigned char gray = colormap[i].rgbRed * 19595 + colormap[i].rgbGreen * 38469 + colormap[i].rgbBlue * 7472 >> 16;
            temp->colormap[i].rgbRed = gray;
            temp->colormap[i].rgbGreen = gray;
            temp->colormap[i].rgbBlue = gray;
        }
    }
    else
    {
        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {

                int pos = (info->biHeight - i - 1) * byteInLine + j * 3;
                unsigned char gray = (pixmap[pos + 2] * 19595 + pixmap[pos + 1] * 38469 + pixmap[pos] * 7472) >> 16;
                temp->pixmap[pos] = gray;
                temp->pixmap[pos + 1] = gray;
                temp->pixmap[pos + 2] = gray;
            }
        }
    }
    return temp;
}

void Img::showColorMap()
{
    if (info->biBitCount == 24)
    {
        cout << "该图无调色板" << endl;
    }
    else
    {
        for (int i = 0; i < 256; i++)
        {
            cout << "[" << i << "](" << (int)colormap[i].rgbRed << "," << (int)colormap[i].rgbGreen << "," << (int)colormap[i].rgbBlue << ")" << endl;
        }
    }
}

Img * Img::trueColorto_8_Color()
{
    if (info->biBitCount == 24)
    {
        cout << "处理中，请稍后..." << endl;
        Img* temp = new Img;
        temp->header = new BITMAPFILEHEADER(*header);

        temp->header->bfOffBits = 14 + 40 + 256 * sizeof(RGBQUAD);



        temp->info = new BITMAPINFOHEADER(*info);
        temp->info->biBitCount = 8;
        temp->info->biClrUsed = 256;
        temp->info->biClrImportant = 256;

        temp->byteInLine = (((int)temp->info->biWidth * (int)temp->info->biBitCount) / 8 + 3) / 4 * 4;

        temp->header->bfSize = 14 + 40 + 256 * sizeof(RGBQUAD) + info->biHeight * temp->byteInLine;

        temp->colormap = new RGBQUAD[256];
        temp->pixmap = new unsigned char[temp->info->biHeight * temp->byteInLine];
        memset(temp->pixmap, 0, temp->info->biHeight * temp->byteInLine);




        RGBpix* map = new RGBpix[info->biHeight * temp->byteInLine];
        OctTree octtree;


        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos_raw = (info->biHeight - i - 1) * byteInLine + j * 3;
                int pos_now = (info->biHeight - i - 1) * temp->byteInLine + j;

                map[pos_now].r = pixmap[pos_raw + 2];
                map[pos_now].g = pixmap[pos_raw + 1];
                map[pos_now].b = pixmap[pos_raw];
                octtree.insert(map[pos_now]);
                while (octtree.leaves_num > 256)
                {
                    octtree.reduceTree();
                }
            }
        }


        RGBpix* pattle = octtree.getPattle();
        for (int i = 0; i < 256; i++)
        {
            temp->colormap[i].rgbRed = pattle[i].r;
            temp->colormap[i].rgbGreen = pattle[i].g;
            temp->colormap[i].rgbBlue = pattle[i].b;
        }
        vector<RGBpix> pat;
        for (int i = 0; i < 256; i++)
        {
            pat.push_back(pattle[i]);
        }
        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos_raw = (info->biHeight - i - 1) * byteInLine + j * 3;
                int pos_now = (info->biHeight - i - 1) * temp->byteInLine + j;

                auto it = find(pat.begin(), pat.end(), octtree.getPattelPos(map[pos_now]));
                temp->pixmap[pos_now] = it - pat.begin();

            }
        }
        return temp;
    }
    else
    {
        cout << "错误！该图片不为24位图片" << endl;
    }
}

Img* Img::trueColorto_8_Gray()
{
    if (info->biBitCount == 24)
    {
        Img* temp = new Img;
        temp->header = new BITMAPFILEHEADER(*header);

        temp->header->bfOffBits = 14 + 40 + 256 * sizeof(RGBQUAD);



        temp->info = new BITMAPINFOHEADER(*info);
        temp->info->biBitCount = 8;
        temp->info->biClrUsed = 256;
        temp->info->biClrImportant = 256;

        temp->byteInLine = (((int)temp->info->biWidth * (int)temp->info->biBitCount) / 8 + 3) / 4 * 4;

        temp->header->bfSize = 14 + 40 + 256 * sizeof(RGBQUAD) + info->biHeight * temp->byteInLine;

        temp->colormap = new RGBQUAD[256];
        temp->pixmap = new unsigned char[temp->info->biHeight * temp->byteInLine];
        memset(temp->pixmap, 0, temp->info->biHeight * temp->byteInLine);

        for (int i = 0; i < 256; i++)
        {
            temp->colormap[i].rgbRed = i;
            temp->colormap[i].rgbGreen = i;
            temp->colormap[i].rgbBlue = i;
        }

        for (int i = 0; i < info->biHeight; i++)
        {
            for (int j = 0; j < info->biWidth; j++)
            {
                int pos_raw = (info->biHeight - i - 1) * byteInLine + j * 3;
                int pos_now = (info->biHeight - i - 1) * temp->byteInLine + j;

                int gray = (int)(pixmap[pos_raw + 2] * 19595 + pixmap[pos_raw + 1] * 38469 + pixmap[pos_raw] * 7472) >> 16;
                temp->pixmap[pos_now] = gray;
            }
        }
    return temp;
    }
    else
    {
        cout << "错误！该图片不是24位图片" << endl;
    }

}

int* OctTree::getPos(int r, int g, int b)
{
    int* pos = new int[8];
    for (int i = 0; i < 8; i++)
    {
        pos[7 - i] = ((r >> i) & 1) * 4 + ((g >> i) & 1) * 2 + ((b >> i) & 1);
    }
    return pos;
}

void OctTree::insert(RGBpix& pix)
{
    int* pos;

    pos = getPos(pix.r, pix.g, pix.b);

    _insert(0, root, pix.r, pix.g, pix.b, pos);

    delete pos;
}

void OctTree::_insert(int level, OctNode* now, int r, int g, int b, int* pos)
{
    if (now->bIsLeaf)
    {
        now->nRedSum += r;
        now->nGreenSum += g;
        now->nBlueSum += b;
        now->nPixelCount++;
    }
    else
    {
        if (now->pChild[*pos] == NULL)
        {
            now->pChild[*pos] = new OctNode;
            now->pChild[*pos]->pParent = now;
            now->nchildSum++;
            lay[level].push_back(now->pChild[*pos]);
            if (level == 7)
            {
                now->pChild[*pos]->bIsLeaf = true;
                now->pChild[*pos]->reducible = true;
                leaves_num++;
            }
        }
        if (now->pChild[*pos] == NULL)
        {
            cout << "error" << endl;
            return;
        }
        _insert(level + 1, now->pChild[*pos], r, g, b, pos + 1);
    }
}

void OctTree::reduceTree()
{
    OctNode* parent = NULL;
    int min = INT_MAX;
    int w = 6;
    do
    {
        for (auto it = lay[w].begin(); it != lay[w].end(); it++)
        {
            int num = 0;
            int sum = 0;
            for (int i = 0; i < 8; i++)
            {
                if ((*it)->pChild[i] != NULL)
                {
                    if ((*it)->pChild[i]->bIsLeaf)
                    {
                        sum += (*it)->pChild[i]->nPixelCount;
                    }
                }
            }
            if ((*it)->nchildSum > 1 && sum < min)
            {
                min = sum;
                parent = (*it);
            }
        }
        if (!parent)
        {
            for (auto it = lay[w].begin(); it != lay[w].end(); it++)
            {
                int num = 0;
                int sum = 0;
                for (int i = 0; i < 8; i++)
                {
                    if ((*it)->pChild[i] != NULL)
                    {
                        if ((*it)->pChild[i]->bIsLeaf)
                        {
                            sum += (*it)->pChild[i]->nPixelCount;
                        }
                    }
                }
                if ((*it)->nchildSum > 0 && sum < min)
                {
                    min = sum;
                    parent = (*it);
                }
            }
        }
        w--;
    } while (!parent);
    int sum = 0;
    if (parent)
    {
        parent->nchildSum = 0;
        for (int i = 0; i < 8; i++)
        {
            if (parent->pChild[i] != NULL)
            {
                if (parent->pChild[i]->bIsLeaf)
                {
                    sum++;
                    parent->nBlueSum += parent->pChild[i]->nBlueSum;
                    parent->nRedSum += parent->pChild[i]->nRedSum;
                    parent->nGreenSum += parent->pChild[i]->nGreenSum;
                    parent->nPixelCount += parent->pChild[i]->nPixelCount;
                    lay[w + 2].remove(parent->pChild[i]);
                    delete parent->pChild[i];
                    parent->pChild[i] = NULL;
                }
            }
        }
        parent->bIsLeaf = true;
        leaves_num -= (sum - 1);
    }
    else
    {
        cout << "no find" << endl;
    }
}

OctTree::OctTree()
{
    root = new OctNode;
    for (int i = 0; i < 8; i++)
    {
        lay.push_back(list<OctNode*>());
    }
}

RGBpix* OctTree::getPattle()
{
    RGBpix* pattle = new RGBpix[256];
    int j = 0;
    for (int i = 0; i < 8; i++)
    {
        for (auto it = lay[i].begin(); it != lay[i].end(); it++)
        {
            if ((*it)->bIsLeaf)
            {
                pattle[j].r = (*it)->nRedSum / (*it)->nPixelCount;
                pattle[j].g = (*it)->nGreenSum / (*it)->nPixelCount;
                pattle[j].b = (*it)->nBlueSum / (*it)->nPixelCount;
                j++;
            }
        }
    }
    return pattle;
}
RGBpix OctTree::getPattelPos(RGBpix pix)
{
    int* pos = getPos(pix.r, pix.g, pix.b);
    OctNode* node = root;
    int i = 0;
    while (1)
    {
        if (node->bIsLeaf)
        {
            return RGBpix(node->nRedSum / node->nPixelCount, node->nGreenSum / node->nPixelCount, node->nBlueSum / node->nPixelCount);
        }
        else
        {
            node = node->pChild[pos[i]];
            i++;
        }
    }
}
