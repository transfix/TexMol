#ifndef __PBKernel_H__
#define __PBKernel_H__

//#include "SphericalHarmonic.h"

#include <stdio.h>
#include <complex>
#include <math.h>

#include <Utility/utility.h>

#define KAPPA_THRESHOLD 0.01
//#define PI 3.14159265
#define DISTANCE_THRESHOLD 1e-1


class vtx{
public:
        int id;
        double x;
        double y;
        double z;

        vtx() {id=0;x=0.0;y=0.0;z=0.0;id=0;}
        vtx(double vx, double vy, double vz) {x=vx;y=vy;z=vz;id=0;}
	vtx(double vx, double vy, double vz, int _id) {x=vx;y=vy;z=vz;id=_id;}

        ~vtx() {}

        void print() { printf("%d: %f %f %f\n", id, x, y, z); }

	vtx& operator=(const vtx& v)
	{
		if (this!=&v) {
                	x=v.x; y=v.y; z=v.z; id=v.id;
	        }
        	return *this;
	}

	vtx operator+(const vtx v) const
        {
                return vtx(x+v.x,y+v.y,z+v.z,id);
        }

	vtx operator-(const vtx v) const
	{
        	return vtx(x-v.x,y-v.y,z-v.z,id);
	}
	
	vtx operator*(const double r) const
        {
                return vtx(r*x,r*y,r*z,id);
        }

	bool operator==(const vtx v)
	{
        	return (x==v.x) &&
	               (y==v.y) &&
	               (z==v.z);
	}

	vtx timesspan(const double span0, const double span1, const double span2)
	{
		x *= span0;
		y *= span1;
		z *= span2;
		return *this;
	}

	double dot(const vtx vec) const
	{
        	return x*vec.x + y*vec.y + z*vec.z;
	}

	void normalize()
	{
		double dis = distance();
		if( dis < 0.00000001)
			return;
		if( dis < 0.01)
		{
			double tmpx = x * 100.0;
			double tmpy = y * 100.0;
			double tmpz = z * 100.0;
			dis = tmpx * tmpx + tmpy * tmpy + tmpz * tmpz;
			dis = sqrt(dis);
			x = tmpx / dis;
			y = tmpy / dis;
			z = tmpz / dis;
		}
		else
		{
			x = x/dis;
			y = y/dis;
			z = z/dis;
		}
 
	}

	

        double magsquare(){ return x*x + y*y + z*z; }
	double distance(){ return sqrt(magsquare()); }

	vtx cross(const vtx& v) const
	{
		return vtx(
                y*v.z - z*v.y,
                z*v.x - x*v.z,
                x*v.y - y*v.x
                );
	}
};


////dBEM
/*double d1(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double 
epsilon);
double d2(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double d3(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double d4(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double d1_singular(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double d3_singular(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double d4_singular(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, 
double epsilon);*/
////nBEM
double G0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double Gk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double dG0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double dGk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double dG0x(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
double dGkx(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);

double ddG0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double 
epsilon);
double ddGk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);

double ddGkmG0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon);
/*double A(int coeffidx, Geometry* geo, double* span, int i, int j, double kappa, double epsilon);

double Afunc(int coeffidx,vtx* pi,vtx* pj, vtx evlpoint, vtx elempoint, double kappa, double epsilon);

*/
#endif
