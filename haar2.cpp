 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 #include <iostream>
 #include <vector>
 #include <string>
 #include <stdio.h>
 #include <fstream>
 using namespace std;
 using namespace cv;

/*
    ____                    __    __
   / __ )___________  _  __|  |  / /
  / __  / ___/ _ \ | /| / /|  | / / 
 / /_/ / /  /  __/ |/ |/ / |  |/ /  
/_____/_/   \___/|__/|__/  |____/  version 10^-9*1 written By Aditya Dubey

*/


 /** Function Headers */
void detectAndDisplay( Mat frame );
//String hand_cascade_name = "Hand.Cascade.1.xml";
//String hand_cascade_name = "hand.xml";
String hand_cascade_name = "hand.xml";
String fist_cascade_name = "fist.xml";
String face_cascade_name = "haarcascade_frontalface_default.xml";
//String left_cascade_name = "left.xml";
CascadeClassifier hand_classifier;
CascadeClassifier fist_classifier;
CascadeClassifier face_classifier;
//CascadeClassifier left_classifier;
string window_name = "Capture - Face detection";

int threshold_value = 80;
int threshold_type = 3;
int const max_binary_value = 225;

//ofstream outfile1("hand.txt");
//ofstream outfile2("fist.txt");
//ofstream outfile3("face.txt");
ofstream outfile("out.txt");

 RNG rng(12345);
 /** @function main */
 int main( int argc, const char** arg )
 {
   //char filename[] = "out.txt"
   //freopen ("out.txt", "w", stdout);
   VideoCapture capture(0);

   Mat frame;
   //-- 1. Load the cascades
   if(  !hand_classifier.load( hand_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; }
   if ( !fist_classifier.load( fist_cascade_name) ) { printf(" Error \n"); return -1; }
   if ( !face_classifier.load( face_cascade_name)) { printf(" Error \n"); return -1; }
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
       if( (char)c == 'q' ) { break; }
       //if( (char)c == 'c' ) { goto L1; }
      }

      outfile.close();
      cout<<"cmae"<<"\n";
      //outfile2.close();
      //outfile3.close();
      //if (remove("out.txt")!=0) perror("File deletion failed");
      //else cout << "File deleted successfully"; 
      std::ofstream ofs;
      ofs.open("out.txt", std::ofstream::out | std::ofstream::trunc);
      ofs.close();
      //remove("fist.txt");
      //remove("face.txt");
      //fclose (stdout);
   return 0; 
 }

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  int s1 = 0;
  std::vector<Rect> hand;
  std::vector<Rect> fist;
  std::vector<Rect> face;
  std::vector<Rect> left;
  Mat frame_gray,mask;
  //mask = frame;

  //blur(frame,frame,Size(3,3)); 
  cvtColor( frame, mask ,COLOR_BGR2GRAY );
  //equalizeHist( frame, frame_gray );
  //threshold( frame, frame_gray, threshold_value, max_binary_value, threshold_type );
  //threshold(frame,frame_gray,150,180,THRESH_BINARY);
  //adaptiveThreshold(frame, frame_gray, 255 , ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY ,11,2);
  //imshow( "Window-2", frame_gray );
  hand_classifier.detectMultiScale( frame, hand, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

for( size_t i = 0; i < hand.size(); i++ )
{
//Point center(hand[i].x *1.5+ hand[i].width*0.5, hand[i].y*1.5 + hand[i].height*0.5);
//cv::rectangle(frame, hand[i], cv::Scalar(255, 255, 255), 5);
//imshow(window_name,hand[i]);
//cout<<hand[i]<<"\n";
if (hand[i].width>175 && hand[i].height>175)// cout<<"palm"<<"width:  "<<hand[i].width<<"  height: "<<hand[i].height<<"\n";
{
//cout<<"palm"<<"\n";
//Point center(hand[i].x *1.5+ hand[i].width*0.5, hand[i].y*1.5 + hand[i].height*0.5);
//cv::rectangle(frame, hand[i], cv::Scalar(255, 255, 255), 5);
outfile << "palm" << endl;
}
else s1++;
}

if (hand.size()==0) s1++;

//fist_classifier.detectMultiScale( frame_gray, fist, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
fist_classifier.detectMultiScale( frame, fist, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

for ( size_t i = 0; i < fist.size(); i++ )
{
//Point center(fist[i].x *1.5+ fist[i].width*0.5, fist[i].y*1.5 + fist[i].height*0.5);
//cv::rectangle(frame, fist[i], cv::Scalar(255, 255, 255), 5);
//imshow(window_name,fist[i]);
if (fist[i].height>125 && fist[i].width > 125) //cout<<"fist-->height  "<<fist[i].height<<"  fist-->width  "<<fist[i].width <<"\n";
{
//cout<<"fist"<<"\n";
//Point center(fist[i].x *1.5+ fist[i].width*0.5, fist[i].y*1.5 + fist[i].height*0.5);
//cv::rectangle(frame, fist[i], cv::Scalar(255, 255, 255), 5);
outfile << "fist" << endl;
}
//else s1++;
}

if (fist.size()==0) s1++;

// smile classifier

//smile_classifier.detectMultiScale(frame,smile, 1.7, 22);
face_classifier.detectMultiScale( mask, face, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

for (size_t i = 0;i< face.size();i++)
{
//Point center(face[i].x *2.5+ face[i].width*2.5, face[i].y*2.5 + face[i].height*2.5);
if (face[i].height>150 && face[i].width>150)
{
//Point center(face[i].x *2.5+ face[i].width*2.5, face[i].y*2.5 + face[i].height*2.5);  
//cv::rectangle(frame, face[i], cv::Scalar(255, 255, 0), 5);
//cout<<"smile-->height  "<<face[i].height<<"smile-->width "<<face[i].width <<"\n";
//cout<<"***"<<"face"<<"\n";
outfile << "face" << endl;
}
//else s1++;
//imshow("new-window", frame_gray);
}

if (face.size()==0) s1++;

if (s1 == 3) 
{
  outfile<< "none" << endl;
  //cout<<"none"<<"\n";
}
//cout<<s1<<endl;
//imshow( window_name, frame);
}
