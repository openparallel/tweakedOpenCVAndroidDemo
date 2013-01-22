#include "Harness.hpp"

Harness::Harness(std::string folderPath, int nruns, bool debug)
{
    test_folder_path = folderPath;
    num_runs = nruns/5;
    print_debug  = debug;
    test_transform1 = new Transform(test_folder_path + "/one.bmp");
    test_transform2 = new Transform(test_folder_path + "/two.bmp");
    test_transform3 = new Transform(test_folder_path + "/three.bmp");
    test_transform4 = new Transform(test_folder_path + "/four.bmp");
    test_transform5 = new Transform(test_folder_path + "/five.bmp");
    util = new Utils();
    RunTests();
}

Harness::Harness(cv::Mat img1, cv::Mat img2, cv::Mat img3, cv::Mat img4, cv::Mat img5, int nruns, bool debug)
{
    test_folder_path = static_cast<std::string>("");
    num_runs = nruns/5;
    util = new Utils();
    test_transform1 = new Transform(img1);
    test_transform2 = new Transform(img2);
    test_transform3 = new Transform(img3);
    test_transform4 = new Transform(img4);
    test_transform5 = new Transform(img5);
    RunTests(); 
}


Harness::~Harness()
{
    delete test_transform1;
    delete test_transform2;
    delete test_transform3;
    delete test_transform4;
    delete test_transform5;
    delete util;
}


void Harness::RunTests()
{
    TestMethod("ConvertFS");
    TestMethod("GSBlur"); 
    TestMethod("Threshold"); 
    TestMethod("Sobel"); 
    TestMethod("EdgeDetect"); 
}


void Harness::TestMethod(std::string methodName)
{
    std::string unop_methodName = "UNOP_" + methodName;
    if(methodName == "Grayscale")
    {
        std::cout<<"Grayscale TEST"<<std::endl;
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Grayscale(methodName,util,true);
            transformed_image = test_transform2->Grayscale(methodName,util,true);
            transformed_image = test_transform3->Grayscale(methodName,util,true);
            transformed_image = test_transform4->Grayscale(methodName,util,true);
            transformed_image = test_transform5->Grayscale(methodName,util,true);
        }
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {
            transformed_image = test_transform1->Grayscale(unop_methodName,util,false);
            transformed_image = test_transform2->Grayscale(unop_methodName,util,false);
            transformed_image = test_transform3->Grayscale(unop_methodName,util,false);
            transformed_image = test_transform4->Grayscale(unop_methodName,util,false);
            transformed_image = test_transform5->Grayscale(unop_methodName,util,false);
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);

    }else if(methodName == "Noise" )
    {
        std::cout<<"Noise TEST"<<std::endl;
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Noise(methodName,util,2000, true); 
            transformed_image = test_transform2->Noise(methodName,util,2000, true); 
            transformed_image = test_transform3->Noise(methodName,util,2000, true); 
            transformed_image = test_transform4->Noise(methodName,util,2000, true); 
            transformed_image = test_transform5->Noise(methodName,util,2000, true); 
        }
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {
            transformed_image = test_transform1->Noise(unop_methodName,util,2000, false); 
            transformed_image = test_transform2->Noise(unop_methodName,util,2000, false); 
            transformed_image = test_transform3->Noise(unop_methodName,util,2000, false); 
            transformed_image = test_transform4->Noise(unop_methodName,util,2000, false); 
            transformed_image = test_transform5->Noise(unop_methodName,util,2000, false); 
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);

    }else if(methodName == "Sepia")
    {
        std::cout<<"Sepia TEST invoked"<<std::endl;
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Sepia(methodName,util, true);
            transformed_image = test_transform2->Sepia(methodName,util, true);
            transformed_image = test_transform3->Sepia(methodName,util, true);
            transformed_image = test_transform4->Sepia(methodName,util, true);
            transformed_image = test_transform5->Sepia(methodName,util, true);
        }
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Sepia(unop_methodName,util, false);
            transformed_image = test_transform2->Sepia(unop_methodName,util, false);
            transformed_image = test_transform3->Sepia(unop_methodName,util, false);
            transformed_image = test_transform4->Sepia(unop_methodName,util, false);
            transformed_image = test_transform5->Sepia(unop_methodName,util, false);
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);

    }else if(methodName == "Invert")
    {
        std::cout<<"Invert TEST invoked"<<std::endl;
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Invert(methodName,util,true);
            transformed_image = test_transform2->Invert(methodName,util,true);
            transformed_image = test_transform3->Invert(methodName,util,true);
            transformed_image = test_transform4->Invert(methodName,util,true);
            transformed_image = test_transform5->Invert(methodName,util,true);
        }
        //std::cout<<"Making "<<num_runs<<" Unoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Invert(unop_methodName,util,false);
            transformed_image = test_transform2->Invert(unop_methodName,util,false);
            transformed_image = test_transform3->Invert(unop_methodName,util,false);
            transformed_image = test_transform4->Invert(unop_methodName,util,false);
            transformed_image = test_transform5->Invert(unop_methodName,util,false);
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);

    }else if(methodName == "ConvertFS")
    {
        #ifdef ANDROID
            LOGE("Captain's Log", "Convert 32F to 16S TEST");
        #else
            std::cout<<"Convert 32F to 16S TEST"<<std::endl;
        #endif
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            test_transform1->ConvertFS(methodName,util, true);
            test_transform2->ConvertFS(methodName,util, true);
            test_transform3->ConvertFS(methodName,util, true);
            test_transform4->ConvertFS(methodName,util, true);
            test_transform5->ConvertFS(methodName,util, true);
        }
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            test_transform1->ConvertFS(unop_methodName,util, false);
            test_transform2->ConvertFS(unop_methodName,util, false);
            test_transform3->ConvertFS(unop_methodName,util, false);
            test_transform4->ConvertFS(unop_methodName,util, false);
            test_transform5->ConvertFS(unop_methodName,util, false);
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);
    }else if(methodName == "GSBlur")
    {
         #ifdef ANDROID
            LOGE("Captain's Log", "Gaussian Blur TEST");
        #else
            std::cout<<"Gaussian Blur TEST"<<std::endl;
        #endif
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->GSBlur(methodName,util, true);
            transformed_image = test_transform2->GSBlur(methodName,util, true);
            transformed_image = test_transform3->GSBlur(methodName,util, true);
            transformed_image = test_transform4->GSBlur(methodName,util, true);
            transformed_image = test_transform5->GSBlur(methodName,util, true);
        }
        //util->ShowImage(transformed_image,"Optimized Blur");
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->GSBlur(unop_methodName,util, false);
            transformed_image = test_transform2->GSBlur(unop_methodName,util, false);
            transformed_image = test_transform3->GSBlur(unop_methodName,util, false);
            transformed_image = test_transform4->GSBlur(unop_methodName,util, false);
            transformed_image = test_transform5->GSBlur(unop_methodName,util, false);
        }
        //util->ShowImage(transformed_image,"UNOptimized Blur");
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);
    }else if(methodName == "Threshold")
    {
        RunThresh(cv::THRESH_BINARY);
        RunThresh(cv::THRESH_BINARY_INV);
        RunThresh(cv::THRESH_TOZERO);
        RunThresh(cv::THRESH_TOZERO_INV);
        RunThresh(cv::THRESH_TRUNC);
        
        RunThresh16(cv::THRESH_BINARY);
        RunThresh16(cv::THRESH_BINARY_INV);
        RunThresh16(cv::THRESH_TOZERO);
        RunThresh16(cv::THRESH_TOZERO_INV);
        RunThresh16(cv::THRESH_TRUNC);

    }else if(methodName == "Sobel")
    {
        #ifdef ANDROID
            LOGE("Captain's Log", "Sobel TEST");
        #else
            std::cout<<"Sobel TEST"<<std::endl;
        #endif
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Sobel(methodName,util, true);
            transformed_image = test_transform2->Sobel(methodName,util, true);
            transformed_image = test_transform3->Sobel(methodName,util, true);
            transformed_image = test_transform4->Sobel(methodName,util, true);
            transformed_image = test_transform5->Sobel(methodName,util, true);
        }
        //util->ShowImage(transformed_image,"Optimized Sobel");
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->Sobel(unop_methodName,util, false);
            transformed_image = test_transform2->Sobel(unop_methodName,util, false);
            transformed_image = test_transform3->Sobel(unop_methodName,util, false);
            transformed_image = test_transform4->Sobel(unop_methodName,util, false);
            transformed_image = test_transform5->Sobel(unop_methodName,util, false);
        }
        //util->ShowImage(transformed_image,"UNOptimized Sobel");
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);
    }else if(methodName == "EdgeDetect")
    {
         #ifdef ANDROID
            LOGE("Captain's Log", "Edge Detection TEST");
        #else
            std::cout<<"Edge Detection TEST"<<std::endl;
        #endif
        //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->EdgeDetect(methodName,util, true);
            transformed_image = test_transform2->EdgeDetect(methodName,util, true);
            transformed_image = test_transform3->EdgeDetect(methodName,util, true);
            transformed_image = test_transform4->EdgeDetect(methodName,util, true);
            transformed_image = test_transform5->EdgeDetect(methodName,util, true);
        }
        //util->ShowImage(transformed_image,"Optimized Sobel");
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            transformed_image = test_transform1->EdgeDetect(unop_methodName,util, false);
            transformed_image = test_transform2->EdgeDetect(unop_methodName,util, false);
            transformed_image = test_transform3->EdgeDetect(unop_methodName,util, false);
            transformed_image = test_transform4->EdgeDetect(unop_methodName,util, false);
            transformed_image = test_transform5->EdgeDetect(unop_methodName,util, false);
        }
        //util->ShowImage(transformed_image,"UNOptimized Sobel");
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);
        //util->ShowImage(transformed_image,"Edge Detected");
    }

    //util->ShowInfo();
}

void Harness::RunThresh(int thresh_type)
{
        std::string methodName,unop_methodName, output;
        if(thresh_type == cv::THRESH_BINARY)
        {
            methodName = static_cast<std::string>("THRESH_BINARY");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_BINARY_INV)
        {
            methodName = static_cast<std::string>("THRESH_BINARY_INV");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_TOZERO)
        {
            methodName = static_cast<std::string>("THRESH_TOZERO");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_TOZERO_INV)
        {
            methodName = static_cast<std::string>("THRESH_TOZERO_INV");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_TRUNC)
        {
            methodName = static_cast<std::string>("THRESH_TRUNC");
            unop_methodName = "UNOP_" + methodName;
        }
        output = methodName + " TEST";
        #ifdef ANDROID
            LOGE("Captain's Log",output.c_str());
        #else
            std::cout<<methodName<<" TEST"<<std::endl;
        #endif 
            //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            test_transform1->Threshold(methodName,util, true, thresh_type);
            test_transform2->Threshold(methodName,util, true, thresh_type);
            test_transform3->Threshold(methodName,util, true, thresh_type);
            test_transform4->Threshold(methodName,util, true, thresh_type);
            test_transform5->Threshold(methodName,util, true, thresh_type);
        }
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            test_transform1->Threshold(unop_methodName,util, false, thresh_type);
            test_transform2->Threshold(unop_methodName,util, false, thresh_type);
            test_transform3->Threshold(unop_methodName,util, false, thresh_type);
            test_transform4->Threshold(unop_methodName,util, false, thresh_type);
            test_transform5->Threshold(unop_methodName,util, false, thresh_type);
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);
}

void Harness::RunThresh16(int thresh_type)
{
        std::string methodName,unop_methodName, output;
        if(thresh_type == cv::THRESH_BINARY)
        {
            methodName = static_cast<std::string>("THRESH_BINARY_16");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_BINARY_INV)
        {
            methodName = static_cast<std::string>("THRESH_BINARY_INV_16");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_TOZERO)
        {
            methodName = static_cast<std::string>("THRESH_TOZERO_16");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_TOZERO_INV)
        {
            methodName = static_cast<std::string>("THRESH_TOZERO_INV_16");
            unop_methodName = "UNOP_" + methodName;
        }else if(thresh_type == cv::THRESH_TRUNC)
        {
            methodName = static_cast<std::string>("THRESH_TRUNC_16");
            unop_methodName = "UNOP_" + methodName;
        }
        output = methodName + " TEST";
        #ifdef ANDROID
            LOGE("Captain's Log",output.c_str());
        #else
            std::cout<<methodName<<" TEST"<<std::endl;
        #endif 
            //std::cout<<"Making "<<num_runs<<" optimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            test_transform1->Threshold16(methodName,util, true, thresh_type);
            test_transform2->Threshold16(methodName,util, true, thresh_type);
            test_transform3->Threshold16(methodName,util, true, thresh_type);
            test_transform4->Threshold16(methodName,util, true, thresh_type);
            test_transform5->Threshold16(methodName,util, true, thresh_type);
        }
        //std::cout<<"Making "<<num_runs<<" UNoptimized runs...."<<std::endl; 
        for(int i=0; i<num_runs; i++)
        {    
            test_transform1->Threshold16(unop_methodName,util, false, thresh_type);
            test_transform2->Threshold16(unop_methodName,util, false, thresh_type);
            test_transform3->Threshold16(unop_methodName,util, false, thresh_type);
            test_transform4->Threshold16(unop_methodName,util, false, thresh_type);
            test_transform5->Threshold16(unop_methodName,util, false, thresh_type);
        }
        if(print_debug)
        {
            util->Show_Timer_Log(methodName);
            //util->Show_Tick_Log(methodName);
            util->Show_Timer_Log(unop_methodName);
            //util->Show_Tick_Log(unop_methodName);
        }
        util->Show_Average_Time(methodName);
        util->Show_Average_Time(unop_methodName);
        //util->Show_Average_Ticks(methodName);
        //util->Show_Average_Ticks(unop_methodName);

        util->Show_Speedup(methodName);
}
