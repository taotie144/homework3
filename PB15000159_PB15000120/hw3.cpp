#include <opencv2/opencv.hpp>
#include <cv.h>
#include <deque>
#include <ctime>

#define MAXN 3000
#define MY_OK 1
#define MY_FAIL -1

using namespace std;
using namespace cv;
//////////////////////////////////////////连通域检测////////////////////////////////////////////////
//功能说明：找到二值图像中所有的连通域，并且进行标记
//grayImg：用来进行连通域分析的目标图，8UC1
//labelImg：用来存储标记结果的图像，CV_32SC1，尺寸与grayImg一致
//bin_th：用来对灰度图进行二值化的阈值
//返回值：MY_OK或者MY_FAIL
int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th)
{
	int i, j, k;
	int label;
	int x, y;
	int row = grayImg.rows, col = grayImg.cols;
	int eight[8][2] = { { 0,-1 },{ -1,-1 },{ -1,0 },{ -1,1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };
	int temp, test;
	int ans[MAXN] = { 0 };
	static bool map[MAXN][MAXN];
	
	deque<int> queue;
	queue.clear();
	static bool is[MAXN];
	bool flag = true;
	uchar *pg;
	int *pl;
	labelImg.create(Size(col, row), CV_32SC1);
	memset(is, false, MAXN);
	memset(map, false, MAXN * MAXN);
	label = 1;
	for (i = 0;i < row;i++)
	{
		pg = grayImg.ptr<uchar>(i);
		pl = labelImg.ptr<int>(i);
		for (j = 0;j < col;j++)
		{
			if (pg[j] < bin_th)
			{
				continue;
			}
			temp = label;
			for (k = 0;k < 4;k++)
			{
				x = i + eight[k][0];
				y = j + eight[k][1];
				if (x >= 0 && y >= 0 && y < col)
				{
					if (grayImg.at<uchar>(x, y) < bin_th)
					{
						continue;
					}
					test = labelImg.at<int>(x, y);
					if (temp != label)
					{
						map[temp][test] = true;
						map[test][temp] = true;
					}
					if (test < temp)
					{
						temp = test;
					}
				}
			}
			if (temp == label)
			{
				label++;
				if (label > MAXN)
				{
					return MY_FAIL;
				}
			}
			pl[j] = temp;
		}
	}
	temp = 1;
	while (flag)
	{
		flag = false;
		for (i = 1;i < label;i++)
		{
			if (!is[i])
			{
				flag = true;
				break;
			}
		}
//		queue.clear();
		queue.push_back(i);
		is[i] = true;
		while (!queue.empty())
		{
			test = queue.front();
			queue.pop_front();
			ans[test] = temp;
			for (i = 1;i < label;i++)
			{
				if (!map[test][i] || is[i])
				{
					continue;
				}
				queue.push_back(i);
				is[i] = true;
			}
		}
		temp++;
	}
	for (i = 0;i < row;i++)
	{
		pl = labelImg.ptr<int>(i);
		for (j = 0;j < col;j++)
		{
			pl[j] = ans[pl[j]];
		}
	}
	return MY_OK;
}
