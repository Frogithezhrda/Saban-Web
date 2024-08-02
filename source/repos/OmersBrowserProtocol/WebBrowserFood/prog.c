#include "handleVisual.h"
#include <windows.h>

void createTab(GtkWidget* notebook, gchar* tabLabel);
void searchingButton(GtkWidget* widget, gpointer data);
void clearTextButton(GtkWidget* widget, gpointer data);
void forwardButtonClicked(GtkWidget* widget, gpointer data);
void backButtonClicked(GtkWidget* widget, gpointer data);
void reloadButtonClicked(GtkWidget* widget, gpointer data);
void handleCss(GtkWidget* widget, gchar* cssFile);
void tabCreator(GtkWidget* button, GtkWidget* notebook);
void clearCurrentPage(GtkWidget* notebook);
void resetTabLabel(GtkWidget* notebook, gchar* newLabel);

GtkWidget* currentWindow = NULL;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main(int argc, char** argv)
{
    system("start browserProtocol.exe");
    GtkWidget* notebook = NULL;
    GtkWidget* window = NULL;
    GtkWidget* hbox = NULL;
    GtkWidget* vbox = NULL;
    GtkWidget* scrolled_window = NULL;
    GtkWidget* content_area = NULL;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Web Browser");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_set_size_request(window, 800, 600);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox), content_area, TRUE, TRUE, 0);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(content_area), scrolled_window, TRUE, TRUE, 0);

    notebook = gtk_notebook_new();
    gtk_widget_set_name(notebook, "notebook");
    gtk_style_context_add_class(gtk_widget_get_style_context(notebook), "notebook");
    gtk_container_add(GTK_CONTAINER(scrolled_window), notebook);

    currentWindow = notebook;
    createTab(notebook, "Tab 1");
    handleCss(window, "styles.css");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}




void tabCreator(GtkWidget* button, GtkWidget* notebook)
{
    static int tabCount = 2;
    gchar* tabLabel = NULL;
    tabLabel = g_strdup_printf("Tab %d", tabCount);
    createTab(notebook, tabLabel);
    g_free(tabLabel);
    tabCount++;
}

void createTab(GtkWidget* notebook, gchar* tabLabel)
{
    GtkWidget* vboxTab = NULL;
    GtkWidget* label = NULL;
    GtkWidget* grid = NULL;
    GtkWidget* entry = NULL;
    GtkWidget* addButton = NULL;
    GtkWidget* searchButton = NULL;
    GtkWidget* clearButton = NULL;
    GtkWidget* reloadButton = NULL;
    GtkWidget* forwardButton = NULL;
    GtkWidget* backButton = NULL;
    GtkWidget* contentBox = NULL;
    gchar* reloadIcon = "./reload.ico";
    gchar* backIcon = "./back.ico";
    gchar* forwardIcon = "./forward.ico";
    gchar* cleanIcon = "./clean.ico";
    gchar* addIcon = "./add.ico";
    GtkWidget* addImg = gtk_image_new_from_file(addIcon);
    GtkWidget* reloadImg = gtk_image_new_from_file(reloadIcon);
    GtkWidget* backImg = gtk_image_new_from_file(backIcon);
    GtkWidget* forwardImg = gtk_image_new_from_file(forwardIcon);
    GtkWidget* cleanImg = gtk_image_new_from_file(cleanIcon);

    vboxTab = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vboxTab), 10);

    label = gtk_label_new(tabLabel);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vboxTab, label);

    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vboxTab), grid, FALSE, FALSE, 0);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 2, 0, 1, 1);
    gtk_widget_set_hexpand(entry, TRUE);

    addButton = gtk_button_new_from_icon_name("list-add", GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_widget_set_size_request(addButton, 30, 30);
    gtk_style_context_add_class(gtk_widget_get_style_context(addButton), "custom-button");
    g_signal_connect(addButton, "clicked", G_CALLBACK(tabCreator), notebook);
    gtk_grid_attach(GTK_GRID(grid), addButton, 0, 0, 1, 1);

    clearButton = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(clearButton), cleanImg);
    gtk_widget_set_size_request(clearButton, 30, 30);
    gtk_style_context_add_class(gtk_widget_get_style_context(clearButton), "custom-button");
    g_signal_connect(clearButton, "clicked", G_CALLBACK(clearTextButton), entry);
    gtk_grid_attach(GTK_GRID(grid), clearButton, 1, 0, 1, 1);


    searchButton = gtk_button_new_with_label("Search");
    gtk_style_context_add_class(gtk_widget_get_style_context(searchButton), "custom-button");
    g_signal_connect(searchButton, "clicked", G_CALLBACK(searchingButton), entry);
    gtk_grid_attach(GTK_GRID(grid), searchButton, 3, 0, 1, 1);

    reloadButton = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(reloadButton), reloadImg);
    gtk_widget_set_size_request(reloadButton, 30, 30);
    gtk_style_context_add_class(gtk_widget_get_style_context(reloadButton), "custom-button");
    g_signal_connect(reloadButton, "clicked", G_CALLBACK(reloadButtonClicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), reloadButton, 4, 0, 1, 1);
    
    forwardButton = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(forwardButton), forwardImg);

    gtk_widget_set_size_request(forwardButton, 30, 30);
    gtk_style_context_add_class(gtk_widget_get_style_context(forwardButton), "custom-button");
    g_signal_connect(forwardButton, "clicked", G_CALLBACK(forwardButtonClicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), forwardButton, 5, 0, 1, 1);


    backButton = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(backButton), backImg);
    gtk_widget_set_size_request(backButton, 30, 30);
    gtk_style_context_add_class(gtk_widget_get_style_context(backButton), "custom-button");
    g_signal_connect(backButton, "clicked", G_CALLBACK(backButtonClicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), backButton, 6, 0, 1, 1);

    contentBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vboxTab), contentBox, TRUE, TRUE, 0);
    gtk_widget_set_name(contentBox, "content-box");
    g_print("Content box created and named 'content-box'\n");

    gtk_widget_show_all(vboxTab);
}

void clearCurrentPage(GtkWidget* notebook)
{
    gint currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), currentPage);

    if (GTK_IS_CONTAINER(page))
    {
        GList* children = gtk_container_get_children(GTK_CONTAINER(page));
        GtkWidget* contentBox = NULL;

        // Search for the content-box widget
        for (GList* iter = children; iter != NULL; iter = g_list_next(iter))
        {
            GtkWidget* child = GTK_WIDGET(iter->data);
            const gchar* name = gtk_widget_get_name(child);

            if (g_strcmp0(name, "content-box") == 0)
            {
                contentBox = child;
                break;
            }
        }

        if (contentBox)
        {
            // Proceed with cleaning up widgets within contentBox
            GList* contentChildren = gtk_container_get_children(GTK_CONTAINER(contentBox));
            for (GList* iter = contentChildren; iter != NULL; iter = g_list_next(iter))
            {
                GtkWidget* contentChild = GTK_WIDGET(iter->data);
                gtk_widget_destroy(contentChild);
            }
            g_list_free(contentChildren);
        }
        g_list_free(children);
    }
}


void searchingButton(GtkWidget* widget, gpointer data)
{
    char* recvBuf = NULL;
    gchar* text = NULL;
    GtkWidget* entry = GTK_WIDGET(data);
    if (!GTK_IS_ENTRY(entry))
    {
        g_warning("Invalid entry widget.");
        return;
    }
    text = gtk_entry_get_text(GTK_ENTRY(entry));
    g_print("Search button clicked in tab with entry text: %s\n", text);
    if (g_strcmp0(text, "") != 0)
    {
        recvBuf = sendMessage(text);
    }
    else
    {
        recvBuf = (char*)malloc(sizeof(char) * strlen("Not Such Domain"));
        strcpy(recvBuf, "Not Such Domain");
    }
    clearCurrentPage(currentWindow);
    resetTabLabel(currentWindow, "Empty Tab");
    handleVisualMain(currentWindow, recvBuf);
    printf("%s", recvBuf);
}

void resetTabLabel(GtkWidget* notebook, gchar* newLabel)
{
    gint currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    GtkWidget* label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), currentPage));

    if (GTK_IS_LABEL(label))
    {
        gtk_label_set_text(GTK_LABEL(label), newLabel);
    }
    else
    {
        g_warning("Error: Tab label is not a valid GTK_LABEL widget.\n");
    }
}


void clearTextButton(GtkWidget* widget, gpointer data)
{
    GtkWidget* entry = GTK_WIDGET(data);
    if (!GTK_IS_ENTRY(entry))
    {
        g_warning("Invalid entry widget.");
        return;
    }
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void forwardButtonClicked(GtkWidget* widget, gpointer data)
{
    g_print("Forward button clicked\n");
}

void backButtonClicked(GtkWidget* widget, gpointer data)
{
    g_print("Back button clicked\n");
}

void reloadButtonClicked(GtkWidget* widget, gpointer data)
{
    g_print("Reload button clicked\n");
}

void handleCss(GtkWidget* widget, gchar* cssFile)
{
    GtkCssProvider* provider = gtk_css_provider_new();
    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GError* error = NULL;
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(cssFile), &error);

    if (error != NULL)
    {
        g_printerr("Error loading CSS file: %s\n", error->message);
        g_error_free(error);
    }

    g_object_unref(provider);
}

