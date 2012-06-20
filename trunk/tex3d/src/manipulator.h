// manipulator.h
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Jan 2008

#ifndef V_MANIPULATOR_H
#define V_MANIPULATOR_H

#include <gtk-2.0/gtk/gtk.h>

class VManipulator
{
    static VManipulator* s_current;
    
public:
    
    static VManipulator* GetCurrent ();
    static void SetCurrent (VManipulator* manip);

    VManipulator ( GtkWidget* canvas );
    void SetZCenter (float zcenter);
    void Load ();
    void Identity();
    void Rotate (float angle, float rx, float ry, float rz);
    void Scale (float sx, float sy, float sz);

private:
    
    void beginGL();    
    void endGL();
    
    static gboolean motion( GtkWidget* widget, GdkEvent* event, gpointer user_data );
    static gboolean buttonPress( GtkWidget* widget, GdkEvent* event, gpointer user_data );
    static gboolean buttonRelease( GtkWidget* widget, GdkEvent* event, gpointer user_data );
    
    float m_matrix[16];
    float m_zcenter;

    GtkWidget* _canvas;
};

#endif
