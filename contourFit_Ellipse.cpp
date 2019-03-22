#include<opencv2/opencv.hpp>
#include<iostream>
#include<ctime>
using namespace std;
using namespace cv;
int main()
{
			int thold = 15;

			clock_t start;
			double duration;
			const int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};  //Where origionally floats in python
			const int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}}; //Where origionally floats in python
			const double kernel[5][5]=		{{0.00145411,0.009482,0.01771471,0.009482,0.00145411},
																														{0.009482,   0.0618304,  0.11551439, 0.0618304,  0.009482},
																														{0.01771471, 0.11551439, 0.2158093,  0.11551439, 0.01771471},
																														{0.009482,   0.0618304,  0.11551439, 0.0618304,  0.009482},
																														{0.00145411, 0.009482,   0.01771471, 0.009482,   0.00145411}};
			// Above normalised kernal for smoothing,  see origional python script for method 
			start = std::clock();
			int height, width, intPixel = 0;
			double floatPixel = 0.0;
			double kernalCumulator = 0.0;
			const int mp = 3;
			Scalar color(0, 0, 255);
			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			cout << "Start image in" << duration << '\n';
			Mat dst;
			Mat rawImg = imread("C:\\Users\\RobertJolley\\OneDrive - Innopharma Labs\\Documents\\openCV_Master\\openCVexample\\openCVexample\\resize.bmp",0);
			height = rawImg.rows;
			width = rawImg.cols;
			cout << "Height of image " << height << '\n';
			cout << "Width of image " << width << '\n';
			Mat filteredImg = Mat::zeros(height, width, CV_8UC1);
			Mat sobelImg = filteredImg;
			Mat directionImg = filteredImg;
			Mat cannyImg = filteredImg;
// Loop to threshold already grayscaled image			
			for (int h = 0; h < (height);h++)
						{ 
						for (int w = 0; w < (width); w++)
									{
												if (rawImg.at<uchar>(h, w) < 5)
															{
																		rawImg.at<uchar>(h, w) = 0;
															}
									}
						}
			imshow("thresholded Image", rawImg);
			waitKey();
// Loop to smooth using Gausian 5 x 5 kernal 
			for (int h = 3; h < (height - 3); h++)
			{
						for (int w = 3; w < (width- 3); w++)
						{
									if (rawImg.at<uchar>(h, w) > 0)
									{
												for (int xk = 0; xk < 5; xk++)
												{
															for (int yk = 0; yk < 5; yk++)
															{
																		floatPixel = (double)(rawImg.at<uchar>((h + (xk - mp)), (w + (yk - mp))));
																		kernalCumulator += floatPixel*(double)kernel[xk][yk]*.90;//Mutiplier required as rounding is making number go above 255,  better solution?
																}
												}
									}

									intPixel = (int)floatPixel;
									if (kernalCumulator < 0.0 || kernalCumulator > 255)
												cout << "kernal Value" << kernalCumulator << '\n';
									filteredImg.at<uchar>(h,w) = (int)kernalCumulator;
									kernalCumulator = 0.0;
						}
			}

			imshow("Filtered Image", filteredImg);
			waitKey();

//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
//			start = clock();


//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
//			start = clock();
			cout << "Start copy image" << duration << '\n';
//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
//			start = clock();
			cout << "End copy image" << duration << '\n';
			waitKey();
			return 0;
}