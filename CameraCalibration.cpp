#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;
using namespace cv;
 
int main()
{
	ofstream fout("calibration_result.txt");          
    
	cout << "1、开始提取角点……" << endl;                
	int image_nums = 0;                              
	int points_per_row = 10;                          
	int points_per_col = 7;                         
	Size image_size;                                   
	Size corner_size(points_per_row, points_per_col);  
	vector<Point2f> points_per_image;                 
	vector<vector<Point2f>> points_all_images;        
	string image_file_name;			                
	Mat image_raw, image_gray;                       
	//while (getline(fin, image_file_name))
    for(const auto& entry : fs::directory_iterator("images_2"))
	{
		image_raw = imread(entry.path().string());                                      
		cvtColor(image_raw, image_gray, COLOR_BGR2GRAY);                              
                
        if (image_gray.empty()){
            cerr << "Could not convert the image to grayscale: "<<entry.path()<<endl;
            continue;
        }

		bool success = findChessboardCorners(image_gray, corner_size, points_per_image); 
		if (success == false)
		{
			cout << "角点提取失败" << endl;
			exit(1);  
		}
		else
		{
			find4QuadCornerSubpix(image_gray, points_per_image, Size(5, 5)); 
			points_all_images.push_back(points_per_image);                   
		}
		if (image_nums == 0)
		{
			cout << "channels = " << image_raw.channels() << endl;  
			cout << "image type = " << image_raw.type() << endl;    
			image_size.width = image_raw.cols;                      
			image_size.height = image_raw.rows;                     
			cout << "image width = " << image_size.width << endl;   
			cout << "image height = " << image_size.height << endl; 
		}
		image_nums++;
	}
	cout << "image_nums = " << image_nums << endl;     

	cout << "2、开始计算角点3D坐标……" << endl;    
	Size block_size(16, 16);                        
	vector<Point3f> points3D_per_image;             
	Point3f point3D;                                
	for (int i = 0; i < corner_size.height; i++)     
	{
		for (int j = 0; j < corner_size.width; j++)  
		{
			point3D = Point3f(block_size.width * j, block_size.height * i, 0);
			points3D_per_image.push_back(point3D);
		}
	}
	vector<vector<Point3f>> points3D_all_images(image_nums, points3D_per_image); 
	int point_counts = corner_size.area();                                       
 
	cout << "3、开始标定相机……" << endl;         
	Mat cameraMat(3, 3, CV_32FC1, Scalar::all(0));   
	Mat distCoeffs(1, 5, CV_32FC1, Scalar::all(0));  
	vector<Mat> rotationMat;                         
	vector<Mat> translationMat;                      
	calibrateCamera(points3D_all_images, points_all_images, image_size, cameraMat, distCoeffs, rotationMat, translationMat, 0); 
 
	cout << "4、将标定结果写入文件……" << endl;
	fout << "相机内参数矩阵:" << endl << cameraMat << endl << endl;
	fout << "相机的畸变系数:" << endl << distCoeffs << endl << endl;
	Mat rotate_Mat = Mat(3, 3, CV_32FC1, Scalar::all(0)); 
	fout << endl;
	fout.close();
 
	return 0;
}