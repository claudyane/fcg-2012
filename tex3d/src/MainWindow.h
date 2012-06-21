/*
 * MainWindow.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtk/gtk.h>

#include "MainWindowPresenter.h"

class MainWindow
{
public:
    MainWindow();
    virtual ~MainWindow();
    void show();
    
    GtkWidget* getCanvas();

private:
    GtkWidget* build();
    GtkWidget* buildButtonsBox();
    GtkWidget* buildCanvasBox();
    GtkWidget* buildCanvas();

    void beginGL();
    void endGL();
    
    static gboolean cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data );
    static gboolean cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data );
    static void cb_loadFile( GtkWidget* button, gpointer user_data );
    static void cb_histogram( GtkWidget* button, gpointer user_data );
    static gboolean cb_deleteWindow( GtkWidget* widget, GdkEvent* event, gpointer data );
    static gint cb_keyPress( GtkWidget* widget, GdkEvent* event, gpointer callback_data );
    
    GtkWidget* _window;
    GtkWidget* _canvas;
    GtkWidget* _messageBar;
    GtkWidget* _fileLabel;
    
    MainWindowPresenter* _presenter;
};

#endif /* MAINWINDOW_H_ */
