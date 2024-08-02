#ifndef HANDLE_VIDEO_H
#define HANDLE_VIDEO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "handleVisual.h"

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include <vector>
extern "C" {
#endif

    // Struct for handling video
    typedef struct {
        std::vector<cv::VideoCapture> captures;
        std::vector<GtkWidget*> videoDrawingAreas;
        std::vector<bool> videoOpenFlags;
        std::vector<size_t> timeouts;
    } VideoHandler;

    gboolean openVideo(VideoHandler* handler, const char* filename);
    bool getFrame(VideoHandler* handler, size_t index, unsigned char** frameData, int* width, int* height);
    void releaseVideo(VideoHandler* handler, size_t index);
    void releaseAllVideos(VideoHandler* handler);
    gboolean drawVideoFrame(GtkWidget* widget, cairo_t* cr, gpointer data);
    gboolean refreshVideoFrame(gpointer data);
    void onVideoDrawingAreaDestroy(GtkWidget* widget, gpointer data);
    void handleVideo(int x, int y, int width, int height, const char* filename);

#ifdef __cplusplus
}
#endif

#endif // HANDLE_VIDEO_H
