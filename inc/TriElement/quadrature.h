#ifndef TRIELEMENT_QUADRATURE_H
#define TRIELEMENT_QUADRATURE_H

namespace TriElementNS {


class Quadrature
{
public:
	Quadrature();
	Quadrature(char *type, int numOfPts);
	int numOfPts_perTri;
	double *xi1, *xi2, *xi3, *w;

	void onePtGaussian();
	void threePtGaussian();
	void sixPtGaussian();
	void sevenPtGaussian();
	void twelvePtGaussian();

	void duffy(int numOfPts);
	void subdivided(int numOfPts);
	void centerduffy(int numOfPts);

};

}

#endif //QUADRATURE_H

