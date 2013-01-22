#ifndef _utils_h
#define _utils_h

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef ANDROID
    #include <android/log.h>
    #define LOG_TAG "Captain's Log"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif


//#include <papi.h>

class Utils{

    private:
        double timer_duration;
        std::map< std::string, std::vector<double> > timer_log;
        std::map< std::string, std::vector<double> > tick_log;
        std::map< std::string, std::vector<double> > average_time_log;

    public:
        Utils();
        void ShowImage(cv::Mat image,std::string title);
        void ShowInfo();
        void StartTimer();
        void StopTimer(std::string methodName);
        void Show_Timer_Log(std::string methodName);
        void Show_Tick_Log(std::string methodName);
        void Show_Average_Time(std::string methodName);
        void Show_Average_Ticks(std::string methodName);
        void Show_Speedup(std::string methodName);
        //void PAPI_handle_error(int retval);
};

#endif

