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
    _presenter = new MainWindowPresenter( _canvas );
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



GtkWidget* MainWindow::getCanvas()
{
    return _canvas;
}



GtkWidget* MainWindow::build()
{
    GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_container_set_border_width( GTK_CONTAINER (window), 5 );
    gtk_window_set_title( GTK_WINDOW (window), "OpenGL Volume Rendering");
    gtk_window_set_resizable( GTK_WINDOW (window), FALSE ); 
    g_signal_connect( window, "delete-event", G_CALLBACK (cb_deleteWindow), window );
    g_signal_connect( window, "key-press-event", G_CALLBACK (cb_keyPress), this );

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
    GtkWidget* loadButton = gtk_button_new();
    gtk_box_pack_start( GTK_BOX(buttonsBox), loadButton, FALSE, FALSE, 2 );
    g_signal_connect( loadButton, "clicked", G_CALLBACK( cb_loadFile ), this );    
    
    std::string filename = "../../data/icons/openFile.png";
    loadWidgetImage( filename.c_str(), GTK_BUTTON(loadButton) );
    
    _fileLabel = gtk_label_new( "" );
    gtk_box_pack_start( GTK_BOX(buttonsBox), _fileLabel, FALSE, FALSE, 2 );
    
    GtkWidget* histogramButton = gtk_button_new();
    gtk_box_pack_start( GTK_BOX(buttonsBox), histogramButton, FALSE, FALSE, 2 );
    g_signal_connect( histogramButton, "clicked", G_CALLBACK( cb_histogram ), this );  
    
    filename = "../../data/icons/histogram.png";
    loadWidgetImage( filename.c_str(), GTK_BUTTON(histogramButton) );
    
    return buttonsBox;
}



GtkWidget* MainWindow::buildCanvasBox()
{
    GtkWidget* canvasBox = gtk_hbox_new( FALSE, 2 );
    
    _canvas = buildCanvas();
    gtk_box_pack_start( GTK_BOX(canvasBox), _canvas, FALSE, FALSE, 2 );
    
    return canvasBox;
}



GtkWidget* MainWindow::buildCanvas()
{
    // Create the canvas and set  it's size
    GtkWidget* canvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(canvas), 500, 500 );

    // OpenGL configuration for the canvas
    GdkGLConfig* glconfig = gdk_gl_config_new_by_mode( static_cast<GdkGLConfigMode>( GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE ) );

    // Define the canvas as capable of rendering OpenGL graphics
    gtk_widget_set_gl_capability( canvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );

    // Connect the signals to the callbacks
    g_signal_connect( canvas, "configure-event", G_CALLBACK( cb_configGLCanvas )    , NULL );
    g_signal_connect( canvas, "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );

    return canvas;
}



gboolean MainWindow::cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data )
{
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( canvas );    
    MainWindow* window = ( MainWindow* )user_data;
    
    window->beginGL();    
    window->_presenter->render();
    
    gdk_gl_drawable_swap_buffers( glDrawable );
    
    window->endGL();
    
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

        // faz o gtk aceitar comandos openGL
        window->beginGL();
        
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
        gtk_widget_queue_draw( window->_canvas );
        
        g_free( filepath );
        
        window->endGL();
    }

    gtk_widget_destroy( fileChooser );
}



void MainWindow::cb_histogram( GtkWidget* button, gpointer user_data )
{
    MainWindow* window = (MainWindow*) user_data;
    window->_presenter->showHistogram();
}



gboolean MainWindow::cb_deleteWindow( GtkWidget* widget, GdkEvent* event, gpointer data )
{
    gtk_main_quit();

    return TRUE;
}



gint MainWindow::cb_keyPress( GtkWidget* widget, GdkEvent* event, gpointer callback_data )
{
    MainWindow* window = ( MainWindow* )callback_data;
    window->_presenter->keyPress( event->key.keyval );
    
    gtk_widget_queue_draw( window->_canvas );
    
    return 0;
}



void MainWindow::beginGL()
{
    GdkGLContext* glContext = gtk_widget_get_gl_context( _canvas );
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( _canvas );
 
    //tell gtk that we will start calling opengl functions
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
}
    


void MainWindow::endGL()
{
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( _canvas );        
    
    //Tell GTK we stopped messing with OpenGL
    gdk_gl_drawable_gl_end( glDrawable );
}



bool MainWindow::loadWidgetImage( const char* filename, GtkButton* widget )
{
    GError* err = NULL;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file( filename, &err );
    
    if(pixbuf == NULL)
    {
       fprintf(stderr, "Erro GTK: não conseguiu carregar imagem %s.\n %s\n", filename, err->message);
       g_error_free( err );

       return false;
    }

    GtkWidget* image = gtk_image_new_from_pixbuf( pixbuf );

    GtkWidget* box = gtk_hbox_new( FALSE, 0 );
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);
    gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);

    gtk_widget_show (image);    
    gtk_widget_show (box);

    gtk_container_add (GTK_CONTAINER (widget), box);
     
    return true;
}
