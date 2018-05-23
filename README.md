# Qt application release of video monitoring program
Implementation script for Qt application release of video monitoring program

### Introduction
This book describes how to package video monitoring application developed in the qt creator IDE under Linux, so that it can run independently and stably without depending on the environment configuration in the new Linux environment.

Author: [Qichao Wu](https://github.com/deepthinking-qichao)
<br>Email: 467736413@qq.com or 13660479414@163.com

### Contents
1. [Video Monitoring Application](#video-monitoring-application)
2. [Qt Application Release](#qt-application-release)
3. [Multi camera Boot Up Sequence](#multi-camera-boot-up-sequence)
3. [Program Loop Open](#program-loop-open)

### Video Monitoring Application
We already have a qt application named <test2> that can monitor the retention of people or large items on the escalator landing floor, statistics on passenger flow in the escalator, whether there are passengers on the escalator 
and whether there is abnormal movement at the escalator handrail, and send the above monitoring situation to the escalator controller through pipeline communication, 
therefore, we also wrote a c++ program named <[last.cpp]()> for testing the pipe communication function of qt applications. The qt application <test2> cannot be uploaded to github for some reason, 
but you can write a qt program and use it to implement qt application release instead of test2.

In order to simulate the function of the video surveillance program, I wrote a C++ program based on VS2013 named <[video_save]()> that recorded surveillance video through the camera for a certain period of time. 
You can convert it into a Qt application program to verify the multi-camera open sequence and the program loop open function.

### Qt Application Release
Qt application release steps are as follows:
1. Use the Release version to compile the project in QtCreator.
2. Enter the compiled build folder, find the executable program (assuming it is test2.exe, the icon is diamond), and copy it to a path A (e.g. ./release).
3. Enter the path A, create a new shell script named <[lddTest.sh]()>. This script is in the folder 'release/' and its parameters are descripted as bellow:
```
exe="test2"                             #name of qt application
des="/home/ubuntu/project/release"      #folder of Qt application release
```
4. Save the script, modify the script permissions to be executable (e.g. chmod 777 lddTest.sh) and execute the script (e.g. sh lddTest.sh), you can see a lot of copied .so files in the release folder. 
These files are dynamic link libraries that the executable program test2.exe depends on.
5. Under path A, create a new script named <[test2.sh]()>. It must be named test2.sh, with the same name as the executable file test2.exe. This script is in the folder 'release/' and you don't need to modify any parameters inside:
6. Execute the script <[test2.sh]()> to finally run the qt application.

Through the above steps, you can complete the qt application release. You can migrate the entire qt application release folder to another machine and run it without reconfiguring the environment. 

### Multi camera Boot Up Sequence
Every time you restart tk1, there will be a change in the camera device number, which is reflected in the device in the OpenCV code 
```
VideoCapture capture(device) 
```
changes every time. This situation causes the camera and the camera processing thread to not match, and the processing result is in an error condition. Errors can only be corrected by manually modifying the device number each time.
So I wrote a rules file (suffix .rules), save in the /etc/udev/rules.d directory, and named <[10-my-webcam.rules]()>. The rules file is in the folder 'release/'. It will be automatically read when you boot, and the camera device number remapping function will be completed.

### Program Loop Open
During program execution, the video monitoring program may crash due to various reasons, or be killed by the operating system. So you need to write a script to determine if the program is normal. 
If the program crashes or is killed, you need to open the program again. The script reads as follows:
1, Loop detection of the existence of the main program process, if there is, then re-execute step 1;
2. If it is detected that the main program does not exist, kill the original remaining process (such as the communication child process), and then re-open the detection main program and communication subroutine.

So I wrote a shell script <[allsh]()> to achieve the above two functions.

#***Hope this help you***