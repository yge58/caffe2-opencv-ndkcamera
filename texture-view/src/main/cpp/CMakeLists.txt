cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CPP_DIR ${CMAKE_SOURCE_DIR})
set(LIB_DIR ${CMAKE_SOURCE_DIR}/../libs/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror")

# Export ANativeActivity_onCreate(), Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

######################################   Includes    ###############################################

include_directories(    ${CPP_DIR}
                        ${CPP_DIR}/camera
                        ${CPP_DIR}/opencv
                        ${CPP_DIR}/opencv2
                        ${CPP_DIR}/caffe2
                        ${CPP_DIR}/Eigen
                        ${CPP_DIR}/protobuf
                   )

######################################   CPU feature   #############################################

include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()

######################################   NDK module   ##############################################

add_library(    ndk-module SHARED
                ${CPP_DIR}/android_main.cpp
                ###  camera  ###
                ${CPP_DIR}/camera/image_reader.cpp
                ${CPP_DIR}/camera/camera_manager.cpp
                ${CPP_DIR}/camera/camera_engine.cpp
                ${CPP_DIR}/camera/camera_listeners.cpp
                ${CPP_DIR}/camera/camera_utils.cpp
           )

########################################  OpenCV 3rdparty  #########################################

add_library( IlmImf STATIC IMPORTED )
set_target_properties( IlmImf PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/libIlmImf.a )


add_library( libjasper STATIC IMPORTED )
set_target_properties( libjasper  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/liblibjasper.a )


add_library( libjpeg STATIC IMPORTED )
set_target_properties( libjpeg  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/liblibjpeg.a )


add_library( libpng STATIC IMPORTED )
set_target_properties( libpng  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/liblibpng.a )


add_library( libtiff STATIC IMPORTED )
set_target_properties( libtiff  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/liblibtiff.a )

add_library( tbb STATIC IMPORTED )
set_target_properties( tbb  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/libtbb.a )



########################################  OpenCV   #################################################

add_library( opencv_core STATIC IMPORTED )
set_target_properties(  opencv_core PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/libopencv_core.a )

add_library( opencv_highgui STATIC IMPORTED )
set_target_properties( opencv_highgui  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/libopencv_highgui.a )

add_library( opencv_imgproc STATIC IMPORTED )
set_target_properties( opencv_imgproc  PROPERTIES IMPORTED_LOCATION
                        ${LIB_DIR}/${ANDROID_ABI}/opencv/libopencv_imgproc.a )




########################################  Caffe2    ################################################

add_library(
    caffe2
    STATIC
    IMPORTED
    )
set_target_properties(
    caffe2
    PROPERTIES IMPORTED_LOCATION
    ${LIB_DIR}/${ANDROID_ABI}/libCaffe2_CPU.a
    )
add_library(
    thread_pool
    STATIC
    IMPORTED
    )
set_target_properties(
    thread_pool
    PROPERTIES IMPORTED_LOCATION
    ${LIB_DIR}/${ANDROID_ABI}/libCAFFE2_PTHREADPOOL.a
    )
add_library(
    glog
    SHARED
    IMPORTED
    )
set_target_properties(
    glog
    PROPERTIES IMPORTED_LOCATION
    ${LIB_DIR}/${ANDROID_ABI}/libglog.so
    )

add_library(
    protobuf
    SHARED
    IMPORTED
    )
set_target_properties(
    protobuf
    PROPERTIES IMPORTED_LOCATION
    ${LIB_DIR}/${ANDROID_ABI}/libprotobuf.a
    )

add_library(
    NNPACK
    STATIC
    IMPORTED
    )
set_target_properties(
    NNPACK
    PROPERTIES IMPORTED_LOCATION
    ${LIB_DIR}/${ANDROID_ABI}/libCAFFE2_NNPACK.a
    )



########################################  Link libraries  ##########################################
# Libraries order matters. The order is from bottom to top, that is, if library A is depending on B.
# Put B at bottom, A on top.
# for example, opencv order: libopencv_calib3d opencv_features2d opencv_flann opencv_imgproc opencv_core

target_link_libraries(  ndk-module
                        -Wl,--whole-archive
                        caffe2
                        -Wl,--no-whole-archive
                        NNPACK
                        thread_pool
                        protobuf
                        android
                        log
                        m
                        z
                        cpufeatures
                        camera2ndk
                        mediandk
                        opencv_imgproc
                        opencv_highgui
                        opencv_core
                        IlmImf
                        libjpeg
                        libpng
                        libtiff
                        libjasper
                        tbb
                     )

