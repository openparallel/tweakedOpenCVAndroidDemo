#ifndef _harness_h_
#define _harness_h_

#include "Utils.hpp"
#include "Transform.hpp"

class Harness{

    private:
        cv::Mat transformed_image;
        std::string test_folder_path;
        int num_runs;
        bool print_debug;
        Transform* test_transform1;
        Transform* test_transform2;
        Transform* test_transform3;
        Transform* test_transform4;
        Transform* test_transform5;
        Utils* util;

    public:
        Harness(std::string folderPath,int nruns, bool debug);
        Harness(cv::Mat img1, cv::Mat img2, cv::Mat img3, cv::Mat img4, cv::Mat img5, int nruns, bool debug);
        ~Harness();
        void TestMethod(std::string methodName);
        void RunTests(); 
        void RunThresh(int thresh_type);
        void RunThresh16(int thresh_type);
};

#endif

