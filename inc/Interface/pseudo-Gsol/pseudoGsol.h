#ifndef PSEUDO_GSOL_H

#define PSEUDO_GSOL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <climits>
#include <time.h>
#include <pthread.h>
#include <Interface/pseudo-Gsol/utils.h>

#ifdef _WIN32
   #include <sys/types.h>
   #include <sys/timeb.h>
#else
   #include <sys/time.h>
#endif

#include <DPG/PG.h>

#ifdef LID
   #undef LID
#endif

#define LID( r, c ) ( ( ( r ) * 4 ) + ( c ) )

class pseudoGsol
{
 private:

    typedef struct
      {
        char *staticMoleculeQUAD;
        char *movingMoleculeQUAD;    
        double distanceCutoff;           
      } PARAMS_IN;
    
    typedef struct
      {
       int id;
       double x, y, z;
       double r, h;
      } ATOM;
    
    typedef struct
      {
	double x, y, z;
	double w, h;     
      } QPOINT;
      
    typedef struct
      {
       double cx, cy, cz;
       double cr;
       bool leaf;
       int cPtr[ 8 ];
       int qPtsStartID, qPtsEndID;
      } QPOINTS_OCTREE_NODE;    
    
    typedef struct
      {
       double cx, cy, cz;
       double cr;
       bool leaf;
       int cPtr[ 8 ];
       int atomsStartID, atomsEndID;
      } ATOMS_OCTREE_NODE;    

    
    PARAMS_IN params;
    
    int numStaticAtoms, numMovingAtoms;
    
    ATOM *staticAtoms;    
    ATOM *movingAtoms;    

    int numStaticQPoints, numMovingQPoints;
                    
    QPOINT *staticQPoints;    
    QPOINT *movingQPoints;    
                
    DPG::Point *staticQPointsPG;
    DPG::Point *movingQPointsPG;
               
    DPG::PG *staticPG;
    DPG::PG *movingPG;

    vector<int> staticTriangleIds;
    vector<int> movingTriangleIds;
    
    int numStaticAtomsOctreeNodes;
    ATOMS_OCTREE_NODE *staticAtomsOctree;
    int staticAtomsOctreeRoot;

    int numMovingAtomsOctreeNodes;
    ATOMS_OCTREE_NODE *movingAtomsOctree;
    int movingAtomsOctreeRoot;

    int numStaticQPointsOctreeNodes;
    QPOINTS_OCTREE_NODE *staticQPointsOctree;
    int staticQPointsOctreeRoot;
    bool *staticQPointsOctreeFlags;

    int numMovingQPointsOctreeNodes;
    QPOINTS_OCTREE_NODE *movingQPointsOctree;
    int movingQPointsOctreeRoot;
    bool *movingQPointsOctreeFlags;    
    
    int numThreads;

    pthread_mutex_t nodesLock;
    int curNode, maxNode;   
    
    double minRadius;
    int maxLeafSize;    
      
    void freeMemory( void );
    bool getParamsFromFile( PARAMS_IN *p, char *paramFile );
    double determinant( double *trans );
    void invert( double *trans, double *transI );    
    inline void transformPoint( double x, double y, double z, double *transMat, double *nx, double *ny, double *nz );
    inline void transformPoint( DPG::Point p, double *transMat, DPG::Point *np );
    double computeXlateForPG( int numStQPoints, QPOINT *stQPoints, int numMvQPoints, QPOINT *mvQPoints );
    bool readQPoints( char *qPtsFile, int *nQPoints, QPOINT **qPoints );
    bool copyAtomsFromArray( int numAtoms, double *atms, int *nAtoms, ATOM **atoms );
    void initFreeNodeServer( int numNodes );
    int nextFreeNode( void );
    void countAtomsOctreeNodesAndSortAtoms( ATOM *atoms, int atomsStartID, int atomsEndID, ATOM *atomsT, int *numNodes );
    int constructAtomsOctree( int atomsStartID, int atomsEndID, ATOM *atoms, ATOMS_OCTREE_NODE *atomsOctree );
    bool buildAtomsOctree( int nAtoms, ATOM *atoms, int *numAtomsOctreeNodes, ATOMS_OCTREE_NODE **atomsOctree, int *atomsOctreeRoot );
    void countQPointsOctreeNodesAndSortQPoints( QPOINT *qPts, int qPtsStartID, int qPtsEndID, 
                                                QPOINT *qPtsT, int *numNodes );
    int constructQPointsOctree( int qPtsStartID, int qPtsEndID, QPOINT *qPts, QPOINTS_OCTREE_NODE *qPointsOctree );
    bool buildQPointsOctree( int nQPoints, QPOINT *qPoints, int *numQPointsOctreeNodes, QPOINTS_OCTREE_NODE **qPointsOctree, int *qPointsOctreeRoot );
    void assignHydrophobicityToQPoints( ATOMS_OCTREE_NODE *atomsOctree, int nodeA, ATOM *atoms,
                                        QPOINTS_OCTREE_NODE *qPointsOctree, int nodeQ, QPOINT *qPoints,
                                        double farDist, double rangeExt );
    void initOctreeFlags( int threadID );
    void collectPseudoGsol( int threadID, double *trans, double *transI, double *pGsol, 
                            double *pGsolHStaticPos, double *pGsolHStaticNeg, double *pGsolHMovingPos, double *pGsolHMovingNeg );
    void markPotentialQPoints( int threadID, int nodeS, int nodeM, double *trans );

    bool readAtoms( char *atomsFile, int *nAtoms, double **atoms );
                                            
 public: 
 
    
    pseudoGsol( char *staticRawFile, char *movingRawFile, char *staticMolFile, char *movingMolFile, double dist, int nThreads );
    ~pseudoGsol( );

    void getPseudoGsol( int threadID, double *trans, double *pGsol,
                                    double *pGsolHStaticPos, double *pGsolHStaticNeg, double *pGsolHMovingPos, double *pGsolHMovingNeg );
    void getPseudoGsol( int threadID, double *pGsol, double *pGsolHStaticPos, double *pGsolHStaticNeg, 
                                    double *pGsolHMovingPos, double *pGsolHMovingNeg );
    void getPseudoGsol( int threadID, double *trans, double *pGsol, double *pGsolH );
    void getPseudoGsol( int threadID, double *pGsol, double *pGsolH );

    void getInterfacePoints(double *trans, char *staticFile, char *movingFile);
    
    void printGsolParamters( FILE* fp );        
};


#endif
