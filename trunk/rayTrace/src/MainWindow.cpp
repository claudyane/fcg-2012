/*
 * MainWindow.cpp
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#include "MainWindow.h"

#include <gtk/gtk.h>
#include <gtk/gtkdrawingarea.h>

MainWindow::MainWindow()
{
    _window = build();
    gtk_widget_show_all( _window );
}

MainWindow::~MainWindow()
{
    gtk_widget_destroy( _window );
}

GtkWidget* MainWindow::build()
{
    GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );



    return window;
}

void MainWindow::buildRayTraceCanvas()
{
    _rayTraceCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(_rayTraceCanvas), 800, 600 );
}
