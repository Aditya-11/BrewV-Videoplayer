#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int g_slider_position = 0;
int slider_position=0;
CvCapture* video_capture = NULL;

void onTrackbarSlide(int current_frame)
{
    current_frame = g_slider_position;
    cvSetCaptureProperty(video_capture,CV_CAP_PROP_POS_FRAMES,current_frame);
}

void on_trackbar( int , void* )
{
 int position=(double)slider_position;
 capture.set(CV_CAP_PROP_POS_FRAMES, position);
 //imshow("",);
}


int main( int argc, char** argv )
{
    cvNamedWindow( "Video", CV_WINDOW_AUTOSIZE );
    video_capture = cvCreateFileCapture( "skivideo.mp4");
    int no_of_frames = (int) cvGetCaptureProperty(video_capture,CV_CAP_PROP_FRAME_COUNT);
    cout<<no_of_frames<<endl;

    if( no_of_frames!= 0 ) 
    {
        cvCreateTrackbar("Slider","Video",&g_slider_position,no_of_framesonTrackbarSlied);
    }

    IplImage* frame;

    while(1) 
    {
        frame = cvQueryFrame( video_capture );
        if( !frame ) break;
        //imshow("Video",frame)
        cvShowImage( "Video", frame );
        //cvSetTrackbarPos("Slider","Video",++g_slider_position);
        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }
    cvReleaseCapture( &video_capture );
    cvDestroyWindow( "Video" );

    return(0);
}