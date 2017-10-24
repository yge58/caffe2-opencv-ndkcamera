
# Project is about:   

Integrating Caffe2, OpenCV with Android NDK Native Camera API.

Source code is based upon the following and google ndk doc, I am extremly grateful to them,   

[1] The whole project is based on [Google NDK camera texture-view sample](https://github.com/googlesamples/android-ndk/tree/master/camera)

[2] The Caffe2 C++ classification procedure is from [Caffe2 example](https://github.com/leonardvandriel/caffe2_cpp_tutorial/blob/master/src/caffe2/binaries/pretrained.cc)

[3] The Caffe2 pretrained protobuf and related libraries are obtained from [AIcamera](https://github.com/bwasti/AICamera)

[4] The OpenCV libraries are obtained from [OpenCV4Android](https://github.com/opencv/opencv/tree/master/samples/android)      

# Preface

This is my first Android App. I am a newbie to Android development, and a C++ starter. The App is working, but is far from finishing. 
Issues or bugs such as memory leak, resource management, user experience, Java calling C++ through JNI, CMake, NDK native API... are still existing. 
Therefore, if you see any problem in the code or have any improvement suggestions, please help me by filing as many issues as you want, and I will be very grateful to them.

# Introduction

We love Caffe, OpenCV and Android, lets make an Android app including all three. 
This app aims at efficiency, I think it is the best practive to integrate Caffe, OpenCV with Android natively. The role of Java is (if I am correct) to manage App's life cycle and User Interface which are very important.

# Source Code Structure

  In texture-view,
           
              |__build
              |__build.gradle
              |__src
                   |__main
                         |__assets
                               |__squeeze_init_net.pb    (squeeze net CNN artecture file)
                               |__squeeze_predict_net.pb (squeeze net pretrained weights)
                         |__cpp
                               |__caffe2  (caffe2 headers)
                               |__camera  (Android NDK native Camera API)
                               |__Eigen   (Eigen headers)
                               |__google  (protobuf headers)
                               |__opencv  (needed for opencv2)
                               |__opencv2 (opencv2 headers)
                               |__android_main.cpp   ( 1. contains all c++ files that are called from Java via JNI)
                                                     ( 2. handles all the native activities including camera, caffe, opencv.)
                               |__classes.h (Imagenet classes)
                               |__CMakeLists.txt     ( CMake )
                         |__Java
                               |__com__sample__textureview
                                                       |__ViewActivity.java (App's life cycle, UI)
                         |__libs
                               |__armeabi-v7a (This is the only ABI supported unfortunately)
                                            |__opencv (opencv4android libraries)
                                            |__(the rest libraries are copied from AICamera)
                         |__res
                         |__AndroidManifest.xml
                               

# Screenshot
-----------
![screenshot](https://github.com/yge58/caffe2-opencv-ndkcamera/blob/master/device-2017-10-23-185701.png)

