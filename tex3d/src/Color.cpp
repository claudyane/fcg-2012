#include "Color.h"


Color::Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}



Color::Color( float r, float g, float b, float a )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}



Color::Color( const Color& copy )
{
    r = copy.r;
    g = copy.g;
    b = copy.b;
    a = copy.a;
}



Color::~Color(){}



void Color::set( float r, float g, float b, float a )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}



Color Color::operator=( const Color& c )
{
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    
    return *this;
}



Color Color::operator+=( const Color& c )
{
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;
    
    return *this;
}



Color Color::operator-=( const Color& c )
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    a -= c.a;
    
    return *this;
}



Color Color::operator*=( const double c )
{
    r *= c;
    g *= c;
    b *= c;
    a *= c;
    
    return *this;
}



Color Color::operator/=( const double c )
{
    r /= c;
    g /= c;
    b /= c;
    a /= c;
    
    return *this;    
}



Color operator+( const Color& c, const Color& b )
{
    return Color( c.r + b.r, c.g + b.g, c.b + b.b, c.a + b.a );
}



Color operator-( const Color& c, const Color& b )
{
    return Color( c.r - b.r, c.g - b.g, c.b - b.b, c.a - b.a );
}



Color operator*( const double c, const Color& b )
{
    return Color( c * b.r , c * b.g, c * b.b, c * b.a );
}



Color operator*( const Color& c, const double b )
{
    return Color( c.r * b, c.g * b, c.b * b, c.a * b );
}



Color operator*( const Color& c, const Color& b )
{
    return Color( c.r * b.r, c.g * b.g, c.b * b.b, c.a * b.a );
}



Color weightOpacity( const Color& a )
{
    return Color( a.r * a.a, a.g * a.a, a.b * a.a, a.a );
}
