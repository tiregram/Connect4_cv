# Intro

This repository is the computer vision part to detect and capture the current state of the game. 

We have no dependencies of Nao on this repository.
# Need

You need to have:
Opencv_version > 3 
compiler with  c++11 enable
linux or mac OS X computer
a webcam or a ip webcam (you can create one with your phone (https://play.google.com/store/apps/details?id=com.pas.webcam))

# Compile

```bash
cmake .
# or to build cmake IP cam tool 
# cmake . -DBUILD_IP_CAM 
make
```

This example was compiled sucesfuly on a linux and mac OS_X computer.

# run

The compilation without the option will result of a dynamic lib named libc4_cv.so.
If BUILD_IP_CAN are activate you will find a executable.

```bash
./c4_cv_ip_cam CAM FILE_CONF MODE
```

IP_CAM:
Two way:
-> hardware: /dev/video0 webcam
-> IP: tcp://128.39.75.111:3001

FILE_CONF:
-> this one is provide by default on this repository over the name of save.cvconf

content of this file
```json
{
"board":{[min_h,min_s,min_v],[max_h, max_s, max_v]},
"piece_1":{[min_h,min_s,min_v],[max_h, max_s, max_v]},
"piece_2":{[min_h,min_s,min_v],[max_h, max_s, max_v]},
"distance":100,
"blur":5,
"size_remap":[200 x 200],
"size_remap_margin":[5 x 5]
}
```

 MODE: 3 modes are available.
 -> config: to improve the config file
 -> Base: to test without the security of prediction. 
 -> optim: to follow the game

# run on nao over a connection
Ssh on it and run this command to launch the camera on a tcp server.

```bash
gst-launch-0.10 -v v4l2src device=/dev/video1 ! video/x-raw-yuv,width=640,height=480,framerate=30/1 ! ffmpegcolorspace ! jpegenc ! multipartmux! tcpserversink port=3001
```

and after refer to the run section. 

# contact 
arthur margerit: ruhtra.mar@gmail.com
derek burrell: derekburrell@gmail.com 
Marveen Parras: marveen.parras@gmail.com
