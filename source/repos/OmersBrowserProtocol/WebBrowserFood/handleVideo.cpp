#include "handleVideo.h"

using namespace cv;

static VideoHandler handler = {};

// Function to open a video file and add it to the handler
gboolean openVideo(VideoHandler* handler, const char* filename) 
{
    if (handler == NULL || filename == NULL) 
    {
        g_print("Error: VideoHandler or file path is NULL.");
        return FALSE;
    }

    cv::VideoCapture cap(filename);
    if (!cap.isOpened())
    {
        g_print("Error: Unable to open video file %s.", filename);
        return FALSE;
    }

    handler->captures.push_back(cap);
    handler->videoOpenFlags.push_back(true);
    return TRUE;
}

// Function to get a frame from a specific video
bool getFrame(VideoHandler* handler, size_t index, unsigned char** frameData, int* width, int* height) 
{
    cv::Mat rgbFrame;
    if (index >= handler->captures.size() || !handler->captures[index].isOpened())
    {
        g_warning("Error: VideoCapture is not opened or index out of range.");
        return false;
    }

    cv::Mat frame;
    handler->captures[index] >> frame;

    if (frame.empty())
    {
        handler->captures[index].set(cv::CAP_PROP_POS_FRAMES, 0);
        handler->captures[index] >> frame; 

        if (frame.empty()) 
        {
            g_warning("Error: Frame is empty after reset.");
            return false;
        }
    }

    *width = frame.cols;
    *height = frame.rows;

    // Convert Mat to RGB format
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

    // Allocate memory for frame data
    *frameData = (unsigned char*)malloc(rgbFrame.total() * rgbFrame.elemSize());
    if (*frameData == NULL)
    {
        g_warning("Error: Unable to allocate memory for frame data.");
        return false;
    }

    std::memcpy(*frameData, rgbFrame.data, rgbFrame.total() * rgbFrame.elemSize());

    return true;
}

// Function to release a specific video
void releaseVideo(VideoHandler* handler, size_t index)
{
    if (index >= handler->captures.size()) 
    {
        g_warning("Error: Video index out of range.");
        return;
    }

    handler->captures[index].release();
    handler->videoOpenFlags[index] = false;
}

void releaseAllVideos(VideoHandler* handler)
{
    for (int i = 0; i < handler->captures.size(); ++i)
    {
        handler->captures[i].release();
    }
    handler->captures.clear();
    handler->videoOpenFlags.clear();
}

gboolean drawVideoFrame(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    int index = (int)data;
    GtkWidget* videoDrawingArea = NULL;
    unsigned char* frameData = NULL;
    int frameWidth = 0, frameHeight = 0;
    GdkPixbuf* scaledPixbuf = NULL;
    GdkPixbuf* pixbuf = NULL;
    if (index >= handler.videoDrawingAreas.size()) 
    {
        g_warning("Error: VideoDrawingArea index out of range.");
        return FALSE;
    }

    videoDrawingArea = handler.videoDrawingAreas[index];
    if (videoDrawingArea == NULL) 
    {
        return FALSE; 
    }

    if (!getFrame(&handler, index, &frameData, &frameWidth, &frameHeight) || frameData == NULL) 
    {
        g_warning("Error: No frame data available or frame data is NULL.");
        return FALSE;
    }

    if (frameWidth <= 0 || frameHeight <= 0) 
    {
        g_warning("Error: Invalid frame dimensions.");
        free(frameData);
        return FALSE;
    }

    int drawingAreaWidth = gtk_widget_get_allocated_width(widget);
    int drawingAreaHeight = gtk_widget_get_allocated_height(widget);

    double scaleX = (double)drawingAreaWidth / frameWidth;
    double scaleY = (double)drawingAreaHeight / frameHeight;
    double scale = std::min(scaleX, scaleY);

    int newWidth = static_cast<int>(frameWidth * scale);
    int newHeight = static_cast<int>(frameHeight * scale);

    int offsetX = (drawingAreaWidth - newWidth) / 2;
    int offsetY = (drawingAreaHeight - newHeight) / 2;

    pixbuf = gdk_pixbuf_new_from_data(
        frameData,
        GDK_COLORSPACE_RGB,
        FALSE,
        8,
        frameWidth,
        frameHeight,
        frameWidth * 3,
        NULL,
        NULL
    );

    if (pixbuf == NULL) 
    {
        g_warning("Error: Failed to create pixbuf from frame data.\n");
        free(frameData);
        return FALSE;
    }

    scaledPixbuf = gdk_pixbuf_scale_simple(pixbuf, newWidth, newHeight, GDK_INTERP_BILINEAR);

    if (scaledPixbuf == NULL) 
    {
        g_warning("Error: Failed to scale pixbuf.");
        g_object_unref(pixbuf);
        free(frameData);
        return FALSE;
    }

    gdk_cairo_set_source_pixbuf(cr, scaledPixbuf, offsetX, offsetY);
    cairo_paint(cr);
    g_object_unref(pixbuf);
    g_object_unref(scaledPixbuf);
    free(frameData);

    return TRUE;
}
gboolean refreshVideoFrame(gpointer data) 
{
    int index = (int)data;
    GtkWidget* videoDrawingArea = NULL;
    if (index >= handler.videoDrawingAreas.size())
    {
        g_warning("Error: VideoDrawingArea index out of range.");
        return FALSE;
    }

    videoDrawingArea = handler.videoDrawingAreas[index];
    if (videoDrawingArea) 
    {
        gtk_widget_queue_draw(videoDrawingArea);
    }
    return TRUE;
}

void handleVideo(int x, int y, int width, int height, const char* filename)
{
    GtkWidget* videoDrawingArea = NULL;
    int index = handler.captures.size();
    if (!openVideo(&handler, filename)) 
    {
        g_print("Error: Unable to open video file %s.", filename);
        return;
    }

    videoDrawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(videoDrawingArea, width, height);
    g_signal_connect(videoDrawingArea, "draw", G_CALLBACK(drawVideoFrame), (gpointer)index);
    g_signal_connect(videoDrawingArea, "destroy", G_CALLBACK(onVideoDrawingAreaDestroy), (gpointer)index);
    addWidgetToContentBox(videoDrawingArea, x, y);

    handler.videoDrawingAreas.push_back(videoDrawingArea);
    handler.timeouts.push_back(g_timeout_add(30, (GSourceFunc)refreshVideoFrame, (gpointer)index));
}

void onVideoDrawingAreaDestroy(GtkWidget* widget, gpointer data) 
{
    int index = (int)data;
    if (index >= handler.videoDrawingAreas.size()) 
    {
        g_warning("Error: VideoDrawingArea index out of range.");
        return;
    }

    handler.videoDrawingAreas[index] = NULL;
    if (handler.timeouts[index] != 0) 
    {
        g_source_remove(handler.timeouts[index]);
        handler.timeouts[index] = 0;
    }
    releaseVideo(&handler, index);
}
