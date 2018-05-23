#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>  
#include <string>  
#include <sstream>
using namespace cv;  

int main()  
{  
    int frame_num = 0;
    //set how long to save a video from camera
    int frame_gap = 25;	//90000 for an hour's video

    //the folder of saved video
    std::string s1;
    std::string s_video = "VideoTest";

    //Some parameters of the output video
    VideoCapture capture(0);                       //If it is a notebook, 0 opens the camera that comes with it, 1 opens the external camera
    double rate = 25.0;
    Size videoSize(640,480);  
 
    Mat frame;  
    VideoWriter *writer;
  
    while (capture.isOpened())  
    {
        //Arrived at a specified time, save the video of the period of time and start the next save 
		if (frame_num%frame_gap == 0)
		{
		    s_video = "VideoTest";		    
            std::stringstream ssFrame;
            ssFrame << frame_num/frame_gap;
            ssFrame >> s1;
		    s_video = s_video + s1 + ".avi";
		    if (frame_num!=0)
		    	delete writer;
		    writer = new VideoWriter(s_video, CV_FOURCC('M', 'J', 'P', 'G'), rate, videoSize); 
            capture >> frame;  
            *writer << frame;  
            imshow("video", frame);  
		}
		else
		{
		    capture >> frame;  
            *writer << frame;  
            imshow("video", frame); 
		}

	    if (waitKey(20) == 27)  
	    {  
	        break;  
	    }  

		frame_num++;
    }  

    return 0;   
}  
