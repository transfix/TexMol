#include <fast-hydro/pseudoGsol.h>
#include <fast-resCont/resContFilter.h>
#include <array>
#include <iostream>
#include <fstream>
#include <vector>
#include <UsefulMath/Matrix.h>
#include <fast-resCont/miscIdent.h>
#include <vis-lib/getVis.h>
#include <TexMol/compat.h>
#define GRIDSIZE 8

using CCVOpenGLMath::Matrix;

enum
{
    RESIDUE_CONT = 0,
    PSEUDO_GSOL
};

class Point4D {
public:
    Point4D():v(0){};
    Point4D(double _x, double _y, double _z, double _v):x(_x), y(_y), z(_z), v(_v){};
    double x;
    double y;
    double z;
    double v;
};

double computeArea(Point4D &a, Point4D &b, Point4D &c) {
    double x1 = b.x - a.x;
    double y1 = b.y - a.y;
    double z1 = b.z - a.z;
    double x2 = c.x - a.x;
    double y2 = c.y - a.y;
    double z2 = c.z - a.z;
    double area = y1 * z2 - z1 * y2 - x1 * z2 + x2 * z1 + x1 * y2 - x2 * y1;
    return area > 0 ? area : - area;
}

vector<string> split(string &s) {
    vector<string> results;
    char *p;
    std::vector<char> buffer(s.size() + 1);
    strcpy(buffer.data(), s.c_str());
    p = strtok(buffer.data(), " ");
    results.push_back(string(p));
    p = strtok(NULL, " ");
    while (p != NULL) {
        results.push_back(string(p));
        p = strtok(NULL, " ");
    }
    return results;
}
double dist(Point4D &a, Point4D &b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

void double2rgb(double x, double &r, double &g, double &b) {
    if (x < 0) {
        b = 1.0;
        r = max(1 + x, 0.0);
        g = max(1 + x, 0.0);
    }
    else if (x > 0) {
		r = 1.0;
		g = min(1 - x, 1.0);
		b = min(1 - x, 1.0);
        
    }
    else {
        r = 1.0;
        g = 1.0;
        b = 1.0;
    }
}

void printVector(vector<double> &values)
{
	for (int i = 0; i < values.size(); i ++)
		cout << values[i] << endl;
}

void printVector(vector<Point> &values)
{
	for (int i = 0; i < values.size(); i ++)
		cout << values[i].getX() << " " << values[i].getY() << " " << values[i].getZ() << endl;
}

/*-------------------------------------------------------------------------
 * Convert quad values to mesh values
 * @quadValues: input quad values
 * @quadWeights: weights for each quad values
 * @density: number of quad points per triangle
 * @rawnFile: mesh file
 * @color: output mesh color
 -------------------------------------------------------------------------*/
int quad2mesh(vector<double> &quadValues, vector<double> &quadWeights, int density, const char* rawnFile, vector<Point> &color) {
    
    int numFaces;
    int numVertices;
    int posQ = 0;

    ifstream ifs;
    ifs.open(rawnFile);
   
    if (ifs.is_open()) {
        string s;
        getline(ifs, s);
        vector<string> title = split(s);
        numVertices = atoi(title[0].c_str());
        numFaces = atoi(title[1].c_str());
        std::vector<Point4D> vertices(numVertices);
        std::vector<Point4D> normal(numVertices);
        
        std::vector<double> weights(numVertices);
        std::vector<std::array<int,3>> index(numFaces);
        
        for (int i = 0; i < numVertices; i ++) {
            weights[i] = 0;
        }
        
        for (int i = 0; i < numVertices; i ++) {
            getline(ifs, s);
            vector<string> t = split(s);
            vertices[i] = Point4D(atof(t[0].c_str()), atof(t[1].c_str()), atof(t[2].c_str()), 0);
            normal[i] = Point4D(atof(t[3].c_str()), atof(t[4].c_str()), atof(t[5].c_str()), 0);
        }

        for (int i = 0; i < numFaces; i ++) {
            getline(ifs, s);
            vector<string> t = split(s);
            index[i][0] = atoi(t[0].c_str());index[i][1] = atoi(t[1].c_str());index[i][2] = atoi(t[2].c_str());
            double area = 0;
            double v = 0;
            for (int j = 0; j < density; j ++) {
                area = area + quadWeights[ posQ ];
                v = v + quadValues[ posQ++ ];
            }
            v = v / density;
            area = area / density;
             
			//compuate values for each vertex
            weights[index[i][0]] += area;
            weights[index[i][1]] += area;
            weights[index[i][2]] += area;
            
            vertices[index[i][0]].v += v;
            vertices[index[i][1]].v += v;
            vertices[index[i][2]].v += v;
        }
        
        //map from values to rgb and output
        for (int i = 0; i < numVertices; i ++) {
            double r = 1, g = 1, b = 1;
            if (weights[i] == 0)
                vertices[i].v = 0;
            else
                vertices[i].v = vertices[i].v / weights[i];
            double2rgb(vertices[i].v, r, g, b);
            color.push_back(Point(r, g, b)); 
        }
    }
    ifs.close();
    return 0;
    
}

int atom2mesh(vector<Point> &atomPoints, vector<double> &atomValues, const char *rawnFile, vector<Point> &color ) {
    double minX = 100000, maxX = -10000, minY = 10000, maxY = -10000, minZ = 10000, maxZ = -10000;
    double unitX, unitY, unitZ;
    
	vector<Point4D> atoms;
    //uniform space grid to store atoms
    vector<Point4D> grid[GRIDSIZE][GRIDSIZE][GRIDSIZE];
	for (int i = 0; i < atomPoints.size(); i ++)
    {
        Point4D pt(atomPoints[i].getX(), atomPoints[i].getY(), atomPoints[i].getZ(), atomValues[i]/7.0);
        atoms.push_back(pt);
        minX = min(pt.x, minX);
        minY = min(pt.y, minY);
        minZ = min(pt.z, minZ);
        maxX = max(pt.x, maxX);
        maxY = max(pt.y, maxY);
        maxZ = max(pt.z, maxZ);
    }
    double offset = 0.001;
    minX = minX - offset;
    minY = minY - offset;
    minZ = minZ - offset;
    maxX = maxX + offset;
    maxY = maxY + offset;
    maxZ = maxZ + offset;
    
    unitX = (maxX - minX) / GRIDSIZE;
    unitY = (maxY - minY) / GRIDSIZE;
    unitZ = (maxZ - minZ) / GRIDSIZE;
    
    for (int i = 0; i < atoms.size(); i ++) {
        int indexX = (int)((atoms[i].x - minX) / unitX);
        int indexY = (int)((atoms[i].y - minY) / unitY);
        int indexZ = (int)((atoms[i].z - minZ) / unitZ);
        grid[indexX][indexY][indexZ].push_back(atoms[i]);
    }
    
    
    //read rawn file
    int numFaces;
    int numVertices;
    
	ifstream ifs;
    ifs.open(rawnFile);
    if (ifs.is_open()) {
        string s;
        getline(ifs, s);
        vector<string> title = split(s);
        numVertices = atoi(title[0].c_str());
        numFaces = atoi(title[1].c_str());
        std::vector<Point4D> vertices(numVertices);
        std::vector<Point4D> normal(numVertices);
        
        std::vector<std::array<int,3>> index(numFaces);
        
        for (int i = 0; i < numVertices; i ++) {
            getline(ifs, s);
            vector<string> t = split(s);
            vertices[i] = Point4D(atof(t[0].c_str()), atof(t[1].c_str()), atof(t[2].c_str()), 0.0);
            normal[i] = Point4D(atof(t[3].c_str()), atof(t[4].c_str()), atof(t[5].c_str()), 0.0);
            
            int indexX = (int)((vertices[i].x - minX) / unitX);
            int indexY = (int)((vertices[i].y - minY) / unitY);
            int indexZ = (int)((vertices[i].z - minZ) / unitZ);
            
            double minDist = 10000, v = 0;
            for (int x = max(indexX - 1, 0); x <= min(indexX + 1, GRIDSIZE - 1); x ++)
                for (int y = max(indexY - 1, 0); y <= min(indexY + 1, GRIDSIZE - 1); y ++)
                    for (int z = max(indexZ - 1, 0); z <= min(indexZ + 1, GRIDSIZE - 1); z ++)
                        for(int k = 0; k < (int)grid[x][y][z].size(); k ++) {
                            if (dist(vertices[i], grid[x][y][z][k]) < minDist) {
                                minDist = dist(vertices[i], grid[x][y][z][k]);
                                v = grid[x][y][z][k].v;
                            }
						}
            vertices[i].v = v;
        }

        //map from values to rgb and output
        int pos = 0, neg = 0;
        for (int i = 0; i < numVertices; i ++) {
            double r = 1, g = 1, b = 1;
            double2rgb(vertices[i].v, r, g, b);
            if (vertices[i].v > 0)
                pos ++;
            if (vertices[i].v < 0)
                neg ++;
			color.push_back(Point(r, g, b));
        }
        ifs.close();
    }
    return 0;
    
}


/*--------------------------------------------------------------------------------------
 * @major functionalify of getVis library
 * @trans: 3 by 4 transformation matrix
 * @transI: invert of trans
 * @paramFile: parameter file for pseodo filter
 * @staticPQR/movingPQR: input pqr files
 * @staticRawN/movingRawn: input mesh file
 * @filterType: type of filter to visualize (0: residue contact 1: pseudo gsol
 --------------------------------------------------------------------------------------*/

DockVis::DockVis(char *paramFile, char *staticPQR, char *movingPQR)
{
	rcFilter = NULL;
	initResContFilter( staticPQR, movingPQR, NULL, &rcFilter);
	pGsol = new pseudoGsol(paramFile, 1);
}

DockVis::~DockVis()
{
	delete rcFilter;
	delete pGsol;
}

void DockVis::getVis(double *trans, double *transI, const char *staticRawn, const char *movingRawn, int filterType, vector<Point> &staticColors, vector<Point> &movingColors, double& filterValue)
{
	if (filterType == RESIDUE_CONT)
	{
		Matrix transMat(trans[0], trans[1], trans[2], trans[3],
						trans[4], trans[5], trans[6], trans[7], 
						trans[8], trans[9], trans[10], trans[11],
						0.0, 0.0, 0.0, 1.0);

//		resContFilter *rcFilter = NULL;

		vector<Point> staticAtoms, movingAtoms;
		vector<double> staticAtomValues, movingAtomValues;

//		initResContFilter( staticPQR, movingPQR, NULL, &rcFilter);

		rcFilter->getScores( transMat, staticAtoms, movingAtoms, staticAtomValues, movingAtomValues, filterValue);
		atom2mesh(staticAtoms, staticAtomValues, staticRawn, staticColors);
		atom2mesh(movingAtoms, movingAtomValues, movingRawn, movingColors);

		
//		delete rcFilter;
	}
	else if (filterType == PSEUDO_GSOL)
	{
//		pseudoGsol *pGsol = new pseudoGsol(paramFile, 1);
		vector<double> staticQuadValues, staticQuadWeights, movingQuadValues, movingQuadWeights;
		pGsol->getScores(trans, transI, staticQuadValues, staticQuadWeights, movingQuadValues, movingQuadWeights, filterValue);
		quad2mesh(staticQuadValues, staticQuadWeights, 1, staticRawn, staticColors);
		quad2mesh(movingQuadValues, movingQuadWeights, 1, movingRawn, movingColors);
//		delete pGsol;
	}
}

/*------------------------------------------------------
 *function test for getVis libibrary
 -------------------------------------------------------*/
/*
void test()
{
	double trans[] = {1,0,0,0,0,1,0,0,0,0,1,0};
	double *transI = trans;
	vector<Point> staticColors, movingColors;
	getVis(trans, transI, "1EAW.inp", "1EAW_r_u.pqr", "1EAW_l_u.pqr", "1EAW_r_u_imp.rawn", "1EAW_l_u_imp.rawn", 1, staticColors, movingColors);
}*/
