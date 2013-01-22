#ifndef _transform_h
#define _transform_h

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "Utils.hpp"

class Transform{

    private:
        cv::Mat image;

    public:
        Transform(std::string imageName);
        Transform(cv::Mat img);
        cv::Mat Grayscale(std::string methodName, Utils* util,bool useOptimized);    
        cv::Mat Noise(std::string methodName, Utils* util,int numPixNoise, bool useOptimized);
        cv::Mat Sepia(std::string methodName, Utils* util,bool useOptimized);
        cv::Mat Invert(std::string methodName, Utils* util, bool useOptimized);
        void ConvertFS(std::string methodName, Utils* util, bool useOptimized);
        cv::Mat GSBlur(std::string methodName, Utils* util, bool useOptimized);
        void Threshold(std::string methodName, Utils* util, bool useOptimized, int thresh_type);
        void Threshold16(std::string methodName, Utils* util, bool useOptimized,int thresh_type);
        cv::Mat Sobel(std::string methodName, Utils* util, bool useOptimized);
        cv::Mat EdgeDetect(std::string methodName, Utils* util, bool useOptimized);
};
#endif
