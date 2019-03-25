#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;
static void help()
{
    printf("\n"
            "This program demonstrated a simple method of connected components clean up of background subtraction\n"
            "When the program starts, it begins learning the background.\n"
            "You can toggle background learning on and off by hitting the space bar.\n"
            "Call\n"
            "./segment_objects [video file, else it reads camera 0]\n\n");
}
static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
    int niters = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat temp;
    dilate(mask, temp, Mat(), Point(-1,-1), niters);
    erode(temp, temp, Mat(), Point(-1,-1), niters*2);
    dilate(temp, temp, Mat(), Point(-1,-1), niters);
    findContours( temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    dst = Mat::zeros(img.size(), CV_8UC3);
    if( contours.size() == 0 )
        return;
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0, largestComp = 0;
    double maxArea = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > maxArea )
        {
            maxArea = area;
            largestComp = idx;
        }
    }
    Scalar color( 0, 0, 255 );
    drawContours( dst, contours, largestComp, color, FILLED, LINE_8, hierarchy );
}

int main(int argc, char** argv)
{
    int thresh = 100;
int max_thresh = 255;
Mat threshold_output;
    VideoCapture cap;
    bool update_bg_model = true;
    CommandLineParser parser(argc, argv, "{help h||}{@input||}");
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string input = parser.get<std::string>("@input");
    if (input.empty())
        cap.open(0);
    else
        cap.open(input);
    if( !cap.isOpened() )
    {
        printf("\nCan not open camera or video file\n");
        return -1;
    }
    Mat tmp_frame, bgmask, out_frame,src;
    cap >> tmp_frame;
    if(tmp_frame.empty())
    {
        printf("can not read data from the video source\n");
        return -1;
    }
    namedWindow("video", 1);
    namedWindow("segmented", 1);
    Ptr<BackgroundSubtractorMOG2> bgsubtractor=createBackgroundSubtractorMOG2();
    //Ptr<BackgroundSubtractorKNN> bgsubtractor = createBackgroundSubtractorKNN();
    //bgsubtractor->setVarThreshold(10);
    Mat frame;
    for(;;)
    {
        cap >> frame;
        if( tmp_frame.empty() )
            break;
        //cv::flip(src, tmp_frame, 1); 
        blur( frame,tmp_frame, Size(3,3));
        cout<<"1";
        cvtColor( tmp_frame, tmp_frame, COLOR_BGR2GRAY );
        //threshold( tmp_frame, threshold_output, thresh, 255, THRESH_BINARY );
        cout<<"2";
        //bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
        //refineSegments(tmp_frame, bgmask, threshold_output);
        bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
        cout<<"3";
        bitwise_and(frame,bgmask,threshold_output);
        cout<<"4";
        //cv::bitwise_and(tmp_frame,threshold_output,threshold_output);
        //cvtColor( threshold_output, threshold_output, COLOR_BGR2GRAY );
        cout<<"5";
        //imshow("video", threshold_output);
        imshow("segmented", threshold_output);
        // program to find countours and draw them in the image


    
    RNG rng(12345);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> >hull( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ) convexHull( Mat(contours[i]), hull[i], false ); 
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    std::cout<<contours.size()<<endl;
    std::cout<<hull.size()<<endl;
    for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       drawContours( drawing, hull, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     }
    namedWindow( "Hull demo", WINDOW_AUTOSIZE );

    imshow( "Hull demo", drawing );


        int keycode = waitKey(30);
        if( keycode == 27 )
            break;
        if( keycode == ' ' )
        {
            update_bg_model = !update_bg_model;
            printf("Learn background is in state = %d\n",update_bg_model);
        }
    }
    return 0;
}

