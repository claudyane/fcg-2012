// manipulator.cpp
// Visualização 3D
// W. Celes - celes@inf.puc-rio.br
// PUC-Rio, Set 2010
#include <GL/gl.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "manipulator.h"
#include "vector.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.1459
#define TORAD(x) (x*180.0f/PI)

VManipulator* VManipulator::s_current = 0;

// internal control
static char g_state = ' ';
static int g_x0, g_y0;

static VVector map (int x, int y)
{
  VVector v;
  int vp[4];
  glGetIntegerv(GL_VIEWPORT,vp);
  int radius = vp[2] < vp[3] ? vp[2]/2 : vp[3]/2;
  v.x = (float)((x-vp[0]) - vp[2]/2) / radius;
  v.y = (float)((vp[3]-y-vp[0]) - vp[3]/2) / radius;
  float len = sqrt(v.x*v.x + v.y*v.y);
  if (len > 1.0) {
    v.x /= len;
    v.y /= len;
    v.z = 0.0f;
  }
  else {
    v.z = sqrt(1 - v.x*v.x - v.y*v.y);
  }
  return v;
}



gboolean VManipulator::buttonRelease( GtkWidget* widget, GdkEvent* event, gpointer user_data )
{
    if (!VManipulator::GetCurrent())
        return TRUE;
    
    g_state = ' ';

    return TRUE;
}



gboolean VManipulator::buttonPress( GtkWidget* widget, GdkEvent* event, gpointer user_data )  
{
    if (!VManipulator::GetCurrent())
        return TRUE;

    GdkEventButton* buttonEvent = (GdkEventButton*) event;

    g_x0 = buttonEvent->x; g_y0 = buttonEvent->y;
    if (buttonEvent->button == 1)
        g_state = 'r';
    else if (buttonEvent->button == 3)
        g_state = 's';
    else
        g_state = ' ';

    return TRUE;
}



gboolean VManipulator::motion( GtkWidget* widget, GdkEvent* event, gpointer user_data )
{
  if (!VManipulator::GetCurrent())
    return TRUE;
  
  VManipulator* manipulator = (VManipulator*)user_data;
  GdkEventMotion* motionEvent = (GdkEventMotion*) event;
  
  manipulator->beginGL();
  
  int x = (int)motionEvent->x;
  int y = (int)motionEvent->y;
  
  int dx = abs( x - g_x0 );
  int dy = abs( y - g_y0 );
  
  if ( dx > 1 || dy > 1) {
    if (g_state == 'r') {
      VVector v0 = map(g_x0, g_y0);
      VVector v1 = map(x, y);
      VVector r = Cross(v0, v1);
      VManipulator::GetCurrent()->Rotate(TORAD(2*asin(r.Length())),r.x,r.y,r.z);

    }
    else if (g_state == 's') {
      int vp[4];
      glGetIntegerv(GL_VIEWPORT,vp);
      float f = dx > dy ? (float)(x-g_x0) / vp[2] : (float) (-y+g_y0) / vp[3];
      VManipulator::GetCurrent()->Scale(1+f, 1+f, 1+f);
    }
    g_x0 = x; g_y0 = y;

    gtk_widget_queue_draw( widget );
    
    manipulator->endGL();
    
    return TRUE;
  }
}



void VManipulator::SetCurrent (VManipulator* manip)
{
  s_current = manip;
}



VManipulator* VManipulator::GetCurrent ()
{
  return s_current;
}



VManipulator::VManipulator ( GtkWidget* canvas )
{
    _canvas = canvas;
    gtk_widget_add_events( _canvas, (GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK));
    
    gtk_signal_connect( GTK_OBJECT (_canvas), "button-press-event", GTK_SIGNAL_FUNC(VManipulator::buttonPress), this );
    gtk_signal_connect( GTK_OBJECT (_canvas), "button-release-event", GTK_SIGNAL_FUNC(VManipulator::buttonRelease), this );
    gtk_signal_connect( GTK_OBJECT (_canvas), "motion-notify-event", GTK_SIGNAL_FUNC(VManipulator::motion), this );
    
    beginGL();
    
    Identity();  
    SetCurrent(this);
    
    endGL();
}



void VManipulator::SetZCenter (float zcenter)
{
  m_zcenter = zcenter;
}



void VManipulator::Identity ()
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
  glPopMatrix();
  glPopAttrib();
}



void VManipulator::Rotate (float angle, float rx, float ry, float rz)
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotatef(angle, rx, ry, rz);
  glMultMatrixf(m_matrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
  glPopMatrix();
  glPopAttrib();
}



void VManipulator::Scale (float sx, float sy, float sz)
{
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glScalef(sx, sy, sz);
  glMultMatrixf(m_matrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
  glPopMatrix();
  glPopAttrib();
}



void VManipulator::Load ()
{
  glTranslatef(0.0f, 0.0f, -m_zcenter);
  glMultMatrixf(m_matrix);
  glTranslatef(0.0f, 0.0f, m_zcenter);
}



void VManipulator::beginGL()
{
    GdkGLContext* glContext = gtk_widget_get_gl_context( _canvas );
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( _canvas );
 
    //tell gtk that we will start calling opengl functions
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
}
    


void VManipulator::endGL()
{
    //GdkGLContext* glContext = gtk_widget_get_gl_context( _canvas );
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( _canvas );
    
    //gdk_gl_drawable_swap_buffers (glDrawable);
    
    //Tell GTK we stopped messing with OpenGL
    gdk_gl_drawable_gl_end( glDrawable );
}

