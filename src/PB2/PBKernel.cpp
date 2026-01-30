#include "PB2/PBKernel.h"
#include <cmath>
#include <iostream>

using namespace std;

#define POLAR_COEFF 0.62322524014023051339402008025032


double cosine(vtx& v1, vtx& v2)
{
	//v1.normalize();
	//v2.normalize();
	double r1 = v1.distance();
	double r2 = v2.distance();
	//printf("%f %f\n",r1,r2);
	return v1.dot(v2)/(r1*r2);
}


double G0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        double d2;
        vtx v = (evlpoint - elempoint);
        double r = v.distance();

	if(r < DISTANCE_THRESHOLD)
	{
		return 0.0; // using singular integral
		//r = DISTANCE_THRESHOLD;
	}
        return OOFP/(r);
}

double Gk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        double d2;
        vtx v = (evlpoint - elempoint);
        double r = v.distance();
	if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral
		//r = DISTANCE_THRESHOLD;
        return OOFP*exp(-kappa*r)/(r);
}

double dG0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        //vtx v = (evlpoint - elempoint);
        vtx v = (evlpoint - elempoint);
	double tmp = v.dot(elemnormal);
        double r = v.distance();
	if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral
		//r = DISTANCE_THRESHOLD;
        //return -OOFP*tmp/(r*r*r);
        return OOFP*tmp/(r*r*r);
}

double dG0x(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        
        vtx v = (evlpoint - elempoint);
	double tmp = v.dot(evlnormal);
        double r = v.distance();
	if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral
		//r = DISTANCE_THRESHOLD;
        return -OOFP*tmp/(r*r*r);
}



double dGk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        //vtx v = (evlpoint - elempoint);
        vtx v = (evlpoint - elempoint );
        double tmp = v.dot(elemnormal);
        double r = v.distance();
	if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral
		//r = DISTANCE_THRESHOLD;
        //return (-OOFP * tmp*exp(-kappa*r)*(1.0+kappa*r))/(r*r*r);
        return (OOFP * tmp*exp(-kappa*r)*(1.0+kappa*r))/(r*r*r);
}
double dGkx(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        //vtx v = (evlpoint - elempoint);
        vtx v = (evlpoint - elempoint );
        double tmp = v.dot(evlnormal);
        double r = v.distance();
	if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral
		//r = DISTANCE_THRESHOLD;
        return (-OOFP * tmp*exp(-kappa*r)*(1.0+kappa*r))/(r*r*r);        
}


double ddG0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double 
epsilon)
{
	double OOFP = 1.0/(4.0*PI);
        vtx v = (evlpoint - elempoint);
	double c1 = cosine(evlnormal,v);
        double c2 = cosine(elemnormal,v);

        double r = v.distance();
        if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral

	//return OOFP*(elemnormal.dot(evlnormal) - 3.0*c1*c2)/(r*r*r);
	
	/*
	double v1,v2;
	v1 = OOFP*(elemnormal.dot(evlnormal) - 3.0*c1*c2)/(r*r*r);
	v2 = OOFP*(elemnormal.dot(evlnormal)/(r*r*r) - 3.0*v.dot(evlnormal)*v.dot(elemnormal)/(r*r*r*r*r));
	if (v1 != v2) {
	  cout << "Mismatch " << v1 << " " << v2 << endl;
	}
	*/

	// arand version: should be the same:
	return OOFP*(elemnormal.dot(evlnormal)/(r*r*r) - 3.0*v.dot(evlnormal)*v.dot(elemnormal)/(r*r*r*r*r));



}

// original 

double ddGk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
	vtx v = (evlpoint - elempoint);
        double c1 = cosine(evlnormal,v);
        double c2 = cosine(elemnormal,v);

        double r = v.distance();
        if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral

        return (1.0+kappa*r)*exp(-kappa*r)*ddG0(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon) -
		OOFP*kappa*kappa*exp(-kappa*r)*c1*c2/(r);

}


 /*
//arand version
double ddGk(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
  double OOFP = 1.0/(4.0*PI);
  vtx v = (evlpoint - elempoint);
  double cevl = cosine(evlnormal,v);
  double celem = cosine(elemnormal,v);
  double dotnormals = evlnormal.dot(elemnormal);
  
  
  double r = v.distance();
  if(r < DISTANCE_THRESHOLD)
    return 0.0; // using singular integral
    
  double value = exp(-kappa*r)*
    (
     dotnormals*kappa*G0(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon)/r
     -kappa*kappa*celem*cevl*G0(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon)
     -kappa*cevl*dG0(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon)
     + kappa*celem*dG0x(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon)
     + ddG0(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon)
     );
  
  
  return value;
}
 */


double ddGkmG0(vtx evlpoint, vtx evlnormal, vtx elempoint, vtx elemnormal, double kappa, double epsilon)
{
	double OOFP = 1.0/(4.0*PI);
	vtx v = (evlpoint - elempoint);
        double c1 = cosine(evlnormal,v);
        double c2 = cosine(elemnormal,v);

        double r = v.distance();
        if(r < DISTANCE_THRESHOLD)
                return 0.0; // using singular integral

        //return ((1.0+kappa*r)*exp(-kappa*r)-1.0)*ddG0(evlpoint,evlnormal,elempoint,elemnormal,kappa,epsilon) 
	//	-kappa*kappa*exp(-kappa*r)*c1*c2/(r);
	return OOFP*(kappa*kappa*(c1*c2-elemnormal.dot(evlnormal))/(2.0*r) +
		kappa*kappa*kappa*elemnormal.dot(evlnormal)/6.0);
}

