// Color object

#ifndef COLOR_CPP_
#define COLOR_CPP_

#include "GL/glut.h"
#include "utility.cpp"
#include <iostream>

class Color
{
	private:double r;
		double g;
		double b;
 
	public:	// Constructor
		Color(double r_, double g_, double b_)
		{
			r = clamp(r_);
			g = clamp(g_);
			b = clamp(b_);
		}

		Color(){} // Why do I need this?

		// Copy constructor
		Color(const Color& c)
		{
			r = c.r;
			g = c.g;
			b = c.b;
		}
		

		// Additive coloration
		Color operator+(const Color& c)
		{
			return Color(r+c.r, g+c.g, b+c.b);
		}

		// Dimming
		Color operator*(double f)
		{
			f = clamp(f);
			return Color(r*f, g*f, b*f);
		}

		// Print
		void print()
		{
			cout << "<" << r << "," << g << "," << b << ">\n";
		}

		void load()
		{
			glColor3f((GLfloat)r, (GLfloat)g, (GLfloat)b);
		}

		// Accessor functions
		double getR() {return r;}
		double getG() {return g;}
		double getB() {return b;}

};

#endif
