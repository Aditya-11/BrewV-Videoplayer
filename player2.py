# BrewV written by Aditya Dubey

"""
    ____                    __    __
   / __ )__________   _  __|  |  / /
  / __  / ___/ _ \ | /| / /|  | / / 
 / /_/ / /  /  __/ |/ |/ / |  |/ /  
/_____/_/   \___/|__/|__/  |____/   
"""

import platform
import os
import sys
import time
#from pymediainfo import MediaInfo
from PyQt5 import QtWidgets, QtGui, QtCore
import vlc
import threading
from subprocess import *
import signal


stop = False

lc = 1
lc_prev = 0
str1 = 'none' 
str3 = []
prev1 = 'on'
#m = 'd'

def decision_master(lis1):
    global prev1
    global m
    #print('hop',len(str3))
    #print(str3[len(str3)-1])
    #print(str3[len(str3)-2])
    if (lis1[len(lis1)-1] == lis1[len(lis1)-2]=='fast-forward'):
        m = 'b'
        prev1 = 'on'
    elif (lis1[len(lis1)-1] == lis1[len(lis1)-2]=='fast-backward'):
        m = 'c'
        prev1 = 'on'
    elif (lis1[len(lis1)-1] == lis1[len(lis1)-2]=='volume-up'):
        m = 'f'
        prev1  = 'on'
    elif (lis1[len(lis1)-1] == lis1[len(lis1)-2]=='volume-down'):
        m = 'e'
        prev1 = 'on'
    #elif (lis1[len(lis1)-1] == lis1[len(lis1)-2]=='none' and prev1 == 'on'):
    elif (lis1[len(lis1)-1] =='none' and prev1 == 'on'):
        m = 'a'
        prev1 = 'off'   
    #elif (lis1[len(lis1)-1] == lis1[len(lis1)-2]=='none' and prev1 == 'off'):
    elif (lis1[len(lis1)-1] == 'none' and prev1 == 'off'):
        return 'd'
    else:
        if prev1 == 'off':
            prev1 = 'on'
            m = 'a'
        else:
            m = 'd'
    print(m,prev1)
    return m

def decision(lis):
    global lc
    global lc_prev
    global str1
    global str3
    lc = len(lis)
    #if(lc>5):
    #    print(str(lis[lc-4]))
    print(lc,lc_prev)
    if lc>lc_prev:
        if lc>8:
            cface = str(lis[(lc-5):lc]).count('face')
            cpalm = str(lis[(lc-5):lc]).count('palm')
            cfist = str(lis[(lc-5):lc]).count('fist')
            cnone = str(lis[(lc-3):lc]).count('none')
            print('cnone-->',cnone)
            print('cface-->',cface,'cpalm-->',cpalm,'cfist-->',cfist)
            if cface>1:
                if cfist>cpalm and cfist>1:
                    str1 = 'fast-backward'
                elif cfist<cpalm and cpalm>1:
                    str1 = 'fast-forward'
                else:
                    str1 = 'Buffering-head'
            elif (cnone>1):
                str1 = 'none'
            elif cface==0:
                if cfist>cpalm and cfist>1:
                    str1 = 'volume-down'
                elif cfist<cpalm and cpalm>1:
                    str1 = 'volume-up'
                elif cfist==0 and cpalm==0:
                    if cnone>1:
                        str1 = 'none'      
        else:
            str1 = 'Buffering'
        #lc_prev = lc
    else: 
        str1 = 'None'
    str3.append(str1)
    str1 = decision_master(str3)
    lc_prev = lc
    return str1

def follow(thefile):
    thefile.seek(0,2)
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.2)
            continue
        yield line


class Player(QtWidgets.QMainWindow):
    """A simple Media Player using VLC and Qt
    """

    def __init__(self, master=None):
        QtWidgets.QMainWindow.__init__(self, master)
        self.setWindowTitle("Media Player")

        # Create a basic vlc instance
        self.instance = vlc.Instance()

        self.media = None

        # Create an empty vlc media player
        self.mediaplayer = self.instance.media_player_new()

        self.create_ui()
        self.is_paused = False

    def create_ui(self):
        """Set up the user interface, signals & slots
        """
        self.widget = QtWidgets.QWidget(self)
        self.setCentralWidget(self.widget)

        # In this widget, the video will be drawn
        if platform.system() == "Darwin": # for MacOS
            self.videoframe = QtWidgets.QMacCocoaViewContainer(0)
        else:
            self.videoframe = QtWidgets.QFrame()

        self.palette = self.videoframe.palette()
        self.palette.setColor(QtGui.QPalette.Window, QtGui.QColor(0, 0, 0))
        self.videoframe.setPalette(self.palette)
        self.videoframe.setAutoFillBackground(True)

        self.positionslider = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
        self.positionslider.setToolTip("Position")
        self.positionslider.setMaximum(1000)
        self.positionslider.sliderMoved.connect(self.set_position)
        self.positionslider.sliderPressed.connect(self.set_position)

        self.hbuttonbox = QtWidgets.QHBoxLayout()
        self.playbutton = QtWidgets.QPushButton("Play")
        self.hbuttonbox.addWidget(self.playbutton)
        self.playbutton.clicked.connect(self.play_pause)

        self.stopbutton = QtWidgets.QPushButton("Stop")
        self.hbuttonbox.addWidget(self.stopbutton)
        self.stopbutton.clicked.connect(self.stop)

        self.hbuttonbox.addStretch(1)
        self.volumeslider = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
        self.volumeslider.setMaximum(100)
        self.volumeslider.setValue(self.mediaplayer.audio_get_volume())
        self.volumeslider.setToolTip("Volume")
        self.hbuttonbox.addWidget(self.volumeslider)
        self.volumeslider.valueChanged.connect(self.set_volume)

        self.vboxlayout = QtWidgets.QVBoxLayout()
        self.vboxlayout.addWidget(self.videoframe)
        self.vboxlayout.addWidget(self.positionslider)
        self.vboxlayout.addLayout(self.hbuttonbox)

        self.widget.setLayout(self.vboxlayout)

        menu_bar = self.menuBar()

        # File menu
        file_menu = menu_bar.addMenu("File")

        # Add actions to file menu
        open_action = QtWidgets.QAction("Load Video", self)
        close_action = QtWidgets.QAction("Close App", self)
        file_menu.addAction(open_action)
        file_menu.addAction(close_action)

        open_action.triggered.connect(self.open_file)
        close_action.triggered.connect(sys.exit)

        self.timer = QtCore.QTimer(self)
        self.timer.setInterval(100)
        self.timer.timeout.connect(self.update_ui)

    def play_pause(self):
        """Toggle play/pause status
        """
        if self.mediaplayer.is_playing():
            self.mediaplayer.pause()
            self.playbutton.setText("Play")
            self.is_paused = True
            self.timer.stop()
        else:
            if self.mediaplayer.play() == -1:
                self.open_file()
                return

            self.mediaplayer.play()
            self.playbutton.setText("Pause")
            self.timer.start()
            self.is_paused = False

    def stop(self):
        """Stop player
        """
        self.mediaplayer.stop()
        self.playbutton.setText("Play")

    def open_file(self):
        """Open a media file in a MediaPlayer
        """

        dialog_txt = "Choose Media File"
        filename = QtWidgets.QFileDialog.getOpenFileName(self, dialog_txt, os.path.expanduser('~'))
        if not filename:
            return

        # getOpenFileName returns a tuple, so use only the actual file name
        self.media = self.instance.media_new(filename[0])

        # Put the media in the media player
        self.mediaplayer.set_media(self.media)

        # Parse the metadata of the file
        self.media.parse()

        # Set the title of the track as window title
        self.setWindowTitle(self.media.get_meta(0))

        # The media player has to be 'connected' to the QFrame (otherwise the
        # video would be displayed in it's own window). This is platform
        # specific, so we must give the ID of the QFrame (or similar object) to
        # vlc. Different platforms have different functions for this
        if platform.system() == "Linux": # for Linux using the X Server
            self.mediaplayer.set_xwindow(int(self.videoframe.winId()))
        elif platform.system() == "Windows": # for Windows
            self.mediaplayer.set_hwnd(int(self.videoframe.winId()))
        elif platform.system() == "Darwin": # for MacOS
            self.mediaplayer.set_nsobject(int(self.videoframe.winId()))

        self.play_pause()

    def set_volume(self, volume):
        """Set the volume
        """
        self.mediaplayer.audio_set_volume(volume)

    def set_position(self):
        """Set the movie position according to the position slider.
        """

        # The vlc MediaPlayer needs a float value between 0 and 1, Qt uses
        # integer variables, so you need a factor; the higher the factor, the
        # more precise are the results (1000 should suffice).

        # Set the media position to where the slider was dragged
        self.timer.stop()
        pos = self.positionslider.value()
        self.mediaplayer.set_position(pos / 1000.0)
        self.timer.start()

    def update_ui(self):
        """Updates the user interface"""

        # Set the slider's position to its corresponding media position
        # Note that the setValue function only takes values of type int,
        # so we must first convert the corresponding media position.
        media_pos = int(self.mediaplayer.get_position() * 1000)
        self.positionslider.setValue(media_pos)

        # No need to call this function if nothing is played
        if not self.mediaplayer.is_playing():
            self.timer.stop()

            # After the video finished, the play button stills shows "Pause",
            # which is not the desired behavior of a media player.
            # This fixes that "bug".
            if not self.is_paused:
                self.stop()

def haar1Loop(player, lis11, x,stop):
    while not stop:
        logfile = open("out.txt","r")
        loglines = follow(logfile)
        for line in loglines:
            #print(line,type(line))
            time.sleep(0.0003)
            lis11.append(str(line))
            x = decision(lis11)
            if (x=='a'):
                player.play_pause()
            elif (x=='b'): # fast forworading
                try:
                    a1 = player.positionslider.value()
                    pos = a1+4
                    player.mediaplayer.set_position(pos / 1000.0)
                except Exception as e:
                    print(e)
            elif (x == 'c'): #fast backwarding
                try:
                    a1 = player.positionslider.value()
                    pos = a1 - 4
                    if (pos<0):
                        pos = 0
                    player.mediaplayer.set_position(pos / 1000.0)
                except Exception as e:
                    print(e)
            elif (x == 'f'): #volume up
                try:
                    vol = int(player.mediaplayer.audio_get_volume())
                    if (vol==100 or vol == 98):
                        vol = 100
                    else:
                        vol = vol + 14
                        player.mediaplayer.audio_set_volume(vol)
                except Exception as e:
                    print(e)

            elif (x == 'e'):
                try:
                    vol = int(player.mediaplayer.audio_get_volume())
                    if (vol==0):
                        vol = 0
                    else:
                        vol = vol - 14
                        player.mediaplayer.audio_set_volume(vol)
                except Exception as e:
                    print(e) 
            elif (x=='q'):
                player.stop()
                break
    sys.exit(0)
    #exit(0)

"""
def signal_handler(sig, frame):
        print('You pressed Ctrl+C!')
        sys.exit(0)
"""


def main():
    """Entry point for our simple vlc player
    """
    global stop
    stop = False
    app = QtWidgets.QApplication(sys.argv)
    player = Player()
    player.show()
    player.resize(640, 480)
    time.sleep(2)

    #length = 640
    #breadth = 480
    #player.resize(800, 800)
    #media_info = MediaInfo.parse('skivideo.mp4')
    #duration in milliseconds
    #duration_in_ms = media_info.tracks[0].duration
    #timeout = time.time() + duration_in_ms/1000
    x = 'd'
    lis11 = []
    #handle = Popen('./haar1', stdin=PIPE, stderr=PIPE, stdout=PIPE, shell=True)
    handle = Popen(["./haar1"], stdin=None, stdout=open(os.devnull, 'wb'), stderr=open(os.devnull, 'wb'))
    t1 = threading.Thread(target=haar1Loop, args=(player,lis11,x,stop))
    t1.start()
    if app.exec_()==0:
        handle.kill()
        #signal.signal(signal.SIGINT, signal_handler)
        #t1.join()
        stop = True
        #t1.join()
        f = open('out.txt', 'r+')
        f.truncate(0)
        os._exit(0)


    #sys.exit(app.exec_())
    #f = open('out.txt', 'r+')
    #f.truncate(0)
if __name__ == "__main__":
    main()
