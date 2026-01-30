// Defines vectors and points

#ifndef AFFINE_CPP_
#define AFFINE_CPP_

#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include "utility.cpp"

using namespace std;

// In this file, the classes must be declared and defined separately because of circular dependancy

class Affine
{
	protected: // Points and Vectors are mostly the same...
		double x;
		double y;
		double z;
	public:	Affine(double x_, double y_, double z_)
		{
			x = x_;
			y = y_;
			z = z_;
		}

		// Default constructor
		Affine(void):x(0), y(0), z(0) {}

		// Print
		void print()
		{
			cout << "<" << x << ", " << y << ", " << z << ">\n";
		}

		double getX() {return x;}
		double getY() {return y;}
		double getZ() {return z;}
		virtual void render(){};
};


class Vector;

// Point
class Point : public Affine
{
	friend class Vector;

	public: Point(double, double, double);
		Point(void);
		Vector operator-(Point);
		Point operator*(double);
		Point operator+(Point);
		void render();
		//This needs to be removed and we need to define point-matrix multiplication
		//using quaternions!
		Point rotate(double angle)
		{
			double c = cos(angle);
			double s = sin(angle);
			return Point(y, (c-s)*x, (c+s)*y);
		}

};

// Vector
class Vector : public Affine
{
	friend class Point;

	public: Vector(double, double, double);
		Vector(void);
		double length();
		Vector unit();
		Vector operator+(Vector);
		Point  operator+(Point);
		Vector operator-(Vector);
		Point  operator-(Point);
		Vector operator*(Vector);
		Vector operator*(double);
		double operator^(Vector);
		Vector operator-();
		void render();
};

		// Constructor
		Point::Point(double x_, double y_, double z_): Affine(x_,y_,z_){}

		// Default constructor
		Point::Point(void)
		{
			Affine();
		}

		// Point-point subtraction
		Vector Point::operator-(Point p)
		{
			return Vector(x-p.x, y-p.y, z-p.z); 
		}

		// Point-point addition (for weighted sums)
		Point Point::operator+(Point p)
		{
			return Point(x+p.x, y+p.y, z+p.z); 
		}

		// Point weighting
		Point Point::operator*(double c)
		{
			//c = clamp(c);
			return Point(c*x, c*y, c*z); 
		}

		// Tell openGL to render this vertex
		// Call only between glBegin(*) and glEnd()!
		void Point::render()
		{
			glVertex3f(x, y, z);
		}


		// Constructor
		Vector::Vector(double x_, double y_, double z_): Affine(x_,y_,z_){}

		// Default constructor
		Vector::Vector(void)
		{
			Affine();
		}

		// Length
		double Vector::length()
		{
			 return sqrt( (x*x)+(y*y)+(z*z) );
		}


		// Return the corresponding unit vector
		Vector Vector::unit()
		{
			double my_length = sqrt( (x*x)+(y*y)+(z*z) );
		 
			// Don't divide by zero
			if (my_length == 0) my_length = 1;

			return Vector(x/my_length, y/my_length, z/my_length);
		}

		// Vector vector addition
		Vector Vector::operator+(Vector v)
		{
			return Vector(x+v.x, y+v.y, z+v.z); 
		}

		// Vector point addition
		Point Vector::operator+(Point p)
		{
			return Point(x+p.x, y+p.y, z+p.z); 
		}

		// Vector vector subtraction
		Vector Vector::operator-(Vector v)
		{
			return Vector(x-v.x, y-v.y, z-v.z); 
		}

		// Vector point subtraction
		Point Vector::operator-(Point p)
		{
			return Point(x-p.x, y-p.y, z-p.z); 
		}

		// Vector scalar multiplication
		Vector Vector::operator*(double c)
		{
			return Vector(c*x, c*y, c*z); 
		}

		// Cross product
		Vector Vector::operator*(Vector v)
		{
			double x_, y_, z_;
			x_ = (y * v.z) - (z * v.y);
			y_ = (z * v.x) - (x * v.z);
			z_ = (x * v.y) - (y * v.x);

			return Vector(x_, y_, z_); 
		}

		// Dot product, can't use a period, so I'm using ^
		double Vector::operator^(Vector v)
		{
			return x*v.x + y*v.y + z*v.z;
		}

		// Unary minus
		Vector Vector::operator-()
		{
			return Vector(-x, -y, -z);
		}

		// Tell openGL to use this vertex as the surface normal
		// Call only between glBegin(*) and glEnd()!
		void Vector::render()
		{
			glNormal3f(x, y, z);
		}



#endif
