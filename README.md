# BrewV-Videoplayer
<p align="center">
<a href="https://github.com/Aditya-11/BrewV-Videoplayer/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-Apache_2.0-Blue.svg" alt="License" /></a>
<a href="https://github.com/Aditya-11/BrewV-Videoplayer/releases/latest"><img src="https://img.shields.io/badge/release-1e--9-green.svg" alt="Latest release" /></a>
<a href="https://www.youtube.com/watch?v=bLmauUfYk4I&feature=youtu.be"><img src="https://img.shields.io/badge/demo-youtube-red.svg" alt="demo" /></a>
 <a href="https://github.com/Aditya-11/BrewV-Videoplayer/blob/master/README.md"><img src="https://img.shields.io/badge/next_version-releasing_soon-orange.svg" alt="demo" /></a>
</p>

![alt text](https://i.imgur.com/07nvIKx.png)

BrewV is a computer vision based video player which uses face and hand gestures to control the functionality of a videoplayer like forward seeking , volume control etc.
The PyQt5 library is used for the design the Gui and libvlc python bindings is used for implementing video player functionalities.
The implementation of computer vision which gives input to player is done in c++ using the concept of Haar cascade to detect the facial and hand gestures,the opencv library is used for doing this. This is just a experimental project and it is made with the intention to make a AI software.
While choosing the gestures, the trade off was between the amount of energy the user would spend for the functionality.

# Has been tested in macOs and Linux

# Installation process
* Make sure Opencv 4.0 is installed
* The software uses libvlc python bindings of libvlc so vlc player should be installed in your system
* cmake should be installed 
* go to the directory where you downloaded the player (you can edit the CMakeLists file by putting haar1 or haar2 based )
* install libraries platform and PyQt5 by using pip3 etc

To run the player --> we have to create the execcutable first

* <b>cmake .</b>
* or incase cmake unable to find Opencv 4.0 path then
* <b>cmake . -DOpenCV_DIR=<path_to_OpenCV></b>
* <b>make</b>
* <b>python3 player2.py</b>


# Usage 

The robustness of this software depends on brightness and other all noises in the surrounding. It has been tested on many cases and works well most of the time.

player1.py is for the normal mode

* run python player1.py  (for running player1.py replace the haar1 with haar2 from CMakeLists file in installation and then start installation from step 1)
* run python player2.py (to get the debug mode where you can see whether the objects are detected properly or not , here keep the CMakeLists file unchanged)

Gesture controls are : 
* 1 seek forward : show palm and face 
* 2 seek backward: show fist and face 
* 3 volume up : put head down and show palm
* 4 volume down : put head down and show fist
* 5 pause: cover the camera with hand or bend the screen of laptop upto some angle.
* 6 play: undo the step 5 or look at the camera.

* 7 After doing step 3 and 4 , you have to first put your hands down and then put the head up ,if both are done simultaneously then there are chances that due to some error the video may seek litle bit , this error will be rectified by next version.

# ToDo 
- [ ] Resolve the issue with buffers when seeking forward and backward.
- [ ] Improving the Computer Vision part for making the cpu uasge less.
- [x] The signal to volume slider at the time of gesture 3 and 4. 
- [ ] Creating the setup file.
- [ ] Include some better gestures to improve the user experience.
- [ ] Keyboard interrupt based start of the application
