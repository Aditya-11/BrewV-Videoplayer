#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;
Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
Mat threshold_output;
RNG rng(12345);
void thresh_callback(int, void* );
Ptr<BackgroundSubtractor> pMOG2;
int main( )
{
    VideoCapture vcap(0);
    if (!vcap.isOpened())
    {
        cout<<"Camera Cant Open"<<endl;
        return -1;
    }
bool update_bg_model = true;
for (;;)
{ 
  Mat fgMaskMOG2; 
  Mat src;
  vcap >> src;
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  pMOG2->apply(threshold_output, fgMaskMOG2);  
  cv::bitwise_and(src, src ,mask=fgMaskMOG2);
  //pMOG2->apply(threshold_output, threshold_output); 
  const char* source_window = "Source";
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src);
  //createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback(0,0);
  char c = (char) waitKey(33);
  if (c==27) break;
  if( c == ' ' )
  {
    update_bg_model = !update_bg_model;
    printf("Learn background is in state = %d\n",update_bg_model);
  }
  //waitKey(0);
}
  return(0);
}
void thresh_callback(int, void* )
{  
  Mat src_copy = src.clone();
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
  vector<vector<Point> >hull( contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     {   
  convexHull( Mat(contours[i]), hull[i], false ); }
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  cout<<contours.size()<<endl;
  cout<<hull.size()<<endl;
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       drawContours( drawing, hull, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     }
  namedWindow( "Hull demo", WINDOW_AUTOSIZE );
  imshow( "Hull demo", drawing );
}
