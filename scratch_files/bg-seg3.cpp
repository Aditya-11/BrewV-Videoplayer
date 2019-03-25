// Written by Aditya Dubey

#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/tracking.hpp"

using namespace std;
using namespace cv;

int main(int argc,char *argv[])
{
    int c = 0;
    Mat threshold_output;
    VideoCapture cap(0);


    if( !cap.isOpened() )
    {
        printf("\nCan not open camera or video file\n");
        return -1;
    }
    else printf("done!\n");
    Mat tmp_frame,gray;
    cap >> tmp_frame;
    if(tmp_frame.empty())
    {
        printf("can not read data from the video source\n");
        return -1;
    }
   while(c!=27)
   {
    // Crop image
	cvtColor(tmp_frame,gray,COLOR_BGR2GRAY);
    blur( tmp_frame,tmp_frame, Size(3,3));
    imshow("color1",gray);
	threshold(gray,gray,0,255,(THRESH_BINARY_INV+ THRESH_OTSU));
    namedWindow("gray",1);
    imshow("gray1",gray);
    c = waitKey(30);
    if (c==27) break;
   }
   return 0;
}