#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <Windows.h>
#include <vector>
#include<list>
#include <algorithm>
#include<limits>

//struct Header
//{
//	unsigned short bfType = 0x424d;
//	int bfSize = 0;
//	unsigned short bfReserved1 = 0;
//	unsigned short bfReserved2 = 0;
//	int bfOffBits = 0;
//};
//struct InfoHeader
//{
//	int biSize = 0x28;
//	int biWidth = 0;
//	int biHeight = 0;
//	unsigned short biPlanes = 1;
//	unsigned short biBiCount = 0;
//	int biCompression = 0;
//	int biSizeImage = 0;
//	int biXPelsPerMeter = 0;
//	int biYPelsPerMeter = 0;
//	int biCIrUsed = 0;
//	int biCIrImportant = 0;
//};
//struct ColorMap
//{
//	unsigned char** palette;
//};
//
//struct biData
//{
//
//};

class RGBpix 
{
public:
	RGBpix() {};
	RGBpix(unsigned char r, unsigned char g, unsigned char b) :
		r(r), g(g), b(b) {};
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	int usedcount = 0;
	//unsigned char pos = 0;
	bool operator<(const RGBpix& p2) { return this->usedcount < p2.usedcount; };
	bool operator==(const RGBpix& p2) {
		return this->r == p2.r && this->g == p2.g && this->b == p2.b;
	};
	
};

class OctNode
{
public:
	BOOL bIsLeaf = false;               // TRUE if node has no children （是否是叶子节点）
	bool reducible = false;
	UINT nPixelCount = 0;           // Number of pixels represented by this leaf （代表的像素数量）
	UINT nRedSum;               // Sum of red components
	UINT nGreenSum;             // Sum of green components
	UINT nBlueSum;              // Sum of blue components （B通道分量总和）
	OctNode* pChild[8] = {NULL};    // Pointers to child nodes  （子结点）
	OctNode* pParent;        // Pointer to next reducible node
	int nchildSum = 0;
	BYTE nColorIndex;             // 仅对叶子节点有效，表示此节点代表的颜色在调色板中的索引！存储到BMP文件时有用。
};


class OctTree
{
public:
	OctTree();
	void insert(RGBpix & color);
	void reduceTree();
	RGBpix* getPattle();
	int leaves_num = 0;
	RGBpix getPattelPos(RGBpix color);
private:
	void _insert(int level, OctNode* now, int r ,int g, int b, int* pos);
	int* getPos(int r, int g, int b);
	OctNode* root;
	std::vector<std::list<OctNode*>> lay;
};


class Img
{
public:
	Img();
	bool read(std::string path);
	bool save(std::string path);
	void show();
	void getHistogram();
    Img * adjust(int l);
    Img* invertColor();
    Img * toGray();
    void showColorMap();
    Img * trueColorto_8_Gray();
    Img * trueColorto_8_Color();
	BITMAPFILEHEADER *header;
	BITMAPINFOHEADER *info;
	RGBQUAD* colormap;
	unsigned char* pixmap;
	int byteInLine;
	bool readed = false;
protected:
	unsigned char* getYUV(int r, int g, int b);
	unsigned char* getRGB(int y, int u, int l);
	
};

