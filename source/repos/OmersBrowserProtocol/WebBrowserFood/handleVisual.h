#ifndef HANDLEVISUAL_H
#define HANDLEVISUAL_H

#pragma warning(disable:4996) // Disable deprecated function warnings (for MSVC)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "handleSocket.h"

#define MIN_VID_SIZE 50


#ifdef __cplusplus
#include <opencv2/opencv.hpp>
extern "C" {
#endif

#define MARGIN 3
	void handleVisualMain(GtkWidget* widget, char* infoBuf);
	void extractTag(char* infoBuf);
	void handleTag(char* tagType, char* tagContent, int x, int y, int width, int height, char* site);
	void updateCurrentTabLabel(char* newLabel);
	void clearCurrentPage(GtkWidget* notebook);
	void paragraphManager(char* tagContent, int x, int y);
	void buttonClicked(GtkWidget* widget, gpointer data);
	void buttonManager(char* tagContent, int x, int y);
	void handleImage(int x, int y, int width, int height);
	void addWidgetToContentBox(GtkWidget* widget, int x, int y);
	void create_link(char* text, char* site, int x, int y);
	void open_link(GtkWidget* widget, gpointer data);

#ifdef __cplusplus
}
#endif


#endif // HANDLEVISUAL_H
