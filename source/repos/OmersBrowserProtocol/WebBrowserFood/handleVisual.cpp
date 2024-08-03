#include "handleVisual.h"
#include "handleVideo.h"

GtkWidget* newWindow = NULL;
GtkWidget* viewport = NULL;
GtkWidget* contentBox = NULL;

void handleVisualMain(GtkWidget* widget, char* infoBuf)
{
    newWindow = widget;
    extractTag(infoBuf);
}

void extractTag(char* infoBuf)
{
    int width = 50;
    int height = 50;
    char* openingTag = NULL;
    char* closingTag = NULL;
    char* tagType = NULL;
    char* tagContent = NULL;
    int length = 0;
    char* nextTag = NULL;
    char* endOfLine = NULL;
    char* currentPosition = infoBuf;
    char* contentStart = NULL;
    char* xPoint = NULL;
    char* yPoint = NULL;
    char* sitePoint = NULL;
    char* widthPoint = NULL;
    char* heightPoint = NULL;
    char* space = NULL;
    char* srcPoint = NULL;
    char* tagAttributes = NULL;
    char* media = NULL;
    char* sendedMedia = NULL;
    char* srcValueEnd = NULL;
    int x = 0;
    int y = 0;
    char* site = NULL;
    while ((openingTag = strchr(currentPosition, '<')) != NULL &&
        (closingTag = strchr(openingTag, '>')) != NULL)
    {
        contentStart = closingTag + 1;
        length = closingTag - openingTag - 1;
        tagType = (char*)malloc((length + 1) * sizeof(char));
        strncpy(tagType, openingTag + 1, length);
        tagType[length] = '\0';

        space = strchr(tagType, ' ');
        if (space != NULL)
        {
            *space = '\0';
            tagAttributes = space + 1;
        }
        else
        {
            tagAttributes = (char*)malloc(sizeof(char) * 3);
            strcpy(tagAttributes, "");
        }
        nextTag = strchr(contentStart, '<');
        endOfLine = strchr(contentStart, '\n');
        if (nextTag == NULL && endOfLine == NULL)
        {
            length = strlen(contentStart);
        }
        else if (nextTag != NULL && endOfLine != NULL)
        {
            length = (nextTag < endOfLine) ? (nextTag - contentStart) : (endOfLine - contentStart);
        }
        else if (nextTag != NULL)
        {
            length = nextTag - contentStart;
        }
        else
        {
            length = endOfLine - contentStart;
        }
        widthPoint = strstr(tagAttributes, "width=");
        heightPoint = strstr(tagAttributes, "height=");
        xPoint = strstr(tagAttributes, "x=");
        yPoint = strstr(tagAttributes, "y=");
        sitePoint = strstr(tagAttributes, "location=");
        srcPoint = strstr(tagAttributes, "src=");
        if (heightPoint != NULL)
        {
            height = atoi(heightPoint + 7);
            printf("height=%d\n", height);
        }
        if (widthPoint != NULL)
        {
            width = atoi(widthPoint + 6);
            printf("width=%d\n", width);
        }
        if (xPoint != NULL)
        {
            x = atoi(xPoint + 2);
            printf("x=%d\n", x);
        }
        if (sitePoint != NULL)
        {
            site = sitePoint + 9;
            printf("location=%s\n", site);
        }
        if (yPoint != NULL)
        {
            y = atoi(yPoint + 2);
            printf("y=%d\n", y);
        }
        if (srcPoint != NULL)
        {
            media = srcPoint + 4;
            srcValueEnd = strpbrk(media, " \t\n\r>");
            if (srcValueEnd != NULL)
            {
                *srcValueEnd = '\0';
            }
            printf("src=%s\n", media);
            sendedMedia = (char*)malloc(sizeof(char) * (strlen(media) + strlen("IMAGE ")) + 1);
            if (strcmp(tagType, "image") == 0)
            {
                sprintf(sendedMedia, "IMAGE %s", media);
            }
            else
            {
                sprintf(sendedMedia, "VIDEO %s", media);
            }
            tagContent = sendMessage(sendedMedia);
            free(sendedMedia);
            if (tagContent != NULL)
            {
                // Handle the received tag content
                handleTag(tagType, tagContent, x, y, width, height, site);
                printf("Extracted tag type: %s, Content: %s\n", tagType, tagContent);
                free(tagContent);
            }
        }

        tagContent = (char*)malloc((length + 1) * sizeof(char));
        printf("CONTENT: %s", contentStart);
        strncpy(tagContent, contentStart, length - 1);
        tagContent[length - 1] = '\0';

        handleTag(tagType, tagContent, x, y, width, height, site);
        printf("Extracted tag type: %s, Content: %s\n", tagType, tagContent);

        x = 0;
        free(tagType);
        free(tagContent);
        currentPosition = contentStart + length;
    }
}

GtkWidget* getContentBox()
{
    GtkWidget* currentPage = gtk_notebook_get_nth_page(GTK_NOTEBOOK(newWindow), gtk_notebook_get_current_page(GTK_NOTEBOOK(newWindow)));
    if (GTK_IS_CONTAINER(currentPage))
    {
        GList* children = gtk_container_get_children(GTK_CONTAINER(currentPage));
        for (GList* iter = children; iter != NULL; iter = g_list_next(iter))
        {
            if (GTK_IS_BOX(iter->data) && g_strcmp0(gtk_widget_get_name(GTK_WIDGET(iter->data)), "content-box") == 0)
            {
                g_list_free(children);
                return GTK_WIDGET(iter->data);
            }
        }
        g_list_free(children);
    }
    return NULL;
}

void addWidgetToContentBox(GtkWidget* widget, int x, int y)
{
    GtkWidget* contentBox = getContentBox();
    if (contentBox)
    {
        if (x == 0)
        {
            GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            gtk_box_pack_start(GTK_BOX(box), widget, TRUE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(contentBox), box, FALSE, FALSE, 0);
        }
        else
        {
            GtkWidget* fixed = NULL;
            GList* children = gtk_container_get_children(GTK_CONTAINER(contentBox));
            for (GList* iter = children; iter != NULL; iter = g_list_next(iter))
            {
                if (GTK_IS_FIXED(iter->data))
                {
                    fixed = GTK_WIDGET(iter->data);
                    break;
                }
            }

            if (fixed == NULL)
            {
                fixed = gtk_fixed_new();
                gtk_box_pack_start(GTK_BOX(contentBox), fixed, FALSE, FALSE, 0);
            }
            g_list_free(children);

            gtk_fixed_put(GTK_FIXED(fixed), widget, x, y);
        }
        gtk_widget_show_all(contentBox);
    }
    else
    {
        g_warning("Error: Content box not found in the current page.\n");
    }
}

void handleTag(char* tagType, char* tagContent, int x, int y, int width, int height, char* site)
{
    if (!strcmp(tagType, "title"))
    {
        updateCurrentTabLabel(tagContent);
    }
    else if (!strcmp(tagType, "button"))
    {
        buttonManager(tagContent, x, y);
    }
    else if (!strcmp(tagType, "p"))
    {
        paragraphManager(tagContent, x, y);
    }
    else if (!strcmp(tagType, "h1"))
    {
        tagContent = g_strdup_printf("<span size='xx-large'>%s</span>", tagContent);
        paragraphManager(tagContent, x, y);
    }
    else if (!strcmp(tagType, "h2"))
    {
        tagContent = g_strdup_printf("<span size='x-large'>%s</span>", tagContent);
        paragraphManager(tagContent, x, y);
    }
    else if (!strcmp(tagType, "h3"))
    {
        tagContent = g_strdup_printf("<span size='large'>%s</span>", tagContent);
        paragraphManager(tagContent, x, y);
    }
    else if (!strcmp(tagType, "textbox"))
    {
        // Handle textbox
    }
    else if (!strcmp(tagType, "link"))
    {
        create_link(tagContent, site, x, y);
    }
    else if (!strcmp(tagType, "image"))
    {
       handleImage(x, y, width, height);
    }
    else if (!strcmp(tagType, "video"))
    {
        handleVideo(x, y, width, height, tagContent);
    }
    else
    {
        g_print("%s Doesn't Exist!.\n", tagType);
    }
}

void updateCurrentTabLabel(char* newLabel)
{
    GtkWidget* notebook = newWindow;
    gint currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    GtkWidget* label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), currentPage));
    if (newWindow == NULL)
    {
        g_print("No Title.\n");
        return;
    }

    if (GTK_IS_LABEL(label))
    {
        gtk_label_set_text(GTK_LABEL(label), newLabel);
    }
    else
    {
        g_warning("Error: Tab label is not a valid GTK_LABEL widget.\n");
    }
}

void buttonManager(char* tagContent, int x, int y)
{
    GtkStyleContext* context = NULL;
    GtkWidget* button = gtk_button_new_with_label(tagContent);
    context = gtk_widget_get_style_context(button);
    gtk_style_context_add_class(context, ".enter_button");
    g_signal_connect(button, "clicked", G_CALLBACK(buttonClicked), NULL);
    addWidgetToContentBox(button, x, y);
}

void buttonClicked(GtkWidget* widget, gpointer data)
{
    g_print("CLICKED!");
}

void paragraphManager(char* tagContent, int x, int y)
{
    GtkWidget* label = gtk_label_new(tagContent);
    gtk_label_set_markup(GTK_LABEL(label), tagContent);
    addWidgetToContentBox(label, x, y);
}

void handleImage(int x, int y, int width, int height)
{
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("temp.jpg", NULL);
    GdkPixbuf* scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);
    GtkWidget* image = gtk_image_new_from_pixbuf(scaled_pixbuf);
    addWidgetToContentBox(image, x, y);
    g_object_unref(scaled_pixbuf);
    g_object_unref(pixbuf);
    remove("temp.jpg");
}


void create_link(char* text, char* site, int x , int y)
{
    GtkWidget* label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), g_markup_printf_escaped("<a href=\"%s\">%s</a>", site, text));
    gtk_label_set_track_visited_links(GTK_LABEL(label), FALSE);
    g_signal_connect(label, "activate-link", G_CALLBACK(open_link), (gpointer)site, NULL, G_CONNECT_AFTER);
    addWidgetToContentBox(label, x, y);
}

void open_link(GtkWidget* widget, gpointer data)
{
    char* url = (char*)data;
    char* recvBuf = sendMessage(url);
    clearCurrentPage(newWindow);
    handleVisualMain(newWindow, recvBuf);
}
