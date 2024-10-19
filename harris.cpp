#include <opencv2/opencv.hpp>
#include <iostream>

void harrisCornerDetection(const cv::Mat &inputImage) {
    // 将图像转换为灰度图像
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    
    // 将灰度图像转换为浮点型
    cv::Mat floatImage;
    grayImage.convertTo(floatImage, CV_32F);
    
    // 设定 Harris 角点检测参数
    int blockSize = 2; // 邻域大小
    int ksize = 3;     // Sobel 算子的大小
    double k = 0.04;   // Harris 角点检测的参数

    // 计算 Harris 角点响应
    cv::Mat harrisResponse;
    cv::cornerHarris(floatImage, harrisResponse, blockSize, ksize, k);
    
    // 归一化处理
    cv::Mat responseNormalized;
    cv::normalize(harrisResponse, responseNormalized, 0, 255, cv::NORM_MINMAX);
    responseNormalized.convertTo(responseNormalized, CV_8U);
    
    // 设定阈值
    double threshold = 200;
    cv::Mat cornerImage = cv::Mat::zeros(inputImage.size(), CV_8UC3);
    
    // 绘制角点
    for (int y = 0; y < responseNormalized.rows; y++) {
        for (int x = 0; x < responseNormalized.cols; x++) {
            if (responseNormalized.at<uchar>(y, x) > threshold) {
                cv::circle(cornerImage, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), 2);
            }
        }
    }

    // 显示结果
    cv::imshow("Harris Corners", cornerImage);
    cv::waitKey(0); // 等待按键
}

int main(int argc, char** argv) {
    // 读取输入图像
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return -1;
    }

    cv::Mat inputImage = cv::imread(argv[1]);
    if (inputImage.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    harrisCornerDetection(inputImage);
    
    return 0;
}
