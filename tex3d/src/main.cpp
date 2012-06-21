#include <gtk/gtk.h>
#include <gtk/gtkglinit.h>
#include <GL/freeglut_std.h>

#include "MainWindow.h"

int main( int argc, char** argv )
{
    gtk_init( &argc, &argv );
    gtk_gl_init( &argc, &argv );
    glutInit( &argc, argv );
    
    MainWindow* window = new MainWindow();
    window->show();
    
    return 0;
}
