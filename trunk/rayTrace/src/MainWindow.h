/*
 * MainWindow.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtk/gtk.h>

class MainWindow
{
public:
    MainWindow();
    virtual ~MainWindow();

private:
    GtkWidget* build();
    void buildRayTraceCanvas();

    GtkWidget* _window;
    GtkWidget* _rayTraceCanvas;
};

#endif /* MAINWINDOW_H_ */
