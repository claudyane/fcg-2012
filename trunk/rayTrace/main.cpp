#include <gtk/gtk.h>
#include <gtk/gtkglinit.h>

#include "MainWindow.h"

int main( int argc, char** argv )
{
    gtk_init( &argc, &argv );
    gtk_gl_init( &argc, &argv );
    
    return 0;
}
