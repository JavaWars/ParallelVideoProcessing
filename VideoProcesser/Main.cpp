#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>           
#include <fstream>
#include <omp.h>

void video_converter_line_logic() {

	 //Открытие файла для обработки
     cv::VideoCapture cap("masha.mp4");
     if (!cap.isOpened()) {
         std::cout << "Unable to open the camera\n";
         std::exit(-1);

	}

     //Получение размера видео и количества кадров в секунду
     int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
     int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
     double FPS = cap.get(CV_CAP_PROP_FPS);

     //Открытие результирующего файла
     cv::VideoWriter out("output.mov", CV_FOURCC('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
     if (!out.isOpened()) {
         std::cout << "Error! Unable to open video file for output." << std::endl;
         std::exit(-1);
	}

    cv::Mat image;//исходный кадр
    cv::Mat panel_1, panel_2, panel_3, panel_4;//обработанные кадры
    int delta = width / 4;

    while (true) {
        cap >> image;
        if (image.empty()) {
            std::cout << "Can't read frames from your camera\n";
            break;

		}

		//Серый цвет
        panel_1 = image(cv::Rect(0, 0, image.cols / 4, image.rows));
        cv::cvtColor(panel_1, panel_1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(panel_1, panel_1, cv::COLOR_GRAY2BGR);
        panel_1.copyTo(image(cv::Rect(0, 0, image.cols / 4, image.rows)));

		//синий
		panel_2 = image(cv::Rect(delta, 0, image.cols / 4, image.rows));
		panel_2 -= cv::Scalar(0, 255, 255);
		panel_2.copyTo(image(cv::Rect(delta, 0, image.cols / 4, image.rows)));

		//зеленый
		panel_3 = image(cv::Rect(2*delta, 0, image.cols / 4, image.rows));
		panel_3 -= cv::Scalar(255, 0, 255);
		panel_3.copyTo(image(cv::Rect(2*delta, 0, image.cols / 4, image.rows)));

		//красный
		panel_4 = image(cv::Rect(3*delta, 0, image.cols / 4, image.rows));
		panel_4 -= cv::Scalar(255, 255, 0);
		panel_4.copyTo(image(cv::Rect(3*delta, 0, image.cols / 4, image.rows)));
        
		//Сохранить кадр
        out << image;

        cv::imshow("Modified video", image);

         //Остановить обратоку если нажата клавиша "ESC"
        if (cv::waitKey(1000.0 / FPS) == 27) break;
	}

}

void video_converter_parallel_sections() {

	//Открытие файла для обработки
	cv::VideoCapture cap("masha.mp4");
	if (!cap.isOpened()) {
		std::cout << "Unable to open the camera\n";
		std::exit(-1);
	}

	//Получение размера видео и количества кадров в секунду
	int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	double FPS = cap.get(CV_CAP_PROP_FPS);

	//Открытие результирующего файла
	cv::VideoWriter out("output.mov", CV_FOURCC('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
	if (!out.isOpened()) {
		std::cout << "Error! Unable to open video file for output." << std::endl;
		std::exit(-1);
	}

	cv::Mat image;//исходный кадр
	cv::Mat panel_1, panel_2, panel_3, panel_4;//обработанные кадры
	int delta = width / 4;

	while (true) {
		cap >> image;
		if (image.empty()) {
			std::cout << "Can't read frames from your camera\n";
			break;

		}
#pragma omp parallel sections 
		{
#pragma omp section  //Серый цвет
			{
				panel_1 = image(cv::Rect(0, 0, image.cols / 4, image.rows));
				cv::cvtColor(panel_1, panel_1, cv::COLOR_BGR2GRAY);
				cv::cvtColor(panel_1, panel_1, cv::COLOR_GRAY2BGR);
				panel_1.copyTo(image(cv::Rect(0, 0, image.cols / 4, image.rows)));
			}
#pragma omp section //синий 
			{
				panel_2 = image(cv::Rect(delta, 0, image.cols / 4, image.rows));
				panel_2 -= cv::Scalar(0, 255, 255);
				panel_2.copyTo(image(cv::Rect(delta, 0, image.cols / 4, image.rows)));
			}
#pragma omp section //зеленый 
			{
				panel_3 = image(cv::Rect(2 * delta, 0, image.cols / 4, image.rows));
				panel_3 -= cv::Scalar(255, 0, 255);
				panel_3.copyTo(image(cv::Rect(2 * delta, 0, image.cols / 4, image.rows)));
			}
#pragma omp section //красный  
			{
				panel_4 = image(cv::Rect(3 * delta, 0, image.cols / 4, image.rows));
				panel_4 -= cv::Scalar(255, 255, 0);
				panel_4.copyTo(image(cv::Rect(3 * delta, 0, image.cols / 4, image.rows)));
			}
		}

		//Сохранить кадр
		out << image;

		cv::imshow("Modified video", image);

		//Остановить обратоку если нажата клавиша "ESC"
		if (cv::waitKey(1000.0 / FPS) == 27) break;
	}

}

void process(cv::Mat &image,int delta) {
	cv::Mat panel_1, panel_2, panel_3, panel_4;//обработанные кадры
	if (image.empty()) {
		std::cout << "Can't read frames from your camera\n";
		return;
	}
	//Серый цвет
	panel_1 = image(cv::Rect(0, 0, image.cols / 4, image.rows));
	cv::cvtColor(panel_1, panel_1, cv::COLOR_BGR2GRAY);
	cv::cvtColor(panel_1, panel_1, cv::COLOR_GRAY2BGR);
	panel_1.copyTo(image(cv::Rect(0, 0, image.cols / 4, image.rows)));

	//синий
	panel_2 = image(cv::Rect(delta, 0, image.cols / 4, image.rows));
	panel_2 -= cv::Scalar(0, 255, 255);
	panel_2.copyTo(image(cv::Rect(delta, 0, image.cols / 4, image.rows)));

	//зеленый
	panel_3 = image(cv::Rect(2 * delta, 0, image.cols / 4, image.rows));
	panel_3 -= cv::Scalar(255, 0, 255);
	panel_3.copyTo(image(cv::Rect(2 * delta, 0, image.cols / 4, image.rows)));

	//красный
	panel_4 = image(cv::Rect(3 * delta, 0, image.cols / 4, image.rows));
	panel_4 -= cv::Scalar(255, 255, 0);
	panel_4.copyTo(image(cv::Rect(3 * delta, 0, image.cols / 4, image.rows)));
}

void video_example_parallel() {
	int const static parallel_threads = 64;//8

	//Открытие файла для обработки
	cv::VideoCapture cap("masha.mp4");
	if (!cap.isOpened()) {
		std::cout << "Unable to open the camera\n";
		std::exit(-1);
	}

	//Получение размера видео и количества кадров в секунду
	int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	double FPS = cap.get(CV_CAP_PROP_FPS);
	//Открытие результирующего файла
	cv::VideoWriter out("output.mov", CV_FOURCC('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
	if (!out.isOpened()) {
		std::cout << "Error! Unable to open video file for output." << std::endl;
		std::exit(-1);
	}

	cv::Mat image[parallel_threads];//исходный кадр
	int delta = width / 4;

	while (true) {
		for (int i = 0; i < parallel_threads; i++) {
			cap >> image[i];
		}

		//Начало параллельной части
#pragma omp parallel for num_threads(parallel_threads)
		for (int i = 0; i < parallel_threads; i++) {
			process(image[i], delta);
		}
		//конец параллельной части

		//Сохранить кадры
		for (int i = 0; i < parallel_threads; i++) {
			if (!image[i].empty()) {
				out << image[i];
				cv::imshow("Modified video", image[i]);
			}
			else {
				std::cout << "Can't read frames from your camera\n";
				return;
			}
		}
		
		//Остановить обратоку если нажата клавиша "ESC"
		if (cv::waitKey(1000.0 / FPS) == 27) break;
	}

}

void video_example_parallel_optimization() {
	//int const static parallel_threads = 200;//8
	int const static block_size = 64;//8
											//Открытие файла для обработки
	cv::VideoCapture cap("masha.mp4");
	if (!cap.isOpened()) {
		std::cout << "Unable to open the camera\n";
		std::exit(-1);
	}

	//Получение размера видео и количества кадров в секунду
	int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	double FPS = cap.get(CV_CAP_PROP_FPS);
	//Открытие результирующего файла
	cv::VideoWriter out("output.mov", CV_FOURCC('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
	if (!out.isOpened()) {
		std::cout << "Error! Unable to open video file for output." << std::endl;
		std::exit(-1);
	}

	cv::Mat image[block_size];//исходный кадр
	int delta = width / 4;

	while (true) {
		for (int i = 0; i < block_size; i++) {
			cap >> image[i];
		}

		//Начало параллельной части
#pragma omp parallel for schedule(guided,1)
		for (int i = 0; i < block_size; i++) {
			process(image[i], delta);
		}
		//конец параллельной части

		//Сохранить кадры
		for (int i = 0; i < block_size; i++) {
			if (!image[i].empty()) {
				out << image[i];
				cv::imshow("Modified video", image[i]);
			}
			else {
				std::cout << "Can't read frames from your camera\n";
				return;
			}
		}

		//Остановить обратоку если нажата клавиша "ESC"
		if (cv::waitKey(1000.0 / FPS) == 27) break;
	}

}


int main() {
	
	int start = clock();
	video_converter_line_logic();
	//video_converter_parallel_sections();
	//video_example_parallel();
	//video_example_parallel_optimization();
	int fin = clock();
	std::ofstream fout("out.txt");
	fout << (fin - start);
	return(0);
}