# BrewV-Videoplayer

BrewV is a computer vision based video player which uses face and hand gestures to control the functionality of a videoplayer.
The PyQt5 library is used for the design the Gui and libvlc bindings for video player functionalities like seeking , volume control etc.
The implementation of computer vision which gives input to player is done in c++ using the concept of Haar cascade to detect the facial and hand gestures,the opencv library is used for doing this. This is just a experimental project and it isnt made with the intention to make a better video player rather than just to make a software 3.0.


# Usage demonstration 
link: https://www.youtube.com/watch?v=bLmauUfYk4I&feature=youtu.be

# Installation process
* The software uses libvlc python bindings so vlc player should be installed in your system
* cmake should be installed 
* go to the directory where you downloaded the player (you can edit the CMakeLists file by putting haar1 or haar2)
1 cmake ..
2 make  (the executables would be created)
3 install libraries platform and PyQt5 by using pip3 etc
4 python player1.py

# Usage 

The robustness of this software depends on brightness and other all noises in the surrounding. 

player1.py is the normal mode

run python player1.py  (for player1.py replace the haar1 with haar2 from CMakeLists file in installation)
run python player2.py (to get the debug mode , the screen where you see the objects are detected properly )

Gesture controls are : 1. seek forward : show palm and face 
2. seek backward: show fist and face 
3. volume up : put head down and show palm
4. volume down : put head down and show fist
5. pause: cover the camera with hand or bend the lid of laptop upto some angle.
6. play: undo the step 5 or look at the camera.

# ToDo 
1. Resolve the issue with buffers when seeking forward and backward.
2. Improving the Computer Vision part for making the cpu uasge less.
3. The signal to volume slider at the time of gesture 3 and 4. 
4. Creating the setup file.
