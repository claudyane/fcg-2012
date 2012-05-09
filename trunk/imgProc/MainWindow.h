/* 
 * File:   MainWindow.h
 * Author: walther
 *
 * Created on April 10, 2012, 4:43 PM
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include "MainWindowPresenter.h"
#include "image.h"

#define FILTER_GAUSS      "Gauss"
#define FILTER_MEDIAN     "Median"
#define FILTER_EDGES      "Edges"
#define FILTER_BINOTSU    "BinOtsu"
#define FILTER_BINOHBUCHI "BinOhbuchi"

/**
 * This class defines the main window and it holds:
 * A canvas to show the images it is processing.
 * A button to open a new image.
 * A button to apply the current filter.
 * A combo box to select the current filter.
 */
class MainWindow
{
public:
    /**
     * Default constructor
     */
    MainWindow();
    
    /**
     * Default destructor
     */
    virtual ~MainWindow();

private:
    
    /**
     * Builds and assembles the interface
     */
    void build();
    
    /**
     * Builds all the buttons used on the window
     */
    void buildButtons();
    
    /**
     * Builds the menu bar and its submenus
     */
    void buildMenu();
    
    /** 
     * Builds the main box 
     */
    GtkWidget* buildMainBox();
    
    /** 
     * Builds the box with the filter combo box and the apply button 
     */
    GtkWidget* buildFilterBox();
    
    /**
     * Builds the  box that holds the 2 canvas 
     */
    GtkWidget* buildTwoCanvasBox();
    
    /** 
     * Builds the box with the original image 
     */
    GtkWidget* buildFirstCanvasBox();
    
    /**
     *  Builds the box with the edited image 
     */
    GtkWidget* buildSecondCanvasBox();
    
    /**
     * Builds the combo box for filter choosing
     */
    void buildFilterChooser();
    
    /**
     * Builds the drawing area
     */
    void buildCanvas();
    
    //CALLBACKS
    /**
     * Callback called once to initialize the openGL state of the canvas
     * @param canvas is the GtkDrawingArea that will be configured
     * @param event holds specific data related to this event
     * @param user_data is not being used but is required by the API
     * @return TRUE if went ok, FALSE otherwise.
     */
    static gboolean cb_configGLCanvas( GtkWidget *canvas, GdkEventConfigure *event, gpointer user_data );
    
    /**
     * Callback called when any part of the canvas is exposed and it needs to be
     * redrawn.
     * @param canvas is the GtkDrawingAres that will be redrawn
     * @param event holds specific data related to this event
     * @param user_data is not being used but is required by the API
     * @return TRUE if went ok, FALSE otherwise.
     */
    static gboolean cb_exposeGLCanvas( GtkWidget *canvas, GdkEventExpose *event, gpointer user_data );
    
    /**
     * Callback called when the apply button is pressed.
     * @param button is the GtkWidget pointer to the button pressed
     * @param user_data holds the pointer to the class MainWindow
     */
    static void cb_applyButton( GtkMenuItem* item, gpointer user_data );
    
    /**
     * Callback called when the open button is pressed.
     * @param button is the GtkWidget pointer to the button pressed
     * @param user_data holds the pointer to the class MainWindow
     */
    static void cb_openMenuItem( GtkMenuItem* item, gpointer user_data );
    
    /**
     * Callback called when the save button is pressed.
     * @param button is the GtkWidget pointer to the button pressed
     * @param user_data holds the pointer to the class MainWindow
     */
    static void cb_saveMenuItem( GtkMenuItem *item, gpointer user_data );
    
    /**
     * Callback called when the reset button is pressed.
     * @param button is the GtkWidget pointer to the button pressed
     * @param user_data holds the pointer to the class MainWindow
     */
    static void cb_resetMenuItem( GtkMenuItem *item, gpointer user_data );
    
    /**
     * Callback called when the quit button is pressed.
     * @param button is the GtkWidget pointer to the button pressed
     * @param user_data holds the pointer to the class MainWindow
     */
    static void cb_quitMenuItem( GtkMenuItem *item, gpointer user_data );
    
    static void cb_comboBoxSelectItem( GtkComboBoxText* widget, gpointer user_data );
    
    /** GtkWidget pointer, pointing to created window */
    GtkWidget* _window;
    
    /** Button to apply filter */
    GtkWidget* _applyButton;

    /** Combo box listing all the available filters */
    GtkWidget* _filterChooser;
    
    /** Drawing Area will hold the OpenGL drawing of the edited image */
    GtkWidget* _dstCanvas;
    
    /** Drawing Area will hold the OpenGL drawing of the original image */
    GtkWidget* _srcCanvas;
    
    /** Menu bar */
    GtkWidget* _menuBar;
    
    /** Sets the highest frequency to pass */
    GtkWidget* _spinHigh;
    GtkWidget* _spinHighLabel;
    
    /** Sets the lowest frequency to pass */
    GtkWidget* _spinLow;
    GtkWidget* _spinLowLabel;
    
    /** Sets the gamma factor */
    GtkWidget* _spinGamma;
    GtkWidget* _spinGammaLabel;
    
    /** Pointer to the backend part of the window */
    MainWindowPresenter* _presenter;
};

#endif	/* MAINWINDOW_H */

