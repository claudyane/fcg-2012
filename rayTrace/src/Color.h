/* 
 * File:   Color.h
 * Author: eliana
 *
 * Created on May 20, 2012, 7:28 PM
 */

#ifndef COLOR_H
#define	COLOR_H

class Color 
{
public:
    
    /**
     * Default constructor
     * Initializes the (0, 0, 0) color
     */
    Color();
    
    /**
     * Constructor
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     */
    Color( float r, float g, float b );

    /**
     * Copy constructor
     */
    Color( const Color& copy );
    
    /**
     * Destructor
     */
    virtual ~Color();
    
    /**
     * Set color components value
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     */
    void set( float r, float g, float b );
    
    /**
     * Operators
     */
    Color operator=( const Color& a );
    Color operator+=( const Color& a );
    Color operator-=( const Color& a );
    Color operator*=( const double a );
    Color operator/=( const double a );
    
    /** Color components */
    float r;
    float g;
    float b;
};

/**
 * Operators
 */
Color operator+( const Color& a, const Color& b );
Color operator-( const Color& a, const Color& b );
Color operator*( const double a, const Color& b );
Color operator*( const Color& a, const double b );


#endif	/* COLOR_H */

