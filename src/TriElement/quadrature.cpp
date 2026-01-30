#include <cstring>
#include <cstdio>
#include "quadrature.h"
#include <iostream>

using namespace std;
using namespace TriElementNS;

// 1D Gaussian Quadrature points and weights
const double _GP[] = {0.0,
			-0.577350269, 0.577350269,
			-0.774596669241483,0.0, 0.774596669241483,
			-0.861136312, -0.339981044,0.339981044,0.861136312,
			-0.906179845938664,-0.538469310105683,0.0,0.906179845938664, 0.538469310105683,
			-0.93246951, -0.66120939,-0.23861918, 0.23861918, 0.66120939, 0.93246951,
			-0.949107912342759, -0.741531185599394, -0.405845151377397,0.000000000000000,0.405845151377397,0.741531185599394,0.949107912342759,
			-0.96028986,-0.79666648,-0.52553241,-0.18343464,0.18343464,0.52553241,0.79666648,0.96028986};

  const double _GW[] = {2.0,
			1.0,1.0,
			0.555555555555556,0.88888888888889,0.555555555555556,
			0.347854845137454,0.652145154862546,0.652145154862546,0.347854845137454,
			0.236926885056189, 0.478628670499366,0.568888888888889,0.236926885056189, 0.478628670499366,
			0.17132449,0.36076157,0.46791393,0.46791393,0.36076157,0.17132449,
			0.129484966168870, 0.279705391489277, 0.381830050505119,0.417959183673469,0.381830050505119, 0.279705391489277, 0.129484966168870,
			0.10122854,0.22238103,0.31370665,0.36268378,0.36268378,0.31370665, 0.22238103,0.10122854};
Quadrature::Quadrature()
{
	numOfPts_perTri = 0;
}

Quadrature::Quadrature(char * type, int numOfPts)
{
  if (strcmp(type, "Gaussian") == 0 || strcmp(type, "gaussian") == 0) {
    numOfPts_perTri = numOfPts;
    xi1 = new double[numOfPts_perTri];
    xi2 = new double[numOfPts_perTri];
    xi3 = new double[numOfPts_perTri];
    w = new double[numOfPts_perTri];
    
    if (numOfPts == 1)
      onePtGaussian();
    else if (numOfPts == 3)
      threePtGaussian();
    else if (numOfPts == 6)
      sixPtGaussian();
    else if (numOfPts == 7)
      sevenPtGaussian();
    else if (numOfPts == 12)
      twelvePtGaussian();
    else printf("Guassian rule: 1, 3, 6, 7, or 12 pt\n");
  } else if (strcmp(type, "Duffy") == 0 || strcmp(type, "duffy") == 0) {    
    duffy(numOfPts);
  } else if (strcmp(type, "Subdivided") == 0 || strcmp(type, "subdivided") == 0) {    
    subdivided(numOfPts);
  } else if (strcmp(type, "CenterDuffy") == 0 || strcmp(type, "centerduffy") == 0) {    
    centerduffy(numOfPts);
  }
  /*
    else if (strcmp(type, "...") == 0 )
    {
    }
  */
  else
    printf("(1, 3, 6, 7, or 12 pt) Guassian or (1-7) degree Duffy ...\n");

  //double area = 0.0;
  //for (int i=0; i<numOfPts_perTri; i++) {
  //  area += w[i];
  //}
  //cout << type << " " << area << endl;


}



// Gaussian
void Quadrature::onePtGaussian()
{
	xi1[0] = 1.0/3.0;	xi2[0] = 1.0/3.0;	xi3[0] = 1.0/3.0;
	w[0] = 1.0;
}

void Quadrature::threePtGaussian()
{
	xi1[0] = 2.0/3.0;	xi2[0] = 1.0/6.0;	xi3[0] = 1.0/6.0;
	xi1[1] = 1.0/6.0;	xi2[1] = 2.0/3.0;	xi3[1] = 1.0/6.0;
        xi1[2] = 1.0/6.0;        xi2[2] = 1.0/6.0;        xi3[2] = 2.0/3.0;

	w[0] = 1.0/3.0;
	w[1] = w[0];
	w[2] = w[0];
}

void Quadrature::sixPtGaussian()
{
	xi1[0] = 0.81684757;	xi2[0] = 0.09157621;	xi3[0] = xi2[0];
	xi1[1] = xi2[0];		xi2[1] = xi1[0];		xi3[1] = xi3[0];
	xi1[2] = xi2[0];		xi2[2] = xi2[0];		xi3[2] = xi1[0];
	xi1[3] = 0.10810302;	xi2[3] = 0.44594849;	xi3[3] = xi2[3];
	xi1[4] = xi2[3];		xi2[4] = xi1[3];		xi3[4] = xi3[3];
	xi1[5] = xi2[3];		xi2[5] = xi2[3];		xi3[5] = xi1[3];

	w[0] = 0.10995174;	w[1] = w[0];	w[2] = w[0];
	w[3] = 0.22338159;	w[4] = w[3];	w[5] = w[3];
}

void Quadrature::sevenPtGaussian()
{
	xi1[0] = 1.0/3.0;        xi2[0] = 1.0/3.0;        xi3[0] = 1.0/3.0;
	xi1[1] = 0.79742699;	xi2[1] = 0.10128651;	xi3[1] = xi2[1];
	xi1[2] = xi2[1];		xi2[2] = xi1[1];		xi3[2] = xi3[1];
	xi1[3] = xi2[1];		xi2[3] = xi2[1];		xi3[3] = xi1[1];
	xi1[4] = 0.05971587;	xi2[4] = 0.47014206;	xi3[4] = xi2[4];
	xi1[5] = xi2[4];		xi2[5] = xi1[4];		xi3[5] = xi3[4];
	xi1[6] = xi2[4];		xi2[6] = xi2[4];		xi3[6] = xi1[4];

	w[0] = 0.225;
	w[1] = 0.12593918;	w[2] = w[1];	w[3] = w[1];
	w[4] = 0.13239415;	w[5] = w[4];	w[6] = w[4];
}

void Quadrature::twelvePtGaussian()
{
	int i;

	xi1[0] = 0.87382197;	xi2[0] = 0.06308901;	xi3[0] = xi2[0];
        xi1[1] = xi2[0];          xi2[1] = xi1[0];          xi3[1] = xi3[0];
        xi1[2] = xi2[0];          xi2[2] = xi2[0];          xi3[2] = xi1[0];
	xi1[3] = 0.50142651;	xi2[3] = 0.24928675;	xi3[3] = xi2[3];
        xi1[4] = xi2[3];          xi2[4] = xi1[3];          xi3[4] = xi3[3];
        xi1[5] = xi2[3];          xi2[5] = xi2[3];          xi3[5] = xi1[3];
	xi1[6] = 0.63650250;	xi2[6] = 0.31035245;	xi3[6] = 0.05314505;
	xi1[7] = xi3[6];		xi2[7] = xi1[6];		xi3[7] = xi2[6];
	xi1[8] = xi3[7];		xi2[8] = xi1[7];		xi3[8] = xi2[7];
	xi1[9] = xi1[6];		xi2[9] = xi3[6];		xi3[9] = xi2[6];
	xi1[10] = xi3[9];		xi2[10] = xi1[9];		xi3[10] = xi2[9];
	xi1[11] = xi3[10];	xi2[11] = xi1[10];	xi3[11] = xi2[10];

	w[0] = 0.05084491;	w[1] = w[0];	w[2] = w[0];
	w[3] = 0.11678628;	w[4] = w[3];	w[5] = w[3];
	for (i = 6; i < 12; i++)
		w[i] = 0.08285108;

}

// Newton-Cotes
//void Quadrature::Newton-Cotes();




//
// rule below is for handing quadrature when there is a singularity at the (1,0) vertex of the triangle
//
void Quadrature::duffy(int numOfPts) {
  if (numOfPts <1 || numOfPts > 7) {
    cout << "Duffy quadrature only supported up to degree 7.  Your quadrature degree: " << numOfPts << endl;
  }

  double * points1D = new double[numOfPts];
  double * weights1D = new double[numOfPts];

  int ruleLoc = numOfPts*(numOfPts - 1)/2;
  
  for (int i=0; i< numOfPts; i++) {
    points1D[i] = (_GP[ruleLoc+i]+1.0)/2.0;  // shift from [-1.1] to [0,1]
    weights1D[i] = (_GW[ruleLoc+i])/2.0;  // shift from [-1.1] to [0,1]
  }

  numOfPts_perTri = numOfPts*numOfPts;

  xi1 = new double[numOfPts_perTri];
  xi2 = new double[numOfPts_perTri];
  xi3 = new double[numOfPts_perTri];
  w = new double[numOfPts_perTri];

  int count = 0;
  for (int i=0; i<numOfPts; i++) {

    double x = points1D[i];
    double wx = weights1D[i];
    for (int j=0; j<numOfPts; j++) {
      double y = points1D[j];
      double wy = weights1D[j];

      
      // set up the quadrature rule here...
      double x1 = x;
      double y1 = (1-x)*y;

      xi1[count] = x1;
      xi2[count] = y1;
      xi3[count] = 1.0 - x1 - y1;
      w[count] = (1 - x)*wy*wx*2.0;  
      // note: multiple by two so that the sum of the weights is 1 
      // even though the actual triangle area is .5

      //cout << count << " " << x << "," << y << " | " << wx << "," << wy
      //	   << " | " << x1 << ","<< y1 << ","<< 1.0 - x1 - y1 << " " << (1 - x)*wy*wx << endl;
      
      count++;
    }
  }
  
  delete [] points1D;
  delete [] weights1D;
}


void Quadrature::subdivided(int numOfPts) {

  int gaussRule = 1;

  double xi1ref[1];
  double xi2ref[1];
  double xi3ref[1];
  double wref[1];

  xi1ref[0] = 1.0/3.0;	xi2ref[0] = 1.0/3.0;	xi3ref[0] = 1.0/3.0;
  wref[0] = 1.0;

  numOfPts_perTri = gaussRule*numOfPts*numOfPts;


  double subSize = 1.0/numOfPts;
  

  //cout << subSize << " x " << endl;

  xi1 = new double[numOfPts_perTri];
  xi2 = new double[numOfPts_perTri];
  xi3 = new double[numOfPts_perTri];
  w = new double[numOfPts_perTri];


  int count = 0;

  //double totalw = 0.0;

  double xoff = 0.0;
  double yoff = 0.0;
  for (int i=0; i<numOfPts; i++) {
    for (int j=0; i+j< numOfPts; j++) {

      for (int q=0; q<gaussRule; q++) {
	xi1[count] = xi1ref[q]*subSize + xoff;
	xi2[count] = xi2ref[q]*subSize + yoff;
	xi3[count] = 1.0 - xi1[count] - xi2[count];
	w[count] = wref[q]*subSize*subSize;
	//totalw += w[count];
	count++;

      }

      if (i+j+1 < numOfPts) {
	for (int q=0; q<gaussRule; q++) {
	  xi1[count] = subSize - xi1ref[q]*subSize + xoff;
	  xi2[count] = subSize - xi2ref[q]*subSize + yoff;
	  xi3[count] = 1.0 - xi1[count] - xi2[count];
	  w[count] = wref[q]*subSize*subSize;	  
	  //totalw += w[count];
	  count++;

	}
      }


      yoff += subSize;
    }
    xoff += subSize;
    yoff = 0.0;

  }

  //for (int q=0; q<count; q++) {
  //  cout << xi1[q] << " " << xi2[q] << " " << xi3[q] << " " << w[q] << endl;
  //}


  //cout << "Quadrature " << count << " " << numOfPts << endl;


}



void Quadrature::centerduffy(int numOfPts) {

  // get the base rule...
  duffy(numOfPts);
  
  // copy this info and allocate the actual quadrature rule...

  int nDuffy = numOfPts_perTri;
  numOfPts_perTri *= 3;
  
  double * xi1duffy = xi1;
  double * xi2duffy = xi2;
  double * xi3duffy = xi3;
  double * wduffy = w;
  
  xi1 = new double[numOfPts_perTri];
  xi2 = new double[numOfPts_perTri];
  xi3 = new double[numOfPts_perTri];
  w = new double[numOfPts_perTri];



  double m00,m01,m10,m11;
  double offx,offy;

  int n = 0;
  // triangle 1
  offx = 0.0;
  offy = 0.0;
  m00 = 1.0/3.0;
  m10 = 1.0/3.0;
  m01 = 0.0;
  m11 = 1.0;

  for (int q=0; q<nDuffy; q++) {
    xi1[q+n*nDuffy] = xi1duffy[q]*m00 + xi2duffy[q]*m01 + offx;
    xi2[q+n*nDuffy] = xi1duffy[q]*m10 + xi2duffy[q]*m11 + offy;
    xi3[q+n*nDuffy] = 1.0 - xi1[q+n*nDuffy] - xi2[q+n*nDuffy];
    w[q+n*nDuffy] = wduffy[q]/3.0;
  }

  
  n = 1;
  // triangle 2
  offx = 0.0;
  offy = 0.0;
  m00 = 1.0/3.0;
  m10 = 1.0/3.0;
  m01 = 1.0;
  m11 = 0.0;

  for (int q=0; q<nDuffy; q++) {
    xi1[q+n*nDuffy] = xi1duffy[q]*m00 + xi2duffy[q]*m01 + offx;
    xi2[q+n*nDuffy] = xi1duffy[q]*m10 + xi2duffy[q]*m11 + offy;
    xi3[q+n*nDuffy] = 1.0 - xi1[q+n*nDuffy] - xi2[q+n*nDuffy];
    w[q+n*nDuffy] = wduffy[q]/3.0;
  }

  n = 2;
  //triangle 3
  offx = 1.0;
  offy = 0.0;
  m00 = -2.0/3.0;
  m10 = 1.0/3.0;
  m01 = -1.0;
  m11 = 1.0;


  for (int q=0; q<nDuffy; q++) {
    xi1[q+n*nDuffy] = xi1duffy[q]*m00 + xi2duffy[q]*m01 + offx;
    xi2[q+n*nDuffy] = xi1duffy[q]*m10 + xi2duffy[q]*m11 + offy;
    xi3[q+n*nDuffy] = 1.0 - xi1[q+n*nDuffy] - xi2[q+n*nDuffy];
    w[q+n*nDuffy] = wduffy[q]/3.0;
  }


}
