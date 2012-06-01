/*
 * MainWindow.cpp
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#include "MainWindow.h"
#include "Image.h"

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <iostream>
#include <sstream>
#include <gtk-2.0/gtk/gtklabel.h>
#include <gtk-2.0/gtk/gtkbox.h>
#include <string.h>
#include <glib-2.0/glib/gtypes.h>
#include <gtk-2.0/gtk/gtkwidget.h>


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
    gtk_main();
}



GtkWidget* MainWindow::build()
{
    GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_container_set_border_width( GTK_CONTAINER (window), 5 );
    gtk_window_set_title( GTK_WINDOW (window), "Raytracing");
    gtk_window_set_resizable( GTK_WINDOW (window), FALSE ); 
    g_signal_connect( window, "delete-event", G_CALLBACK (cb_deleteWindow), window );

    GtkWidget* mainBox = gtk_vbox_new( FALSE, 2 );
    gtk_container_add( GTK_CONTAINER(window), mainBox );

    GtkWidget* buttonsBox = buildButtonsBox();
    gtk_box_pack_start_defaults( GTK_BOX(mainBox), buttonsBox );
    
    gtk_box_pack_start( GTK_BOX (mainBox), gtk_hseparator_new(), TRUE , TRUE , 5  );
    
    GtkWidget* canvasBox = buildCanvasBox();
    gtk_box_pack_start_defaults( GTK_BOX(mainBox), canvasBox );
    
    gtk_box_pack_start( GTK_BOX (mainBox), gtk_hseparator_new(), TRUE , TRUE , 5  );
    
    _messageBar = gtk_label_new( "Created by Eliana Goldner and Walther Maciel" );
    gtk_box_pack_start_defaults( GTK_BOX(mainBox), _messageBar );

    gtk_widget_show_all( window );

    return window;
}



GtkWidget* MainWindow::buildButtonsBox()
{
    GtkWidget* buttonsBox = gtk_hbox_new(  FALSE, 5 );
    
    // creating button to load a scene
    GtkWidget* loadButton = gtk_button_new_with_label( "Load Scene" );
    gtk_widget_set_size_request( loadButton, 100, 30 );
    gtk_box_pack_start( GTK_BOX(buttonsBox), loadButton, FALSE, FALSE, 2 );
    g_signal_connect( loadButton, "clicked", G_CALLBACK( cb_loadFile ), this );    
    
    _fileLabel = gtk_label_new( "" );
    gtk_box_pack_start( GTK_BOX(buttonsBox), _fileLabel, FALSE, FALSE, 2 );
    
    return buttonsBox;
}



GtkWidget* MainWindow::buildCanvasBox()
{
    GtkWidget* canvasBox = gtk_hbox_new( FALSE, 2 );
    
    _rayTraceCanvas = buildRayTraceCanvas();
    gtk_box_pack_start( GTK_BOX(canvasBox), _rayTraceCanvas, FALSE, FALSE, 2 );
    
    return canvasBox;
}



GtkWidget* MainWindow::buildRayTraceCanvas()
{
    // Create the canvas and set  it's size
    GtkWidget* rayTraceCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(rayTraceCanvas), 300, 300 );

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



void MainWindow::cb_loadFile( GtkWidget* button, gpointer user_data )
{
    GtkWidget* fileChooser;
    gchar* filepath = 0;
    MainWindow* window = (MainWindow*) user_data;

    fileChooser = gtk_file_chooser_dialog_new("Abrir Arquivo",
                                               GTK_WINDOW (window->_window),
                                               GTK_FILE_CHOOSER_ACTION_OPEN,
                                               GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                               GTK_STOCK_OPEN, GTK_RESPONSE_OK,
                                               NULL);

    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_set_name( GTK_FILE_FILTER (filter), "(*.scn, *.SCN)");
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filter), "*.scn" );
    gtk_file_filter_add_pattern( GTK_FILE_FILTER (filter), "*.SCN" );
    gtk_file_chooser_add_filter( GTK_FILE_CHOOSER (fileChooser), GTK_FILE_FILTER (filter) );

    if (gtk_dialog_run( GTK_DIALOG (fileChooser) ) == GTK_RESPONSE_OK)
    {
        filepath = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER (fileChooser) );

        if (!window->_presenter->loadFile( filepath ))
        {
            std::string message = "Error loading file";
            gtk_label_set_text( GTK_LABEL (window->_fileLabel), message.c_str()  ); 
        }
        else
        {
            std::string strFilePath = filepath;
            size_t pos = strFilePath.find_last_of( "/\\" );
            
            if (pos != std::string::npos)
            {
                std::string filename = strFilePath.substr( pos + 1 );
                gtk_label_set_text( GTK_LABEL (window->_fileLabel), filename.c_str() );
            }
            else
            {
                gtk_label_set_text( GTK_LABEL (window->_fileLabel), filepath );
            }
            
        }

        gtk_label_set_label( GTK_LABEL(window->_messageBar), "Created by Eliana Goldner and Walther Maciel" );
        gtk_widget_queue_draw( window->_rayTraceCanvas );
        
        g_free( filepath );
    }

    gtk_widget_destroy( fileChooser );
}



void MainWindow::cb_render( GtkWidget* button, gpointer user_data )
{
//    MainWindow* window = (MainWindow*)user_data;
//       
//    //gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON );
//    time_t begin = time(NULL);
//    window->_presenter->render();
//    time_t end = time(NULL);
//       
//    if (!window->_presenter->getImage())
//        return;
//    
//    int width  = imgGetWidth( window->_presenter->getImage() );
//    int heigth = imgGetHeight( window->_presenter->getImage()  );
//
//    gtk_drawing_area_size( GTK_DRAWING_AREA (window->_rayTraceCanvas) , width, heigth );
//    gtk_window_resize( GTK_WINDOW (window->_window), width, heigth );            
//    
//    long long elapsed = end - begin;
//    std::stringstream message;
//    message << "Time elapsed: " << elapsed << " seconds.";
//    
//    gtk_label_set_label( GTK_LABEL(window->_messageBar), message.str().c_str() );
}



gboolean MainWindow::cb_deleteWindow( GtkWidget* widget, GdkEvent* event, gpointer data )
{
    gtk_main_quit();

    return TRUE;
}
