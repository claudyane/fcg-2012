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
     * Initializes the (0, 0, 0, 1) color
     */
    Color();
    
    /**
     * Constructor
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     * @param a Alpha component, defaults to 1.0f
     */
    Color( float r, float g, float b, float a = 1.0f );

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
     * @param a Alpha component defaults to 1.0f
     */
    void set( float r, float g, float b, float a = 1.0f );
    
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
    float a;
};

/**
 * Operators
 */
Color operator+( const Color& a, const Color& b );
Color operator-( const Color& a, const Color& b );
Color operator*( const double a, const Color& b );
Color operator*( const Color& a, const double b );
Color operator*( const Color& a, const Color& b );


#endif	/* COLOR_H */

