// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <set>
#include <fstream>

using namespace std;

// Структура для зберігання інформації про зображення
struct BitmapInfo {
	int width;
	int height;
	int depth;
};

// Функція для зчитування інформації про зображення з заголовка BMP файлу
BitmapInfo readBitmapInfo(const string& filename) {
	BitmapInfo info;

	// Відкриття файлу BMP у бінарному режимі
	ifstream file(filename, ios::binary);
	
	if (!file) {
		cerr << "Помилка відкриття файлу" << endl;
		exit(1);
	}

	// Перехід до позиції, де зберігається розмір зображення
	file.seekg(18);
	file.read(reinterpret_cast<char*>(&info.width), sizeof(info.width));
	file.read(reinterpret_cast<char*>(&info.height), sizeof(info.height));

	// Перехід до позиції, де зберігається глибина кольору
	file.seekg(28);
	file.read(reinterpret_cast<char*>(&info.depth), sizeof(info.depth));

	// Закриття файлу
	file.close();

	return info;
}

class ImageProcession {
	int height, width, channels, depth, colors;
	cv::Mat image;
	

	void getInfoFromHeader(const string& str) {
		
		fstream file(str, ios::binary);
		
		if (!file.is_open()) {
			cerr << "Помилка відкриття файлу" << endl;
			exit(1);
		}

		// Перехід до позиції, де зберігається розмір зображення
		file.seekg(18);
		file.read(reinterpret_cast<char*>(&width), sizeof(width));
		file.read(reinterpret_cast<char*>(&height), sizeof(height));

		// Перехід до позиції, де зберігається глибина кольору
		file.seekg(28);
		file.read(reinterpret_cast<char*>(&depth), sizeof(depth));

		// Закриття файлу
		file.close();

	}


public:

	string imgName;

	ImageProcession(const string& str)
	{
		imgName = str;

		image = cv::imread(imgName);

		if (image.empty()) {
			cerr << "Couldn't load the image" << endl;
			exit(1);
		}

		getInfoFromHeader(imgName);
		channels = image.channels();
		colors = 1 << depth;
	}	

	void imageProcessing() 
	{
		int stripeWidth = 20;

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				cv::Vec3b& pixel = image.at<cv::Vec3b>(y, x);
				uchar gray = (pixel[0] + pixel[1] + pixel[2]) / 3;
				pixel[0] = gray; // Синій (Blue)
				pixel[1] = gray; // Зелений (Green)
				pixel[2] = 255 - gray; // Червоний (Red)
			}
		}
					
		cv::rectangle(image, cv::Point(0, height / 2 - stripeWidth / 2), cv::Point(width, height / 2 + stripeWidth / 2), cv::Scalar(255, 0, 0), cv::FILLED);

		std::string::size_type pos = imgName.find_last_of("\\/");
		std::string folderPath = (pos != std::string::npos) ? imgName.substr(0, pos) : ".";
		std::string newimgName = folderPath + "\\new_" + imgName.substr(pos + 1);



		cv::imwrite(newimgName, image);


		resize(image, image, cv::Size(1280, 1280));
		cv::imshow("Результат", image);

		cv::waitKey(0);
	}
};

class ImageInfo {

	int height, width, channels, depth, colors;


public:

	ImageInfo(BitmapInfo b, int channels)
	{
		this->height = b.height;
		this->width = b.width;
		this->channels = channels;
		this->depth = b.depth;
		this->colors = 1 << depth;
	}

	ImageInfo(int height, int width, int channels, int depth)
	{
		this->height = height;
		this->width = width;
		this->channels = channels;
		this->depth = depth;
		this->colors = 1 << depth;
	}
		
	int getHeight() 
	{
		return height;
	}

	int getWidth() 
	{
		return width;
	}

	void printInfo() 
	{
		cout << "Размер изображения: " << width << "x" << height << " пикселей" << endl;
		cout << "Глубина цвета: " << depth << " бит" << endl;
		cout << "Количество каналов цвета: " << channels << endl;
		cout << "Количество цветов: " << colors << endl;
	}
};

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "RU");

	string imgName;
	
	cout << "Enter name of image" << endl;

	cin >> imgName;

	readBitmapInfo(imgName);

	ImageProcession ip(imgName);
	ip.imageProcessing();
	
	
	//cv::Mat image = cv::imread(imgName);

	//if (image.empty()) {
	//	cerr << "Couldn't load the image" << endl;
	//	return -1;
	//}

	//
	////Информация про картинку
	//ImageInfo imInfo = ImageInfo(readBitmapInfo(imgName), image.channels());
	//	
	//imInfo.printInfo();
	//
	//for (int y = 0; y < imInfo.getHeight(); ++y) {
	//	for (int x = 0; x < imInfo.getWidth(); ++x) {
	//		cv::Vec3b& pixel = image.at<cv::Vec3b>(y, x);
	//		uchar gray = (pixel[0] + pixel[1] + pixel[2]) / 3;
	//		pixel[0] = gray; // Синій (Blue)
	//		pixel[1] = gray; // Зелений (Green)
	//		pixel[2] = 255 - gray; // Червоний (Red)
	//	}
	//}

	//int stripeWidth = 20;

	//cv::rectangle(image, cv::Point(0, imInfo.getHeight() / 2 - stripeWidth / 2), cv::Point(imInfo.getWidth(), imInfo.getHeight() / 2 + stripeWidth / 2), cv::Scalar(255, 0, 0), cv::FILLED);


	//std::string::size_type pos = imgName.find_last_of("\\/");
	//std::string folderPath = (pos != std::string::npos) ? imgName.substr(0, pos) : ".";
	//std::string newimgName = folderPath + "\\new_" + imgName.substr(pos + 1);

	//

	//cv::imwrite(newimgName, image);

	//
	//resize(image, image, cv::Size(1280, 1280));
	//cv::imshow("Результат", image);

	//cv::waitKey(0);
}
