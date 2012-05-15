/*
 * MainWindow.cpp
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#include "MainWindow.h"
#include "image.h"

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>

MainWindow::MainWindow()
{
    _window = build();
    _presenter = new MainWindowPresenter();
}

MainWindow::~MainWindow()
{
    gtk_widget_destroy( _window );
    delete _presenter;
}

void MainWindow::show()
{
    gtk_widget_show_all( _window );
}

GtkWidget* MainWindow::build()
{
    GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

    GtkWidget* mainBox = gtk_vbox_new( FALSE, 10 );
    gtk_container_add( GTK_CONTAINER(window), mainBox );

    _rayTraceCanvas = buildRayTraceCanvas();
    gtk_box_pack_start_defaults( GTK_BOX(mainBox), _rayTraceCanvas );

    return window;
}

GtkWidget* MainWindow::buildRayTraceCanvas()
{
    // Create the canvas and set  it's size
    GtkWidget* rayTraceCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(rayTraceCanvas), 800, 600 );

    // OpenGL configuration for the canvas
    GdkGLConfig* glconfig = gdk_gl_config_new_by_mode( static_cast<GdkGLConfigMode>( GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE ) );

    // Define the canvas as capable of rendering OpenGL graphics
    gtk_widget_set_gl_capability( rayTraceCanvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );

    // Connect the signals to the callbacks
    g_signal_connect( rayTraceCanvas, "configure-event", G_CALLBACK( cb_configGLCanvas )    , NULL );
    g_signal_connect( rayTraceCanvas, "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );

    return rayTraceCanvas;
}

gboolean MainWindow::cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data )
{
    GdkGLContext *glContext = gtk_widget_get_gl_context( canvas );
    GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable( canvas );
 
    //tell gtk that we will start calling opengl functions
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
    
    //Clear the canvas with black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //Recover access to the image
    MainWindow* window = ( MainWindow* )user_data;
    Image* image = window->_presenter->getImage();
    
    //If Image loaded OK, start drawing
    if( image )
    {
        glBegin( GL_POINTS );
            int width = imgGetWidth( image );
            int height = imgGetHeight( image );

            for (int w = 0; w < width; ++w)
            {
                for (int h = 0; h < height; ++h)
                {
                    float r,g,b;
                    imgGetPixel3f( image, w, h, &r, &g, &b );
                    glColor3f( r,g,b );
                    glVertex2i( w, h );
                }
            }
        glEnd();
    }
    
    //Update image
    gdk_gl_drawable_swap_buffers (glDrawable);
    
    //Tell GTK we stopped messing with OpenGL
    gdk_gl_drawable_gl_end( glDrawable );
    
    //everything went fine
    return TRUE;
}

gboolean MainWindow::cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data )
{
    GdkGLContext *glContext = gtk_widget_get_gl_context( canvas );
    GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable( canvas );

    // start opengl commands
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }

    glLoadIdentity();
    glViewport( 0, 0, canvas->allocation.width, canvas->allocation.height );
    glMatrixMode( GL_PROJECTION );
    glOrtho ( 0, canvas->allocation.width, 0, canvas->allocation.height, -10, 10 );

    // end opengl commands
    gdk_gl_drawable_gl_end( glDrawable );

    return true;
}
