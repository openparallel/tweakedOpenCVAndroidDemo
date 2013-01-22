#include "Utils.hpp"

Utils::Utils()
{
    timer_duration = 0.0;
}

void Utils::ShowImage(cv::Mat image, std::string title)
{
    //cv::namedWindow(title);
    //cv::imshow(title, image);
}

void Utils::ShowInfo()
{
    //std::cout<<"TICK FREQUENCY: "<<cv::getTickFrequency()<<std::endl;
    //std::cout<<"Num CPUs: "<<cv::getNumberOfCPUs()<<std::endl;
    //std::cout<<"Num Threads:"<<cv::getNumThreads()<<std::endl;
    //std::cout<<"Build Information"<<cv::getBuildInformation()<<std::endl;
}

void Utils::StartTimer()
{
    timer_duration = static_cast<double>(cv::getTickCount());
}


void Utils::StopTimer(std::string methodName)
{
    timer_duration = static_cast<double>(cv::getTickCount()) - timer_duration;
    tick_log[methodName].push_back(timer_duration); // num of ticks
    timer_duration /= cv::getTickFrequency(); // in s
    timer_log[methodName].push_back(timer_duration);
}

void Utils::Show_Timer_Log(std::string methodName)
{
    if(timer_log[methodName].size()!=0)
    {
        //std::cout<<"Method "<<methodName<<" has the following runtimes (s): "<<std::endl;
        int vec_size = timer_log[methodName].size();
        for(int i=0; i <vec_size; i++)
        {
            //if(i == vec_size -1)
                //std::cout<<timer_log[methodName][i]<<std::endl;
            //else
                //std::cout<<timer_log[methodName][i]<<",";
        }
    }else
    {
        //std::cout<<"Method "<<methodName<<" has not been run"<<std::endl;
    }
}

void Utils::Show_Tick_Log(std::string methodName)
{
    if(tick_log[methodName].size()!=0)
    {
        //std::cout<<"Method "<<methodName<<" has the following tick counts: "<<std::endl;
        int vec_size = tick_log[methodName].size();
        for(int i=0; i <vec_size; i++)
        {
            //if(i == vec_size -1)
                //std::cout<<tick_log[methodName][i]<<std::endl;
            //else
                //std::cout<<tick_log[methodName][i]<<",";
        }
    }else
    {
        //std::cout<<"Method "<<methodName<<" has not been run"<<std::endl;
    }
}

void Utils::Show_Average_Time(std::string methodName)
{
    std::string output;
    std::ostringstream avg,vsize;
    if(timer_log[methodName].size()!=0)
    {
        int vec_size = timer_log[methodName].size();
        double num_runs = static_cast<double>(vec_size);
        double sum,average = 0.0;

        for(int i=0; i<vec_size; i++)
        {
            sum += timer_log[methodName][i];
        }

        average = sum / num_runs;
        average_time_log[methodName].push_back(average);
        avg << average;
        vsize << vec_size;
        output = methodName + ": AVERAGE TIME: " + avg.str() + "(" + vsize.str() + ")";
    #ifdef ANDROID
        LOGE("Captain's Log",output.c_str());
    #else    
        std::cout<<methodName<<": AVERAGE TIME: "<<average<<"("<<vec_size<<")"<<std::endl;
    #endif
    
    }else
    {
        //std::cout<<"Method "<<methodName<<" has not been run"<<std::endl;
    }
}

void Utils::Show_Average_Ticks(std::string methodName)
{
    if(tick_log[methodName].size()!=0)
    {
        int vec_size = tick_log[methodName].size();
        double num_runs = static_cast<double>(vec_size);
        double sum,average = 0.0;

        for(int i=0; i<vec_size; i++)
        {
            sum += tick_log[methodName][i];
        }

        average = sum / num_runs;
        //std::cout<<"Method "<<methodName<<" took AVERAGE TICKS: "<<average<<"s over "<<vec_size<<" runs"<<std::endl;
    }else
    {
        //std::cout<<"Method "<<methodName<<" has not been run"<<std::endl;
    }
}

void Utils::Show_Speedup(std::string methodName)
{
    std::string unop_methodName = "UNOP_" + methodName;
    int num_elems;
    std::string output;
    std::ostringstream spd;
    double optimized = 0, unoptimized = 0, speedup = 0;
    if(average_time_log[methodName].size()!=0 && average_time_log[unop_methodName].size()!=0)
    {
        num_elems = average_time_log[methodName].size();
        for(int i=0; i<num_elems; i++)
        {
            optimized += average_time_log[methodName][i];
            unoptimized += average_time_log[unop_methodName][i];
        }
        speedup = unoptimized / optimized;
        spd << speedup;
        output = methodName + ": SPEEDUP: " + spd.str();
    #ifdef ANDROID
        LOGE("Captain's Log",output.c_str());
    #else    
        std::cout<<methodName<<": SPEEDUP: "<<speedup<<std::endl;
    #endif
        //std::cout<<std::endl;
    }else
    {
        //std::cout<<"n_mthn: "<<average_time_log[methodName].size();
        //std::cout<<", n_un_mthn: "<<average_time_log[unop_methodName].size();
        //std::cout<<"Method "<<methodName<<" has not been averaged"<<std::endl;
    }
}

//void Utils::PAPI_handle_error (int retval)
//{
         //printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
//}


