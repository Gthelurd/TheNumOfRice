#include <iostream>  
#include <stack> 
#include <map> 
// #include <windows.h> 
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/opencv.hpp> 
#include <opencv2/imgproc.hpp> 
using namespace std;
using namespace cv;
 
 void areaLabeling(Mat &src, Mat&res)
 {
     if (src.empty() || src.type() != CV_8UC1)
     {
         cout << "图象未读取或者是多通道" << endl;
         return;
     }
     res.release();
     src.convertTo(res, CV_32SC1);//CV_32SC1，32位单通道
 
     int label = 1;//标记值
     int row = src.rows-1, col = src.cols-1;
 
     for (int i = 1; i < row - 1; i++)
     {
         int* data = res.ptr<int>(i);//获取一行值
         for (int j = 1; j < col - 1; j++)
         {
             if (data[j] == 1)//目标区域点:未被处理过
             {
                 //放置种子
                 stack<pair<int, int>> labelPixel;
                 labelPixel.push(pair<int, int>(i, j));
                 ++label;
                 printf("第 %d 颗种子入栈, 位置 ( %d , %d )\n", label - 1, i, j);
 
                 while (!labelPixel.empty())
                 {
                     pair<int, int> curPixel = labelPixel.top();
                     int x = curPixel.first;
                     int y = curPixel.second;
                     res.at<int>(x, y) = label;//种子标记
                     labelPixel.pop();
 
                     //领域像素位置入栈
                     if(res.at<int>(x, y-1) == 1)
                         labelPixel.push(pair<int, int>(x, y - 1));
                     if (res.at<int>(x, y+1) == 1)
                         labelPixel.push(pair<int, int>(x, y + 1));
                     if (res.at<int>(x-1, y) == 1)
                         labelPixel.push(pair<int, int>(x - 1, y));
                     if (res.at<int>(x+1, y) == 1)
                         labelPixel.push(pair<int, int>(x + 1, y));
                 }
             }
         }
     }
     //输出统计数字
     cout << "总计：" << label-1 << endl;
 }
 
 //生成彩色Scalar
 Scalar formatRandom()
 {
     uchar r = rand() % 255;
     uchar g = rand() % 255;
     uchar b = rand() % 255;
     return Scalar(r, g, b);
 }
 
 void Bin2BGR(Mat &src, Mat &res)
 {
     if (!src.data || src.type() != CV_32SC1)
     {
         cout << "图像错误" << endl;
         return ;
     }
     int row = src.rows;
     int col = src.cols;
     map<int, Scalar> colorMp;
 
     res.release();
     res.create(row, col, CV_8UC3);
     res = Scalar::all(0);
     for (int i = 0; i < row; i++)
     {
         int* data_bin = src.ptr<int>(i);//提取二值图像一行
         uchar* data_bgr = res.ptr<uchar>(i);//提取彩色图象一行
         for (int j = 0; j < col; j++)
         {
             if (data_bin[j] > 1)
             {
                 if (colorMp.count(data_bin[j]) <= 0)//还未生成颜色
                 {
                     //随机生成颜色
                     colorMp[data_bin[j]] = formatRandom();
                 }
                 //赋值颜色
                 Scalar c = colorMp[data_bin[j]];
                 *data_bgr++ = c[0];
                 *data_bgr++ = c[1];
                 *data_bgr++ = c[2];
             }
             else
             {
                 data_bgr++;
                 data_bgr++;
                 data_bgr++;
             }    
         }
     }
 }
 
 int main()
 {
     cout << "输入操作：0-数大米(未滤波)，1-区域标记, 2-数大米（滤波） />";
     int ans;
     cin >> ans;
     if (ans == 1)
     {
         Mat src = imread("../images/raw_testimg.png");
         imshow("原图像", src);
         //转二值图像
         cvtColor(src, src, COLOR_BGR2GRAY);
         imshow("灰度图象", src);
         threshold(src, src, 50, 1, THRESH_BINARY_INV);//背景为白，目标为黑
         imshow("二值图像", src);
         Mat res, rgbRes;
         //图象标记
         areaLabeling(src, res);
         //转换彩色图象并显示
         Bin2BGR(res, rgbRes);
         imshow("标记图彩色", rgbRes);
     }
     else if (ans == 2)
     {
         Mat src = imread("../images/some_rice.png");
         imshow("原图像", src);
         //中值滤波去除椒盐噪声
         medianBlur(src, src, 3);
         imshow("中值滤波", src);
         //转二值图像
         cvtColor(src, src, COLOR_BGR2GRAY);
         imshow("灰度图象", src);
         threshold(src, src, 22, 1, THRESH_BINARY);
        //  threshold(src, src, 100, 1, THRESH_BINARY);//背景为白，目标为黑
         imshow("二值图像", src);
         Mat res, rgbRes;
         //图象标记
         areaLabeling(src, res);
         //转换彩色图象并显示
         Bin2BGR(res, rgbRes);
         imshow("标记图彩色", rgbRes);
     }
     else if (ans == 0)
     {
         Mat src = imread("../images/some_rice.png");
        //  Mat src = imread("../images/my_rice.png");
         imshow("原图像", src);
         //转二值图像
         cvtColor(src, src, COLOR_BGR2GRAY);
         imshow("灰度图象", src);
         threshold(src, src, 20, 1, THRESH_BINARY);//背景为白，目标为黑
         imshow("二值图像", src);
         Mat res, rgbRes;
         //图象标记
         areaLabeling(src, res);
         //转换彩色图象并显示
         Bin2BGR(res, rgbRes);
         imshow("标记图彩色", rgbRes);
     }
     else
     {
         cout << "输入错误, 退出" << endl;
         return 0;
     }
     
 
     waitKey(0);
    return 0;
 }