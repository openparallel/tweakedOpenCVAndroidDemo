#include "Transform.hpp"

Transform::Transform(std::string imageName)
{

    /* Read in image */
    //image = cv::imread(imageName,1);

    if(!image.data)
    {
        std::cout<<"No image data"<<std::endl;
    }

}

Transform::Transform(cv::Mat img)
{
    img.copyTo(image);
    
    if(!image.data)
    {
        std::cout<<"No image data"<<std::endl;
    }
}


cv::Mat Transform::Grayscale(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat gray_image;
    util->StartTimer();
    cv::cvtColor(image,gray_image, CV_RGB2GRAY);
    util->StopTimer(methodName);
    if(useOptimized == false) 
        cv::setUseOptimized(true);
    return gray_image;
}


cv::Mat Transform::Noise(std::string methodName, Utils* util,int numPixNoise, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat noisy_image = image.clone();

    util->StartTimer();
    for(int k=0; k< numPixNoise; k++)
    {
        int i = rand()%noisy_image.cols;
        int j = rand()%noisy_image.rows;

        if (noisy_image.channels() == 1)
        {
            noisy_image.at<uchar>(j,i) = 255;
    
        }else if (noisy_image.channels() == 3)
        {
            noisy_image.at<cv::Vec3b>(j,i)[0] = 255;
            noisy_image.at<cv::Vec3b>(j,i)[1] = 255;
            noisy_image.at<cv::Vec3b>(j,i)[2] = 255;
        }
    }
    util->StopTimer(methodName);
    if(useOptimized == false) 
        cv::setUseOptimized(true);
    return noisy_image;
}


cv::Mat Transform::Sepia(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    float red,green,blue = 0;
    float fred,fgreen,fblue = 0;
    uchar ored,ogreen,oblue = 0;
    cv::Mat sepia_image = image.clone();
    int numLines    = sepia_image.rows;
    int numCols     = sepia_image.cols * image.channels();
    
    util->StartTimer();
    for(int j=0; j<numLines; j++)
    {
        uchar* imageData = sepia_image.ptr<uchar>(j);

        for(int i=0; i<numCols; i+=3)
        {
                blue = (float)imageData[i];
                green = (float)imageData[i+1];
                red = (float)imageData[i+2];
                
                fred    =   (blue*0.189 + green*0.769 + red*0.393);
                fgreen  =   (blue*0.168 + green*0.686 + red*0.349);
                fblue   =   (blue*0.242 + green*0.534 + red*0.131);
                 
                oblue = cv::saturate_cast<uchar>(fblue);
                ored = cv::saturate_cast<uchar>(fred);
                ogreen = cv::saturate_cast<uchar>(fgreen);

                imageData[i] = oblue;
                imageData[i+1] = ogreen;
                imageData[i+2] = ored;
        }
    }

    util->StopTimer(methodName);
    if(useOptimized == false) 
        cv::setUseOptimized(true);
    return sepia_image;
}

cv::Mat Transform::Invert(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat inverted_image;
    //cv::Mat gray_image, gray32f_image, grayinv_image;
    
    std::vector<cv::Mat> image8u_channels,  image32f_channels, image32finv_channels;
    if(image.cols > image.rows)
        cv::resize(image, inverted_image, cv::Size(image.rows,image.rows));
    else 
        cv::resize(image, inverted_image, cv::Size(image.cols,image.cols));

    //cv::cvtColor(image,gray_image, CV_RGB2GRAY);
    cv::split(inverted_image, image8u_channels);
    cv::split(inverted_image, image32f_channels);
    cv::split(inverted_image, image32finv_channels);

    //util->ShowImage(inverted_image,"test");
    (image8u_channels[0]).convertTo(image32f_channels[0],CV_32F);
    (image8u_channels[1]).convertTo(image32f_channels[1],CV_32F);
    (image8u_channels[2]).convertTo(image32f_channels[2],CV_32F);
    //gray_image.convertTo(gray32f_image,CV_32F);

    //std::cout<<"Image 8u Type: "<<image8u_channels[0].type()<<std::endl;
    //std::cout<<"Image 32f Type: "<<image32f_channels[0].type()<<std::endl;
    
    util->StartTimer();
    cv::invert(image32f_channels[0],image32finv_channels[0],cv::DECOMP_CHOLESKY);
    cv::invert(image32f_channels[1],image32finv_channels[1],cv::DECOMP_CHOLESKY);
    cv::invert(image32f_channels[2],image32finv_channels[2],cv::DECOMP_CHOLESKY);
    //cv::invert(gray32f_image,grayinv_image,cv::DECOMP_CHOLESKY);
    util->StopTimer(methodName);
   
    (image32finv_channels[0]).convertTo(image8u_channels[0],CV_8U);
    (image32finv_channels[1]).convertTo(image8u_channels[1],CV_8U);
    (image32finv_channels[2]).convertTo(image8u_channels[2],CV_8U);
    //grayinv_image.convertTo(gray_image,CV_8U);
    cv::merge(image8u_channels,inverted_image);
    if(useOptimized == false) 
        cv::setUseOptimized(true);
    return inverted_image;
}


void Transform::ConvertFS(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat converted_imageF,converted_imageS;

    image.convertTo(converted_imageF,CV_32F);
    util->StartTimer();
    converted_imageF.convertTo(converted_imageS,CV_16S);
    util->StopTimer(methodName);
    if(useOptimized == false) 
        cv::setUseOptimized(true);
}

cv::Mat Transform::GSBlur(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat converted_image16s,image16s,converted_image;
    //image.convertTo(image16s,CV_16S);
    util->StartTimer();
    cv::GaussianBlur(image,converted_image,cv::Size(3,3),1.5);
    util->StopTimer(methodName);
    if(useOptimized == false)
        cv::setUseOptimized(true);
    //converted_image16s.convertTo(converted_image,CV_8U);
    return converted_image;
}

void Transform::Threshold(std::string methodName, Utils* util, bool useOptimized,int thresh_type)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat converted_image;
    util->StartTimer();
    cv::threshold(image,converted_image,60,255,thresh_type);
    util->StopTimer(methodName);
    if(useOptimized == false)
        cv::setUseOptimized(true);

}

void Transform::Threshold16(std::string methodName, Utils* util, bool useOptimized,int thresh_type)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat image16s,converted_image16s,converted_image;
    image.convertTo(image16s,CV_16S);
    util->StartTimer();
    cv::threshold(image16s,converted_image16s,60,255,thresh_type);
    util->StopTimer(methodName);
    converted_image16s.convertTo(converted_image,CV_8U);
    if(useOptimized == false)
        cv::setUseOptimized(true);

}

cv::Mat Transform::Sobel(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat sobel,sobelX,sobelY,converted_image;
    double sobmin,sobmax;
    util->StartTimer();
    cv::Sobel(image,sobelX,CV_16S,1,0);
    cv::Sobel(image,sobelY,CV_16S,0,1);
    util->StopTimer(methodName);
    sobel = abs(sobelX) + abs(sobelY); // L1 norm 
    cv::minMaxLoc(sobel,&sobmin,&sobmax); // find sobel max value
    sobel.convertTo(converted_image,CV_8U,-255./sobmax,255); // converted_image = -alpha*sobel + 255
    if(useOptimized == false)
        cv::setUseOptimized(true);
    return converted_image;
}


cv::Mat Transform::EdgeDetect(std::string methodName, Utils* util, bool useOptimized)
{
    cv::setUseOptimized(useOptimized);
    cv::Mat sobel,sobelX,sobelY,sobel_image,thresh_image,threshX,threshY,thresh;
    double sobmin,sobmax;
    util->StartTimer();
    cv::Sobel(image,sobelX,CV_16S,1,0);
    cv::threshold(sobelX,threshX,200,255,cv::THRESH_BINARY);
    cv::Sobel(image,sobelY,CV_16S,0,1);
    cv::threshold(sobelY,threshY,200,255,cv::THRESH_BINARY);
    util->StopTimer(methodName);
    //sobel = abs(sobelX) + abs(sobelY); // L1 norm 
    thresh = abs(threshX) + abs(threshY);    
    cv::minMaxLoc(thresh,&sobmin,&sobmax); // find sobel max value
    //sobel.convertTo(sobel_image,CV_8U,-255./sobmax,255); // converted_image = -alpha*sobel + 255
    thresh.convertTo(thresh_image,CV_8U,-255./sobmax,255); // converted_image = -alpha*sobel + 255
    //cv::threshold(sobel_image,thresh_image,200,255,cv::THRESH_BINARY);
    //cv::threshold(thresh_image,sobel_image,200,255,cv::THRESH_BINARY);
    //util->StopTimer(methodName);
    if(useOptimized == false)
        cv::setUseOptimized(true);
    return thresh_image;
}
