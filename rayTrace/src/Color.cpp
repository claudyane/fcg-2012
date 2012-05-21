#include "Color.h"


Color::Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}



Color::Color( float r, float g, float b )
{
    this->r = r;
    this->g = g;
    this->b = b;
}



Color::Color( const Color& copy )
{
    r = copy.r;
    g = copy.g;
    b = copy.b;
}



Color::~Color()
{
    
}



void Color::set( float r, float g, float b )
{
    this->r = r;
    this->g = g;
    this->b = b;
}



Color Color::operator=( const Color& a )
{
    r = a.r;
    g = a.g;
    b = a.b;
    
    return *this;
}



Color Color::operator+=( const Color& a )
{
    r += a.r;
    g += a.g;
    b += a.b;
    
    return *this;
}



Color Color::operator-=( const Color& a )
{
    r -= a.r;
    g -= a.g;
    b -= a.b;
    
    return *this;
}



Color Color::operator*=( const double a )
{
    r *= a;
    g *= a;
    b *= a;
    
    return *this;
}



Color Color::operator/=( const double a )
{
    r /= a;
    g /= a;
    b /= a;
    
    return *this;    
}



Color operator+( const Color& a, const Color& b )
{
    return Color( a.r + b.r, a.g + b.g, a.b + b.b );
}



Color operator-( const Color& a, const Color& b )
{
    return Color( a.r - b.r, a.g - b.g, a.b - b.b );
}



Color operator*( const double a, const Color& b )
{
    return Color( a * b.r , a * b.g, a * b.b );
}



Color operator*( const Color& a, const double b )
{
    return Color( a.r * b, a.g * b, a.b * b );
}
