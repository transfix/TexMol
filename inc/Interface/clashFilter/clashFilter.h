#ifndef CLASH_FILTER_H

#define CLASH_FILTER_H


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <climits>
#include <time.h>
#include <pthread.h>
#include "utils.h"

#ifdef _WIN32
   #include <sys/types.h>
   #include <sys/timeb.h>
#else
   #include <sys/time.h>
#endif

#include "utils.h"

#ifdef LID
   #undef LID
#endif

#define LID( r, c ) ( ( ( r ) * 4 ) + ( c ) )


class clashFilter
{
 private:

  typedef struct
   {
     float x;
     float y;
     float z;
   } Point;

   typedef struct
    {
     int id;
     double x, y, z;
     double q, r;
    } ATOM;

   typedef struct
    {
     double cx, cy, cz;
     double cq, cr;
     bool leaf;
     int cPtr[ 8 ];
     int atomsStartID, atomsEndID;
    } ATOMS_OCTREE_NODE;    

   typedef struct
    {
     clashFilter *cF;
     double *trans;
     int nClashes, nSevereClashes;
     double interactionValue;
    } THREAD_RESULT;

   ATOM *staticAtoms;
   int nStaticAtoms;      

   int numStaticAtomsOctreeNodes;
   bool staticAtomsOctreeBuilt;
   ATOMS_OCTREE_NODE *staticAtomsOctree;
   int staticAtomsOctreeRoot;

   ATOM *movingAtoms;
   int nMovingAtoms;    

   std::vector<int> staticAtomIds;
   std::vector<int> movingAtomIds;

   int numMovingAtomsOctreeNodes;
   bool movingAtomsOctreeBuilt;
   ATOMS_OCTREE_NODE *movingAtomsOctree;
   int movingAtomsOctreeRoot;
   
   bool priorComputationCleared;
   
   bool computedInteractionNaively;

   double minRadius, minRadiusUsed;
   int maxLeafSize, maxLeafSizeUsed;
   
   double epsilon;
   
   pthread_mutex_t nodesLock;
   int curNode, maxNode;   

   pthread_mutex_t subtreeRootsLock;
   int curSubtreeRoot, maxSubtreeRoot;   
   int *movingAtomsSubtreeRoots;
   int movingAtomsSubtreeRootsSize;
   
   double *trans;   
   double clashFrac, severeClashFrac, fuzzyFrac;     
      
   int numThreads; 
   double distCutoff;  
   
   bool printStatus;

   void printError( const char *format, ... );
   double getTime( void );   
   void freeMemory( void );
   void setDefaults( void );
   bool allocateMovingAtomsSubtreeRootsArray( int nThreads );   
   void initFreeNodeServer( int numNodes );   
   int nextFreeNode( void );
   void initSubtreeRootServer( );   
   int nextSubtreeRoot( void );   
    inline void transformPoint( double x, double y, double z, double *transMat, double *nx, double *ny, double *nz );
    inline void transformPoint( Point p, double *transMat, Point *np );
   bool copyAtomsFromArray( int numAtomsSrc, double *atmsSrc, int *numAtomsDest, ATOM **atmsDest );   
   void countAtomsOctreeNodesAndSortAtoms( ATOM *sAtoms, int sAtomsStartID, int sAtomsEndID, ATOM *sAtomsT, int *numNodes );      
   int constructAtomsOctree( int atomsStartID, int atomsEndID, ATOM *atoms, ATOMS_OCTREE_NODE *atomsOctree );
   bool buildStaticAtomsOctree( void );   
   bool buildMovingAtomsOctree( void );            
   bool buildOctrees( void );         
   void fillMovingAtomsSubtreeRootsArray( int nodeID, int maxNodesInLevel );   
   void approximateInteractions( double *trans, int nodeS, int nodeM, int *nClashes, int *nSevereClashes, double *interactionValue );

   bool readAtoms( char *atomsFile, int *nAtoms, double **atoms );


   static void *approximateInteractionsThread( void *v )
    {
     THREAD_RESULT *tR = ( THREAD_RESULT * ) v;     
     
     tR->nClashes = tR->nSevereClashes = 0; 
     tR->interactionValue = 0;
     
     while ( 1 )
       {
        int nextRoot = tR->cF->nextSubtreeRoot( );     
        if ( nextRoot < 0 ) break;      
        
        int nClashes, nSevereClashes;
        double interactionValue;
        
        tR->cF->approximateInteractions( tR->trans, tR->cF->staticAtomsOctreeRoot, nextRoot, &nClashes, &nSevereClashes, &interactionValue );
        
        tR->nClashes += nClashes;
        tR->nSevereClashes += nSevereClashes;
        tR->interactionValue += interactionValue;
       }
      return nullptr;
    }

    
 public:

   clashFilter( char *staticMolFile, char *movingMolFile, double dist, int nThreads );   
      
   ~clashFilter( );
   
   bool setMinRadius( double minRad );
   bool setMaxLeafSize( int maxLfSize );
   bool setEpsilon( double eps );
   bool setNumThreads( int nThreads );

   bool setProximityFactors( double clashFactor, double severeClashFactor, double fuzzyFactor );
   bool setTransformationMatrix( double *trans );

   void setPrintStatus( bool printStat );
      
   void printCurrentSettings( void );   
   
   bool computeInteractions( int *nClashes, int *nSevereClashes, double *interactionValue );
   bool computeInteractions( double *trans, int *nClashes, int *nSevereClashes, double *interactionValue );
   void getInterfaceAtoms( double *trans, char *staticFile, char *movingFile);
   
   bool computeInteractionsNaively( int *nClashes, int *nSevereClashes, double *interactionValue );
   bool computeInteractionsNaively( double *trans, int *nClashes, int *nSevereClashes, double *interactionValue );   
};

#endif
