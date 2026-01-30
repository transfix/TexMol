#include <stdlib.h>
#include <vector>

using namespace std;

class DockVis
{
	resContFilter *rcFilter;
	pseudoGsol *pGsol;
	
	public:
		DockVis(char *paramFile, char *staticPQR, char *movingPQR);
		~DockVis();
		void getVis(double *trans, double *transI, const char *staticRawn, const char *movingRawn, int filterType, vector<Point> &staticColors, vector<Point> &movingColors, double& filterValue);
//		void test();
};
