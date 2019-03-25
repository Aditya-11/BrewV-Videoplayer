 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 #include <iostream>
 #include <vector>
 #include <stdio.h>
 using namespace std;
 using namespace cv;

 /** Function Headers */
void detectAndDisplay( Mat frame );
//String hand_cascade_name = "Hand.Cascade.1.xml";
//String hand_cascade_name = "hand.xml";
String hand_cascade_name = "hand.xml";
String fist_cascade_name = "fist.xml";
//String smile_cascade_name = "right.xml";
//String left_cascade_name = "left.xml";
CascadeClassifier hand_classifier;
CascadeClassifier fist_classifier;
//CascadeClassifier smile_classifier;
//CascadeClassifier left_classifier;

string window_name = "Capture - Face detection";

int threshold_value = 80;
int threshold_type = 3;
int const max_binary_value = 225;

 RNG rng(12345);
 /** @function main */
 int main( int argc, const char** arg )
 {
   VideoCapture capture(0);
   Mat frame;
   //-- 1. Load the cascades
   if(  !hand_classifier.load( hand_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; }
   if ( !fist_classifier.load( fist_cascade_name) ) { printf(" Error \n"); return -1; }
//   if ( !smile_classifier.load( smile_cascade_name)) { printf(" Error \n"); return -1; }
//   if ( !left_classifier.load( left_cascade_name)) { printf(" Error \n"); return -1; }
   else printf("done!\n");
   //-- 2. Read the video stream
     while( true )
     {
        capture >> frame;
   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { 
           detectAndDisplay( frame );
       }
       else
       { 
         printf(" --(!) No captured frame -- Break!"); break; 
       }
       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   return 0; 
 }

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  std::vector<Rect> hand;
  std::vector<Rect> fist;
  std::vector<Rect> smile;
  std::vector<Rect> left;
  Mat frame_gray,mask;

  blur(frame,frame,Size(3,3)); 
  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  //equalizeHist( frame_gray, frame_gray );
  mask = frame_gray;
  threshold( frame_gray, frame_gray, threshold_value, max_binary_value, threshold_type );
  //threshold(frame_gray,frame_gray,200,160,THRESH_BINARY);
  //adaptiveThreshold(frame_gray, frame_gray, 255 , ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY ,11,2);
  imshow( "Window-2", frame_gray );
  hand_classifier.detectMultiScale( frame_gray, hand, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

for( size_t i = 0; i < hand.size(); i++ )
{
Point center(hand[i].x *1.5+ hand[i].width*0.5, hand[i].y*1.5 + hand[i].height*0.5);
cv::rectangle(frame, hand[i], cv::Scalar(255, 255, 255), 5);
//imshow(window_name,hand[i]);
//cout<<hand[i]<<"\n";
if (hand[i].width>175 && hand[i].height>175) cout<<"palm"<<"width:  "<<hand[i].width<<"  height: "<<hand[i].height<<"\n";
}
//fist_classifier.detectMultiScale( frame_gray, fist, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
fist_classifier.detectMultiScale( frame, fist, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

for ( size_t i = 0; i < fist.size(); i++ )
{
Point center(fist[i].x *1.5+ fist[i].width*0.5, fist[i].y*1.5 + fist[i].height*0.5);
cv::rectangle(frame, fist[i], cv::Scalar(255, 255, 255), 5);
//imshow(window_name,fist[i]);
if (fist[i].height>125 && fist[i].width > 125) cout<<"fist-->height  "<<fist[i].height<<"  fist-->width  "<<fist[i].width <<"\n";
}
// right hand classifier


//right_classifier.detectMultiScale( frame, right, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
//smile_classifier.detectMultiScale(frame,smile, 1.7, 22);
/*for (size_t i = 0;i< smile.size();i++)
{
Point center(smile[i].x *1.5+ smile[i].width*0.5, smile[i].y*1.5 + smile[i].height*0.5);
cv::rectangle(frame, smile[i], cv::Scalar(0, 255, 0), 2);
cout<<"smile-->height  "<<smile[i].height<<"smile-->width "<<smile[i].width <<"\n";
}*/

// left hand classifier 
//left_classifier.detectMultiScale( mask, left, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
/*
for (size_t i = 0;i< left.size();i++)
{
Point center(left[i].x *1.5+ left[i].width*0.5, left[i].y*1.5 + left[i].height*0.5);
cv::rectangle(frame, left[i], cv::Scalar(255, 255, 255), 5);
cout<<"left-->height"<<left[i].height<<"left-->width"<<left[i].width <<"\n";
}*/
imshow( window_name, frame);
}
