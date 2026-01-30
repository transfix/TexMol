#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include "quadrature.h"
#include "trielement.h"

namespace TriElementNS
{
        class Vertex;
        class Face;
        class ASpline : public TriElement
        {
        public:
                ASpline();
                void Triangle_Bezier_Patch(int resolution);
		void Triangle_Patch_Nodes(char *type, int numOfPts);

        private:
                void Cubic_Bezier_Coeffs(double *p1, double *p2, double *p3, double *n1,double *n2,double *n3,
                                  double *b300, double *b210, double *b120, double *b030, double *b021, double *b012, double *b003, double *b102, double *b201);
                void Simple_Bezier_B111(double *p1, double *p2, double *p3, double *n1, double *n2, double *n3,
                                 double *b300, double *b210, double *b120, double *b030, double *b201, double *b021, double *b111);
		void Triangle_Patch(int t, double *p1, double *p2, double *p3, double *n1, double *n2, double *n3, 
				    double *b300, double *b210, double *b120, double *b030, double *b021, double *b012, double *b003, 
				    double *b102, double *b201, double *b111_1, double *b111_2, double *b111_3, int resolution);
                void Triangle_Patch_Nodes(TriElementNS::Quadrature *my_quadr, int i, double *p1, double *p2, double *p3, double *n1, double *n2, double *n3, 
                                          double *b300, double *b210, double *b120, double *b030, double *b021, double *b012, double *b003, 
                                          double *b102, double *b201, double *b111_1, double *b111_2, double *b111_3);
                double BB2lmd(double *b300, double *b210, double *b120, double *b030, double *b021, double *b012, double *b003,
                             double *b102, double *b201, double *b111_1, double *b111_2, double *b111_3, double b1, double b2, double b3);
                double Bezier_LambdaPolyZeroC1(double *F1, double *N1, double *N2, double *N3, double *D1, double *D2, double *D3);
                double Bezier_ThreeLambdaPolyValueC1(double *F1, double *N1, double *N2, double *N3, double *D1, double *D2,double *D3, double lmd);
                double Bezier_ThreeLambdaDervPolyValueC1(double *F1, double *N1, double *N2, double *N3, double *D1, double *D2,double *D3, double lmd);
                void BB_Gradient(double *p1_lmd, double *p2_lmd, double *p3_lmd, double *n1, double *n2, double *n3, 
                                 double *b300, double *b210, double *b120, double *b030, double *b021, double *b012, double *b003, 
                                 double *b102, double *b201, double *b111_1, double *b111_2, double *b111_3,
                                 double b1, double b2, double b3, double lmd, double *n);
	};
}

#endif //SURFACE_H

