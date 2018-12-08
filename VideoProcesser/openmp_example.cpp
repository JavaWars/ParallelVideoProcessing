#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>           
#include <fstream>
#include <time.h>

#include <omp.h>

void doTask() {

	cv::Mat imgOriginal;        // input image
	cv::Mat imgProcessed;           // Canny edge image

	imgOriginal = cv::imread("image.png");          // open image

	if (imgOriginal.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
																//return(0);                                              // and exit program
	}

	for (int y = 0; y<imgOriginal.rows; y++)
	{
		for (int x = 0; x<imgOriginal.cols; x++)
		{
			// get pixel
			cv::Vec3b color = imgOriginal.at<cv::Vec3b>(cv::Point(x, y));

			//color[1] = 0;

			// set pixel
			imgOriginal.at<cv::Vec3b>(cv::Point(x, y)) = color;
		}
	}

	cv::imshow("imgOriginal", imgOriginal);     // show windows
	cv::imshow("imgCanny", imgProcessed);

	//cv::imwrite("out.txt", imgProcessed);
}
