#include<opencv2/opencv.hpp>
#include<iostream>
#include<ctime>
using namespace std;
using namespace cv;
int main()
{
			clock_t start;
			double duration;
			const int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};  //Where origionally floats in python
			const int sobelY[3][3] = {{-1.0, -2.0, -1.0}, {0.0, 0.0, 0.0}, {1.0, 2.0, 1.0}}; //Where origionally floats in python
			const float kernel[5][5]={{0.00145411,0.009482,0.01771471,0.009482,0.00145411},
						{0.009482,   0.0618304,  0.11551439, 0.0618304,  0.009482},
			{0.01771471, 0.11551439, 0.2158093,  0.11551439, 0.01771471},
			{0.009482,   0.0618304,  0.11551439, 0.0618304,  0.009482},
			{0.00145411, 0.009482,   0.01771471, 0.009482,   0.00145411}};
			// Above normalised kernal for smoothing,  see origional python script for method 
			start = std::clock();
			int height, width;
			Scalar color(0, 0, 255);
			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			cout << "Start image in" << duration << '\n';
			Mat dst;
			Mat img = imread("C:\\Users\\RobertJolley\\OneDrive - Innopharma Labs\\Documents\\openCV_Master\\openCVexample\\openCVexample\\resize.bmp");
			height = img.rows;
			width = img.cols;
			cout << "Height of image " << height << '\n';
			cout << "Width of image " << width << '\n';

//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
//			start = clock();


//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
//			start = clock();
			cout << "Start copy image" << duration << '\n';
			Mat filtered = img;
//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
//			start = clock();
			cout << "End copy image" << duration << '\n';
			waitKey();
			return 0;
}