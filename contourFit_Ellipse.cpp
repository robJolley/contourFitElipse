#define PI 3.14159265358979323846
#include<opencv2/opencv.hpp>
#include<iostream>
#include<ctime>

#define ND 0  //directions for 8 connected matrix
#define TL 1
#define TM 2
#define TR 3
#define MR 4
#define BR 5
#define BM 6
#define BL 7
#define ML 8


using namespace std;
using namespace cv;
int curw = 0;
int curh = 0;
bool nextPixel;
int primaryDir = 0;

void tlFunction()
{
			nextPixel = true;
			curh -= 1;
			curw -= 1;
			primaryDir = TL;
}

void tmFunction()
{

			nextPixel = true;
			curh -= 1;
			primaryDir = TM;
}

void trFunction()
{
			nextPixel = true;
			curh -= 1;
			curw += 1;
			primaryDir = TR;
}
void mrFunction()
{

			nextPixel = true;
			curw += 1;
			primaryDir = MR;
}

void brFunction()
{
			nextPixel = true;
			curh += 1;
			curw += 1;
			primaryDir = BR;
}

void bmFunction()
{

			nextPixel = true;
			curh += 1;
			primaryDir = BM;
}
void blFunction()
{
			nextPixel = true;
			curh += 1;
			curw -= 1;
			primaryDir = BL;
}

void mlFunction()
{

			nextPixel = true;
			curw -= 1;
			primaryDir = ML;
}
int main()
{
			int thold = 15;

			clock_t start;
			double duration;
			const int sobelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };  //Where origionally floats in python
			const int sobelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} }; //Where origionally floats in python
			const int kernel[5][5] = { {1,6,12,6,1},
																														{6,42,79,42,6},
																														{12,79,148,79,12},
																														{6,42,79,42,6},
																														{1,6,12,6,1} };// 1/732
			// Above normalised kernal for smoothing,  see origional python script for method 
			start = std::clock();
			int height, width, intPixel, tSx, tSy, tS, dirE, maxDir, curPoint, contDirection, cannyImgPix, nd, tl, tm, tr, mr, br, bm, bl, ml = 0;
			int contNum = 128;
			int	contPixCount = 0;
			int	curContNum = 0;
			int	contPlace = 0;
			int	oldContPlace = 0;
			bool maxPoint;
			struct pixel {
						int number;
						int h;
						int w;

			};
			std::vector<pixel> contourList;
			double floatPixel = 0.0;
			int kernalCumulator = 0;
			const int mp = 3;
			Scalar color(0, 0, 255);
			//			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			//			start = clock();
			//			cout << "Start image in" << duration << '\n';
			//			Mat dst;
			Mat rawImg = imread("C:\\Users\\RobertJolley\\OneDrive - Innopharma Labs\\Documents\\openCV_Master\\openCVexample\\openCVexample\\resize.bmp ", 0);
			height = rawImg.rows;
			width = rawImg.cols;
			cout << "Height of image " << height << '\n';
			cout << "Width of image " << width << '\n';
			Mat filteredImg = Mat::zeros(height, width, CV_8UC1);
			Mat sobelImg = Mat::zeros(height, width, CV_8UC1);
			Mat directionImg = Mat::zeros(height, width, CV_8UC1);
			Mat cannyImg = Mat::zeros(height, width, CV_8UC1);
			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			cout << "Start image in" << duration << '\n';
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

			//			imshow("thresholded Image", rawImg);
			//			waitKey();
			// Loop to smooth using Gausian 5 x 5 kernal

			for (int h = 3; h < (height - 3); h++)
			{
						for (int w = 3; w < (width - 3); w++)
						{
									if (rawImg.at<uchar>(h, w) > 0)//Thresholding included
									{
												for (int xk = 0; xk < 5; xk++)
												{
															for (int yk = 0; yk < 5; yk++)
															{
																		intPixel = (rawImg.at<uchar>((h + (xk - mp)), (w + (yk - mp))));
																		kernalCumulator += intPixel*(kernel[xk][yk]);//Mutiplier required as rounding is making number go above 255,  better solution?
															}
												}
									}

									kernalCumulator = kernalCumulator / 732;
									if (kernalCumulator < 0.0 || kernalCumulator > 255)
									{
												cout << "kernal Value: " << kernalCumulator;
												cout << " intPixel:" << intPixel << '\n';
									}
									filteredImg.at<uchar>(h, w) = (int)kernalCumulator;
									kernalCumulator = 0;
						}
			}
			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			cout << "Smoothing Duration" << duration << '\n';
			//			imshow("smoothed Image", filteredImg);
			//			waitKey();
						// Loop to Sobel filter
			for (int h = 0; h < (height); h++)
			{
						//#pragma omp parallel for
						for (int w = 0; w < (width); w++)
						{

									if (filteredImg.at<uchar>(h, w) > 0)
									{
												for (int xk = 0; xk < 3; xk++)
												{
															for (int yk = 0; yk < 3; yk++)
															{
																		tSx += sobelX[xk][yk] * filteredImg.at<uchar>(((h - 1) + xk), ((w - 1) + yk));
																		tSy += sobelY[xk][yk] * filteredImg.at<uchar>(((h - 1) + xk), ((w - 1) + yk));
															}
												}
												tS = (int)(0.28*sqrt(tSx*tSx + tSy*tSy));//going over 255 thus 0.4 need to fix 16 bit wide in python??
												if (tS > 25)// Was 76
												{
															if (tS > 255)
																		cout << "Ts:" << tS << " tSx:" << tSx << " tSy:" << tSy << '\n';
															sobelImg.at<uchar>(h, w) = tS;
															dirE = (int)(atan((float)tSy / (float)tSx))*(float)(180.0 / PI);
															if (dirE < 0)
																		dirE = dirE + 180;
															dirE = 180 - dirE;
															directionImg.at<uchar>(h, w) = dirE;
												}
												tS = 0;
												tSx = 0;
												tSy = 0;
									}
									//									tS = 0;
									//									tSx = 0;
									//									tSy = 0;
						}
			}

			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			cout << "Sobel Duration" << duration << '\n';
			// #pragma omp parallel for
			for (int h = 1; h < (height - 1); h++)
			{
						for (int w = 1; w < (width - 1); w++)
						{
									dirE = directionImg.at<uchar>(h, w);
									maxPoint = true;
									maxDir = 0;
									curPoint = sobelImg.at<uchar>(h, w);
									if (curPoint > 0)
									{
												if ((22 <= dirE) && (dirE <= 67)) // #NW
												{
															maxDir = 255;
															if (sobelImg.at<uchar>(h - 1, w + 1) > curPoint)
															{
																		maxPoint = false;
															}
															if (curPoint < sobelImg.at<uchar>(h + 1, w - 1))
															{
																		maxPoint = false;
															}
												}
												else if ((67 <= dirE) && (dirE <= 122)) //N
												{
															maxDir = 255;
															if (sobelImg.at<uchar>(h - 1, w) > curPoint)
															{
																		maxPoint = false;
															}
															if (curPoint < sobelImg.at<uchar>(h + 1, w))
															{
																		maxPoint = false;
															}
												}
												else if ((122 <= dirE) && (dirE <= 157)) // #NE
												{
															maxDir = 255;
															if (sobelImg.at<uchar>(h - 1, w - 1) > curPoint)
															{
																		maxPoint = false;
															}
															if (curPoint < sobelImg.at<uchar>(h + 1, w + 1))
															{
																		maxPoint = false;
															}
												}
												else //E
												{
															maxDir = 255;
															if (sobelImg.at<uchar>(h, w - 1) > curPoint)
															{
																		maxPoint = false;
															}
															if (curPoint < sobelImg.at<uchar>(h, w + 1, 0))
															{
																		maxPoint = false;
															}
												}
												if (maxPoint)
												{
															cannyImg.at<uchar>(h, w) = 255;
												}
									}

						}

			}
			/*
			pixel tempPixel;
			duration = ((clock()) - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			cout << "Canny Edge Duration" << duration << '\n';
			int curhtemp, curwtemp;
			nextPixel = false;
			for (int h = 1; h < (height - 1); h++)
			{
						for (int w = 1; w < (width - 1); w++)
						{
									cannyImgPix = cannyImg.at<uchar>(h, w);
									curh = h;
									curw = w;
									primaryDir = ND;
									while ((curh <= (height - 1)) && (curw < (width - 1)) && (cannyImgPix == 1))
									{
												nextPixel = false;
												curhtemp = curh;
												curwtemp = curw;
												tl = cannyImg.at<uchar>(curh - 1, curw - 1); //defining the 8 connected matrix
												tm = cannyImg.at<uchar>(curh - 1, curw);	//top left TL, top Middle ect....
												tr = cannyImg.at<uchar>(curh - 1, curw + 1);
												ml = cannyImg.at<uchar>(curh, curw - 1);
												mr = cannyImg.at<uchar>(curh, curw + 1);
												bl = cannyImg.at<uchar>(curh + 1, curw - 1);
												bm = cannyImg.at<uchar>(curh + 1, curw);
												br = cannyImg.at<uchar>(curh + 1, curw + 1);
												if ((tl == 1) || (tm == 1) || (tr == 1) || (ml == 1) || (mr == 1) || (bl == 1) || (bm == 1) || (br == 1))
												{
															//----------------- MR First ---------------------
															if (primaryDir == ND || primaryDir == MR)
															{
																		if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else if (br == 1)
																					brFunction();
																		else if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else
																					nextPixel = false;
															}
															//       ----------------- BR ---------------------
															else if (primaryDir == BR)
															{
																		if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else if (br == 1)
																					brFunction();
																		else if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else
																					nextPixel = false;
															}
															//       ----------------- BM ---------------------

															else if (primaryDir == BM)
															{
																		if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else if (br == 1)
																					brFunction();
																		else if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else
																					nextPixel = false;
															}
															//       ----------------- BL ---------------------
															else if (primaryDir == BL)
															{
																		if (mr == 1)
																					mrFunction();
																		else if (br == 1)
																					brFunction();
																		else if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else
																					nextPixel = false;
															}

															//----------------- ML ---------------------
															else if (primaryDir == ML)
															{
																		if (bl == 1)
																					blFunction();
																		else if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else
																					nextPixel = false;
															}

															//       ----------------- TL ---------------------
															else if (primaryDir == TL)
															{
																		if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else if (br == 1)
																					brFunction();
																		else
																					nextPixel = false;
															}
															//       ----------------- TM ---------------------
															else if (primaryDir == TM)
															{
																		if (bl == 1)
																					blFunction();
																		else if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else if (bl == 1)
																					blFunction();
																		else if (bm == 1)
																					bmFunction();
																		else
																					nextPixel = false;
															}

															// ----------------- TR ---------------------
															else if (primaryDir == TR)
															{
																		if (ml == 1)
																					mlFunction();
																		else if (tl == 1)
																					tlFunction();
																		else if (tm == 1)
																					tmFunction();
																		else if (tr == 1)
																					trFunction();
																		else if (mr == 1)
																					mrFunction();
																		else if (br == 1)
																					brFunction();
																		else if (bm == 1)
																					bmFunction();
																		else if (bl == 1)
																					blFunction();
																		else
																					nextPixel = false;
															}

															else
															{
																		cannyImg.at<uchar>(curhtemp, curwtemp) = contNum;
																		nextPixel = false;
															}
												}
												if (nextPixel == true)
												{
															cannyImg.at<uchar>(curhtemp, curwtemp) = contNum;
															cannyImgPix = cannyImg.at<uchar>(curh, curw);
															tempPixel.number = contNum;
															tempPixel.h = curh;
															tempPixel.w = curw;
															contourList.push_back(tempPixel);
															contPlace += 1;
															contPixCount += 1;
												}
												else {
															cannyImgPix = 0;
															cannyImg.at<uchar>(curhtemp, curwtemp) = contNum;
															contNum += 1;
															if (contPixCount < 80)
															{
																		cout << "presize: " << contourList.size();
																		contourList.erase(contourList.begin() + oldContPlace, contourList.end());
																		cout << " postsize: " << contourList.size();
																		cout << " contPlace: " << contPlace;
																		cout << " oldContPlace: " << oldContPlace << '\n';
																		contPlace = oldContPlace;
															}
															contPixCount = 0;
															oldContPlace = contPlace;

												}
									}
						}
			}
			ofstream myfile("contourList.csv");
			int vsize = contourList.size();
			for (int n; n < vsize; n++)
			{
						tempPixel = contourList[n];
						myfile << tempPixel.number << "," << tempPixel.h << "," << tempPixel.w << '\n';
			}
			myfile.close();
			*/
			imshow("Direction Image", directionImg);
			imwrite("directionImage.jpg", directionImg);

			imshow("Canny Image", cannyImg);
			imwrite("cannyImage.jpg", cannyImg);

			imwrite("sobelImage.jpg", sobelImg);
			imshow("SobelImage", sobelImg);
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