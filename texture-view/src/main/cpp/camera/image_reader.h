/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAMERA_IMAGE_READER_H
#define CAMERA_IMAGE_READER_H

#include <media/NdkImageReader.h>
#include <functional>

// time
#include <chrono>
#include <thread>
#include <ctime>
#include <stdio.h>

// caffe
#include <caffe2/core/predictor.h>
#include <caffe2/core/operator.h>
#include <caffe2/core/timer.h>
#include "caffe2/core/init.h"

// cv
#include "opencv2/opencv.hpp"

#define MAX_RESULT 8


/*
 * When Compiling with gnustl C++, to_string is undefined.
 * */
#include <sstream>

namespace std {
    template<class T>
    std::string to_string(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }
}

struct {
    bool operator()(std::pair<int, int> a, std::pair<int, int> b) const
    {
        return a.first > b.first;
    }
} myGreater;


/*
 * ImageFormat:
 *     A Data Structure to communicate resolution between camera and ImageReader
 */
struct ImageFormat {
    int32_t width;
    int32_t height;

    int32_t format;  // Through out this demo, the format is fixed to
    // YUV_420 format
};

class ImageReader {
public:
    /**
     * Ctor and Dtor()
     */
    explicit ImageReader(ImageFormat *res, enum AIMAGE_FORMATS format);

    ~ImageReader();

    /**
     * Report cached ANativeWindow, which was used to create camera's capture
     * session output.
     */
    ANativeWindow *GetNativeWindow(void);

    ANativeWindow *GetNativeWindowFromSurface(void);
    /**
     * Retrieve Image on the top of Reader's queue
     */
    AImage *GetNextImage(void);

    /**
    * Retrieve Image on the back of Reader's queue, dropping older images
    */
    AImage *GetLatestImage(void);

    /**
     * Delete Image
     * @param image {@link AImage} instance to be deleted
     */
    void DeleteImage(AImage *image);

    /**
     * AImageReader callback handler. Called by AImageReader when a frame is
     * captured
     * (Internal function, not to be called by clients)
     */
    void ImageCallback(AImageReader *reader);

    /**
     * DisplayImage()
     *   Present camera image to the given display buffer. Avaliable image is
     * converted
     *   to display buffer format. Supported display format:
     *      WINDOW_FORMAT_RGBX_8888
     *      WINDOW_FORMAT_RGBA_8888
     *   @param buf {@link ANativeWindow_Buffer} for image to display to.
     *   @param image a {@link AImage} instance, source of image conversion.
     *            it will be deleted via {@link AImage_delete}
     *   @return true on success, false on failure
     */
    bool DisplayImage(ANativeWindow_Buffer *buf, AImage *image);

    /**
     * Configure the rotation angle necessary to apply to
     * Camera image when presenting: all rotations should be accumulated:
     *    CameraSensorOrientation + Android Device Native Orientation +
     *    Human Rotation (rotated degree related to Phone native orientation
     */
    void SetPresentRotation(int32_t angle);

    /**
     * regsiter a callback function for client to be notified that jpeg already
     * written out.
     * @param ctx is client context when callback is invoked
     * @param callback is the actual callback function
     */
    void RegisterCallback(void *ctx, std::function<void(void *ctx, const char *fileName)>);


    void SetCaffePredictor(caffe2::Predictor *p) {this->predictor = p;};

    void SetCacheDirString(const char *dir) {this->_cache_dir = dir;};

    caffe2::Predictor *predictor;
    float avg_fps = 0.0;
    float total_fps = 0.0;
    int iters_fps = 10;

    bool _caffe_is_running;

    bool _caffe_result_is_ready;

    std::string _CaffeResult;

    const char * _cache_dir;

private:
    int32_t presentRotation_;
    AImageReader *reader_;

    std::function<void(void *ctx, const char *fileName)> callback_;
    void *callbackCtx_;

    void PresentImage(ANativeWindow_Buffer *buf, AImage *image);

    void PresentImage90(ANativeWindow_Buffer *buf, AImage *image);

    void PresentImage180(ANativeWindow_Buffer *buf, AImage *image);

    void PresentImage270(ANativeWindow_Buffer *buf, AImage *image);

    void WriteFile(AImage *image);

    void RunCaffe(AImage *image);
};

#endif  // CAMERA_IMAGE_READER_H
