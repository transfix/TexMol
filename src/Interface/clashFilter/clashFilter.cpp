#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "clashFilter.h"

using namespace std;

void clashFilter::printError( char *format, ... )
{
   char eMsg[ 500 ];
   va_list args;
   
   va_start( args, format );
   
   vsprintf( eMsg, format, args );
   
   va_end( args );
   
   printf( "\nError: %s\n\n", eMsg );   
}


double clashFilter::getTime( void )
{
#ifdef _WIN32
   time_t ltime;
   _timeb tstruct;
   time( &ltime );
   _ftime( &tstruct );
   return ( double ) ( ltime + 1e-3 * ( tstruct.millitm ) );
#else
   struct timeval t;
   gettimeofday( &t, NULL );
   return ( double )( t.tv_sec + 1e-6 * t.tv_usec );
#endif
}


void clashFilter::freeMemory( void )
{
   freeMem( staticAtoms );
   freeMem( movingAtoms );
   freeMem( staticAtomsOctree );
   freeMem( movingAtomsOctree );  
   freeMem( movingAtomsSubtreeRoots ); 
}


bool clashFilter::allocateMovingAtomsSubtreeRootsArray( int nThreads )
{
   int arraySize = 1;
   
   while ( arraySize < 8 * nThreads )
      arraySize *= 8;
      
   movingAtomsSubtreeRoots = ( int * ) malloc( arraySize * sizeof( int ) );      
   
   if ( movingAtomsSubtreeRoots == NULL )
     {
      printError( "Failed to allocate memory!" );
      return false;
     }
     
   movingAtomsSubtreeRootsSize = arraySize;  
        
   return true;  
}


void clashFilter::setDefaults( void )
{
   staticAtoms = NULL;
   movingAtoms = NULL;
      
   staticAtomsOctree = NULL;
   movingAtomsOctree = NULL;
   
   movingAtomsSubtreeRoots = NULL;
   
   minRadius = 2.0;
   minRadiusUsed = -1;   
   
   maxLeafSize = 10;
   maxLeafSizeUsed = -1;   

   epsilon = 0.5;
   
   clashFrac = 0.4;
   severeClashFrac = 0.25;
   fuzzyFrac = 1.0;     
      
   staticAtomsOctreeBuilt = false;
   movingAtomsOctreeBuilt = false;
   
   priorComputationCleared = true;

   curNode = maxNode = 0;
   curSubtreeRoot = maxSubtreeRoot = 0;   
   
   staticAtomsOctreeRoot = -1;
   movingAtomsOctreeRoot = -1;
   
   numThreads = 1;

//   transMatrix.reset( );
      
   printStatus = true;
   
   pthread_mutex_init( &nodesLock, NULL );   
}


void clashFilter::printCurrentSettings( void )
{
   printf( "\nCurrent Parameter Settings:\n" );
   printf( "\tminRadius = %lf, maxLeafSize = %d, epsilon = %lf, numThreads = %d\n", minRadius, maxLeafSize, epsilon, numThreads );
   printf( "\tclasfFrac = %lf, severeClashFrac = %lf, fuzzyFrac = %lf\n\n", clashFrac, severeClashFrac, fuzzyFrac );   
}


clashFilter::clashFilter( char *staticMolFile, char *movingMolFile, double dist, int nThreads )
{
   setDefaults( );

   printStatus = true;
   distCutoff = dist;
   numThreads = nThreads;

   int nStAtoms, nMvAtoms;  
   double *stAtoms, *mvAtoms;  

   cout<<"reading atoms"<<endl;
    
   if ( !readAtoms( staticMolFile, &nStAtoms, &stAtoms )
    || !readAtoms( movingMolFile, &nMvAtoms, &mvAtoms ) )  
     exit( 1 );
         
   if ( !copyAtomsFromArray( nStAtoms, stAtoms, &nStaticAtoms, &staticAtoms ) 
     || !copyAtomsFromArray( nMvAtoms, mvAtoms, &nMovingAtoms, &movingAtoms ) )
      {
       freeMemory( );
       exit( 1 );
      }
            
   if ( !allocateMovingAtomsSubtreeRootsArray( numThreads ) )
     {
      freeMemory( );
      exit( 1 );
     }
 
   buildOctrees( );
  
   if ( printStatus ) printCurrentSettings( );
}



clashFilter::~clashFilter( )
{
   freeMemory( );
   if ( printStatus ) printf( "\n" );
}


bool clashFilter::setProximityFactors( double clashFactor, double severeClashFactor, double fuzzyFactor )
{
   if ( clashFactor < 0 )
     {
      printError( "clashFactor must be a non-negative real number!" );
      return false;     
     }
     
   clashFrac = clashFactor;
   if ( printStatus ) printf( "\nclashFactor is set to %lf\n", clashFactor );

   if ( severeClashFactor < 0 )
     {
      printError( "severeClashFactor must be a non-negative real number!" );
      return false;     
     }

   if ( severeClashFactor > clashFactor )
     {
      printError( "severeClashFactor cannot be larger than clashFactor!" );
      return false;     
     }
     
   severeClashFrac = severeClashFactor;
   if ( printStatus ) printf( "\nsevereClashFactor is set to %lf\n", severeClashFactor );
     
   if ( fuzzyFactor < 0 )
     {
      printError( "fuzzyFactor must be a non-negative real number!" );
      return false;     
     }
     
   fuzzyFrac = fuzzyFactor;
   if ( printStatus ) printf( "\nfuzzyFactor is set to %lf\n", fuzzyFactor );   
   
   return true;
}


bool clashFilter::setMinRadius( double minRad )
{
   if ( minRad < 0 )
     {
      printError( "minRadius must be a non-negative real number!" );
      return false;     
     }
     
   minRadius = minRad;

   buildOctrees( );

   if ( printStatus ) printf( "\nminRadius is set to %lf\n", minRad );
   
   return true;
}


bool clashFilter::setMaxLeafSize( int maxLfSize )
{
   if ( maxLfSize <= 0 )
     {
      printError( "maxLeafSize must be a positive integer!" );
      return false;     
     }
     
   maxLeafSize = maxLfSize;

   buildOctrees( );

   if ( printStatus ) printf( "\nmaxLeafSize is set to %d\n", maxLfSize );

   return true;
}


bool clashFilter::setEpsilon( double eps )
{
   if ( eps < 0 )
     {
      printError( "epsilon must be a non-negative real number!" );
      return false;     
     }
     
   epsilon = eps;

   if ( printStatus ) printf( "\nepsilon is set to %lf\n", eps );

   return true;
}


bool clashFilter::setNumThreads( int nThreads )
{
   if ( nThreads < 1 )
     {
      printError( "numThreads must be a positive integer!" );
      return false;     
     }
     
   int *movingAtomsSubtreeRootsT;  
   
   movingAtomsSubtreeRootsT = movingAtomsSubtreeRoots;
     
   if ( allocateMovingAtomsSubtreeRootsArray( nThreads ) )
     {
      numThreads = nThreads;
      freeMem( movingAtomsSubtreeRootsT );
      if ( movingAtomsOctreeBuilt ) fillMovingAtomsSubtreeRootsArray( movingAtomsOctreeRoot, 1 );   
      if ( printStatus ) printf( "\nnumThreads is set to %d\n", numThreads );
      return true;
     }
   else
     {
      movingAtomsSubtreeRoots = movingAtomsSubtreeRootsT;
      if ( printStatus ) printf( "\nnumThreads remains unchanged ( %d )\n", numThreads );      
      return false;
     }            
}



bool clashFilter::setTransformationMatrix( double *t )
{
   for(int i=0;i<12;i++)
   trans[i] = t[i];
   
   if ( printStatus ) 
     {
       printf( "\nTransformation Matrix is set to:\n" );
//       transMatrix.print( );
     }  
   
   return true;
}


void clashFilter::setPrintStatus( bool printStat )
{
   printStatus = printStat;
   
   if ( printStatus ) printf( "\nprintStatus is set to true\n" );
}


int clashFilter::nextFreeNode( void )
{
   int nextNode = -1;
   
   pthread_mutex_lock( &nodesLock );
   if ( curNode < maxNode ) nextNode = curNode++;
   pthread_mutex_unlock( &nodesLock );
   
   return nextNode;
}


void clashFilter::initFreeNodeServer( int numNodes )
{
//   pthread_mutex_init( &nodesLock, NULL );
   curNode = 0;
   maxNode = numNodes;
}


int clashFilter::nextSubtreeRoot( void )
{
   int nextRoot = -1;
   
   pthread_mutex_lock( &subtreeRootsLock );
   if ( curSubtreeRoot < maxSubtreeRoot ) nextRoot = curSubtreeRoot++;
   pthread_mutex_unlock( &subtreeRootsLock );
   
   return ( nextRoot == -1 ) ? ( - 1 ) : movingAtomsSubtreeRoots[ nextRoot ];
}


void clashFilter::initSubtreeRootServer( void )
{
   pthread_mutex_init( &subtreeRootsLock, NULL );
   curSubtreeRoot = 0;
}


bool clashFilter::copyAtomsFromArray( int numAtomsSrc, double *atmsSrc, int *numAtomsDest, ATOM **atmsDest )
{
   if ( printStatus ) printf( "\ncopying atoms from array... " );

   double startT = getTime( );
   
   if ( numAtomsSrc <= 0 )
     {
      printError( "No atoms to copy!" );
      return false;
     }
   
   *numAtomsDest = numAtomsSrc;
   ( *atmsDest ) = ( ATOM * ) malloc( numAtomsSrc * sizeof( ATOM ) );
   
   if ( ( *atmsDest ) == NULL )
     {
      printError( "Failed to allocate memory for atoms!" );
      return false;
     }

   for ( int i = 0; i < numAtomsSrc; i++ )   
     {          
      ( *atmsDest )[ i ].x = atmsSrc[ 4 * i + 0 ];
      ( *atmsDest )[ i ].y = atmsSrc[ 4 * i + 1 ];
      ( *atmsDest )[ i ].z = atmsSrc[ 4 * i + 2 ];
      ( *atmsDest )[ i ].r = atmsSrc[ 4 * i + 3 ];
      ( *atmsDest )[ i ].id = i;
     }    
     
   double endT = getTime( );
   
   if ( printStatus ) printf( "done ( %lf sec, copied %d atoms )\n", endT - startT, numAtomsSrc );
         
   return true;
}


void clashFilter::countAtomsOctreeNodesAndSortAtoms( ATOM *atoms, int atomsStartID, int atomsEndID, 
                                                     ATOM *atomsT, int *numNodes )
{
   double minX = atoms[ atomsStartID ].x, minY = atoms[ atomsStartID ].y, minZ = atoms[ atomsStartID ].z;
   double maxX = atoms[ atomsStartID ].x, maxY = atoms[ atomsStartID ].y, maxZ = atoms[ atomsStartID ].z, maxR = atoms[ atomsStartID ].r;
   
   for ( int i = atomsStartID + 1; i <= atomsEndID; i++ )
     {
      if ( atoms[ i ].x < minX ) minX = atoms[ i ].x;      
      if ( atoms[ i ].x > maxX ) maxX = atoms[ i ].x;      
      
      if ( atoms[ i ].y < minY ) minY = atoms[ i ].y;      
      if ( atoms[ i ].y > maxY ) maxY = atoms[ i ].y;      

      if ( atoms[ i ].z < minZ ) minZ = atoms[ i ].z;      
      if ( atoms[ i ].z > maxZ ) maxZ = atoms[ i ].z;      
      
      if ( atoms[ i ].r > maxR ) maxR = atoms[ i ].r;            
     } 
   
   double cx = ( minX + maxX ) / 2,
          cy = ( minY + maxY ) / 2,
          cz = ( minZ + maxZ ) / 2;

   double r2 = ( atoms[ atomsStartID ].x - cx ) * ( atoms[ atomsStartID ].x - cx )
             + ( atoms[ atomsStartID ].y - cy ) * ( atoms[ atomsStartID ].y - cy )
             + ( atoms[ atomsStartID ].z - cz ) * ( atoms[ atomsStartID ].z - cz );
   
   for ( int i = atomsStartID + 1; i <= atomsEndID; i++ )
     {
      double r2T = ( atoms[ i ].x - cx ) * ( atoms[ i ].x - cx )
                 + ( atoms[ i ].y - cy ) * ( atoms[ i ].y - cy )
                 + ( atoms[ i ].z - cz ) * ( atoms[ i ].z - cz );
     
      if ( r2T > r2 ) r2 = r2T;
     } 

   double cr = sqrt( r2 ) + maxR;
      
   *numNodes = 1;
      
   if ( ( atomsEndID - atomsStartID +  1 > maxLeafSize ) && ( cr > minRadius ) )
     {
      int atomsCount[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      
      for ( int i = atomsStartID; i <= atomsEndID; i++ )
        {
         atomsT[ i ] = atoms[ i ];
         
         int j = ( zeroIfLess( atoms[ i ].z, cz ) << 2 )
               + ( zeroIfLess( atoms[ i ].y, cy ) << 1 )
               + ( zeroIfLess( atoms[ i ].x, cx ) );
         
         atomsCount[ j ]++;
        }
        
      int atomsStartIndex[ 8 ];
      int atomsCurIndex[ 8 ];              
      
      atomsCurIndex[ 0 ] = atomsStartIndex[ 0 ] = atomsStartID;
      for ( int i = 1; i < 8; i++ )
        atomsCurIndex[ i ] = atomsStartIndex[ i ] = atomsStartIndex[ i - 1 ] + atomsCount[ i - 1 ];

      for ( int i = atomsStartID; i <= atomsEndID; i++ )
        {        
         int j = ( zeroIfLess( atomsT[ i ].z, cz ) << 2 )
               + ( zeroIfLess( atomsT[ i ].y, cy ) << 1 )
               + ( zeroIfLess( atomsT[ i ].x, cx ) );
           
         atoms[ atomsCurIndex[ j ] ] = atomsT[ i ];
         atomsCurIndex[ j ]++;  
        }        
        
      for ( int i = 0; i < 8; i++ ) 
        if ( atomsCount[ i ] > 0 )
          {
           int numNodesT = 0;
           
           countAtomsOctreeNodesAndSortAtoms( atoms, atomsStartIndex[ i ], atomsStartIndex[ i ] + atomsCount[ i ] - 1, atomsT, &numNodesT );
         
           *numNodes += numNodesT;
          }
     }  
}



int clashFilter::constructAtomsOctree( int atomsStartID, int atomsEndID, ATOM *atoms, ATOMS_OCTREE_NODE *atomsOctree )
{
   int nodeID = nextFreeNode( );
      
   atomsOctree[ nodeID ].atomsStartID = atomsStartID;
   atomsOctree[ nodeID ].atomsEndID = atomsEndID;
   
   double minX = atoms[ atomsStartID ].x, minY = atoms[ atomsStartID ].y, minZ = atoms[ atomsStartID ].z;
   double maxX = atoms[ atomsStartID ].x, maxY = atoms[ atomsStartID ].y, maxZ = atoms[ atomsStartID ].z, maxR = atoms[ atomsStartID ].r;
   double qSum = 0;
   
   for ( int i = atomsStartID + 1; i <= atomsEndID; i++ )
     {
      if ( atoms[ i ].x < minX ) minX = atoms[ i ].x;      
      if ( atoms[ i ].x > maxX ) maxX = atoms[ i ].x;      
      
      if ( atoms[ i ].y < minY ) minY = atoms[ i ].y;      
      if ( atoms[ i ].y > maxY ) maxY = atoms[ i ].y;      

      if ( atoms[ i ].z < minZ ) minZ = atoms[ i ].z;      
      if ( atoms[ i ].z > maxZ ) maxZ = atoms[ i ].z;      

      if ( atoms[ i ].r > maxR ) maxR = atoms[ i ].r;      
      
      qSum += atoms[ i ].q;
     } 
   
   double cx = atomsOctree[ nodeID ].cx = ( minX + maxX ) / 2;
   double cy = atomsOctree[ nodeID ].cy = ( minY + maxY ) / 2;
   double cz = atomsOctree[ nodeID ].cz = ( minZ + maxZ ) / 2;
   
   double cq = atomsOctree[ nodeID ].cq = qSum;

   double r2 = ( atoms[ atomsStartID ].x - cx ) * ( atoms[ atomsStartID ].x - cx )
             + ( atoms[ atomsStartID ].y - cy ) * ( atoms[ atomsStartID ].y - cy )
             + ( atoms[ atomsStartID ].z - cz ) * ( atoms[ atomsStartID ].z - cz );
   
   for ( int i = atomsStartID + 1; i <= atomsEndID; i++ )
     {
      double r2T = ( atoms[ i ].x - cx ) * ( atoms[ i ].x - cx )
                 + ( atoms[ i ].y - cy ) * ( atoms[ i ].y - cy )
                 + ( atoms[ i ].z - cz ) * ( atoms[ i ].z - cz );
     
      if ( r2T > r2 ) r2 = r2T;
     } 
   
   double cr = atomsOctree[ nodeID ].cr = sqrt( r2 ) + maxR;
         
   if ( ( atomsEndID - atomsStartID +  1 <= maxLeafSize ) || ( cr <= minRadius ) )
      atomsOctree[ nodeID ].leaf = true;
   else
     {
      atomsOctree[ nodeID ].leaf = false;     
      
      int atomsCount[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      
      for ( int i = atomsStartID; i <= atomsEndID; i++ )
        {
         int j = ( zeroIfLess( atoms[ i ].z, cz ) << 2 )
               + ( zeroIfLess( atoms[ i ].y, cy ) << 1 )
               + ( zeroIfLess( atoms[ i ].x, cx ) );
         
         atomsCount[ j ]++;
        }
        
      int atomsStartIndex[ 8 ];

      atomsStartIndex[ 0 ] = atomsStartID;
      for ( int i = 1; i < 8; i++ )
        atomsStartIndex[ i ] = atomsStartIndex[ i - 1 ] + atomsCount[ i - 1 ];

      for ( int i = 0; i < 8; i++ ) 
        if ( atomsCount[ i ] > 0 )
          {
           int j = constructAtomsOctree( atomsStartIndex[ i ], atomsStartIndex[ i ] + atomsCount[ i ] - 1, atoms, atomsOctree );
           atomsOctree[ nodeID ].cPtr[ i ] = j; 
          }
        else atomsOctree[ nodeID ].cPtr[ i ] = -1;           
     }  
     
   return nodeID;  
}


void clashFilter::fillMovingAtomsSubtreeRootsArray( int nodeID, int maxNodesInLevel )
{
   if ( nodeID < 0 ) return;
   
   if ( nodeID == movingAtomsOctreeRoot ) maxSubtreeRoot = 0;
   
   if ( movingAtomsOctree[ nodeID ].leaf || ( maxNodesInLevel >= movingAtomsSubtreeRootsSize ) ) movingAtomsSubtreeRoots[ maxSubtreeRoot++ ] = nodeID;
   else 
     {
      for ( int i = 0; i < 8; i++ ) 
        if ( movingAtomsOctree[ nodeID ].cPtr[ i ] >= 0 )
           fillMovingAtomsSubtreeRootsArray( movingAtomsOctree[ nodeID ].cPtr[ i ], 8 * maxNodesInLevel );
     }
}


bool clashFilter::buildStaticAtomsOctree( void )
{  
   if ( printStatus ) printf( "\nbuilding static atoms octree... " );
   
   double startT = getTime( );
   
   ATOM *atomsT;         
   atomsT = ( ATOM * ) malloc( nStaticAtoms * sizeof( ATOM ) );
   
   if ( atomsT == NULL )
     {
      printError( "Failed to allocate temporary memory for static atoms!" );
      if ( !staticAtomsOctreeBuilt ) exit( 1 );
      return false;
     }

   countAtomsOctreeNodesAndSortAtoms( staticAtoms, 0, nStaticAtoms - 1, atomsT, &numStaticAtomsOctreeNodes );
   freeMem( atomsT );
   
   ATOMS_OCTREE_NODE *atomsOctreeT;
   
   atomsOctreeT = ( ATOMS_OCTREE_NODE * ) malloc( numStaticAtomsOctreeNodes * sizeof( ATOMS_OCTREE_NODE ) );

   if ( atomsOctreeT == NULL )
     {
      printError( "Unable to %s static atoms octree - memory allocation failed!", ( staticAtomsOctreeBuilt ) ? "rebuild" : "build" );
      if ( !staticAtomsOctreeBuilt ) exit( 1 );
      return false;
     }
 
   freeMem( staticAtomsOctree );
   staticAtomsOctree = atomsOctreeT; 

   initFreeNodeServer( nStaticAtoms );
   
   staticAtomsOctreeRoot = constructAtomsOctree( 0, nStaticAtoms - 1, staticAtoms, staticAtomsOctree );

   staticAtomsOctreeBuilt = true;

   double endT = getTime( );
   
   if ( printStatus ) printf( "done ( %lf sec )\n", endT - startT );
   
   return true;
}



bool clashFilter::buildMovingAtomsOctree( void )
{  
   if ( printStatus ) printf( "\nbuilding moving atoms octree... " );
   
   double startT = getTime( );
   
   ATOM *atomsT;         
   atomsT = ( ATOM * ) malloc( nMovingAtoms * sizeof( ATOM ) );
   
   if ( atomsT == NULL )
     {
      printError( "Failed to allocate temporary memory for moving atoms!" );
      if ( !movingAtomsOctreeBuilt ) exit( 1 );
      return false;
     }

   countAtomsOctreeNodesAndSortAtoms( movingAtoms, 0, nMovingAtoms - 1, atomsT, &numMovingAtomsOctreeNodes );
   freeMem( atomsT );
   
   ATOMS_OCTREE_NODE *atomsOctreeT;
   
   atomsOctreeT = ( ATOMS_OCTREE_NODE * ) malloc( numMovingAtomsOctreeNodes * sizeof( ATOMS_OCTREE_NODE ) );

   if ( atomsOctreeT == NULL )
     {
      printError( "Unable to %s moving atoms octree - memory allocation failed!", ( movingAtomsOctreeBuilt ) ? "rebuild" : "build" );
      if ( !movingAtomsOctreeBuilt ) exit( 1 );
      return false;
     }
 
   freeMem( movingAtomsOctree );
   movingAtomsOctree = atomsOctreeT;  

   initFreeNodeServer( nMovingAtoms );
   
   movingAtomsOctreeRoot = constructAtomsOctree( 0, nMovingAtoms - 1, movingAtoms, movingAtomsOctree );
   
   movingAtomsOctreeBuilt = true;
   
   fillMovingAtomsSubtreeRootsArray( movingAtomsOctreeRoot, 1 );   

   double endT = getTime( );
   
   if ( printStatus ) printf( "done ( %lf sec )\n", endT - startT );
   
   return true;
}


bool clashFilter::buildOctrees( void )
{
   if ( ( minRadius != minRadiusUsed ) || ( maxLeafSize != maxLeafSizeUsed ) || ( !staticAtomsOctreeBuilt ) ) buildStaticAtomsOctree( );  
   if ( ( minRadius != minRadiusUsed ) || ( maxLeafSize != maxLeafSizeUsed ) || ( !movingAtomsOctreeBuilt ) ) buildMovingAtomsOctree( );
   minRadiusUsed = minRadius;
   maxLeafSizeUsed = maxLeafSize;
}


inline void clashFilter::transformPoint( double x, double y, double z, double *transMat, double *nx, double *ny, double *nz )
{
   *nx = transMat[  0 ] * x + transMat[  1 ] * y + transMat[  2 ] * z + transMat[  3 ];
   *ny = transMat[  4 ] * x + transMat[  5 ] * y + transMat[  6 ] * z + transMat[  7 ];
   *nz = transMat[  8 ] * x + transMat[  9 ] * y + transMat[ 10 ] * z + transMat[ 11 ];       
}


inline void clashFilter::transformPoint( Point p, double *transMat, Point *np )
{
   np->x = transMat[  0 ] * p.x + transMat[  1 ] * p.y + transMat[  2 ] * p.z + transMat[  3 ];
   np->y = transMat[  4 ] * p.x + transMat[  5 ] * p.y + transMat[  6 ] * p.z + transMat[  7 ];
   np->z = transMat[  8 ] * p.x + transMat[  9 ] * p.y + transMat[ 10 ] * p.z + transMat[ 11 ];       
}


void clashFilter::approximateInteractions( double *trans, int nodeS, int nodeM, int *nClashes, int *nSevereClashes, double *interactionValue )
{
   double sumRad = staticAtomsOctree[ nodeS ].cr + movingAtomsOctree[ nodeM ].cr;
   double sumRad2 = sumRad * sumRad;   
   
   Point P, Q;
   
   P.x = movingAtomsOctree[ nodeM ].cx;
   P.y = movingAtomsOctree[ nodeM ].cy;
   P.z = movingAtomsOctree[ nodeM ].cz;
   
   transformPoint( P, trans, &Q );
   
   double dx = staticAtomsOctree[ nodeS ].cx - Q.x,
          dy = staticAtomsOctree[ nodeS ].cy - Q.y,
          dz = staticAtomsOctree[ nodeS ].cz - Q.z;
   double d2 = dx * dx + dy * dy + dz * dz;

   
   bool farEnough = false;
   
   if ( ( d2 > 4 * sumRad2 ) && ( d2 > ( sumRad2 / ( epsilon * epsilon ) ) ) ) farEnough = true;
   
   *nClashes = *nSevereClashes = 0;
   *interactionValue = 0;
   
   if ( farEnough ) 
      {
        double qiqj = staticAtomsOctree[ nodeS ].cq * movingAtomsOctree[ nodeM ].cq;
        *interactionValue = qiqj / d2; 
      }  
   else
      {
       if ( staticAtomsOctree[ nodeS ].leaf && movingAtomsOctree[ nodeM ].leaf )
         {            
           for ( int i = movingAtomsOctree[ nodeM ].atomsStartID; i <= movingAtomsOctree[ nodeM ].atomsEndID; i++ )
             {
		Point oldP, newP;
		oldP.x = movingAtoms[ i ].x;
		oldP.y = movingAtoms[ i ].y;
		oldP.z = movingAtoms[ i ].z;

		transformPoint( oldP, trans, &newP );
       
	      int id = movingAtoms[ i ].id;
              double xM = newP.x , yM = newP.y , zM = newP.z;      
              
              bool clash = false, severeClash = false;
             
              for ( int j = staticAtomsOctree[ nodeS ].atomsStartID; j <= staticAtomsOctree[ nodeS ].atomsEndID; j++ )
                {                            
                 dx = staticAtoms[ j ].x - xM;
                 dy = staticAtoms[ j ].y - yM;
                 dz = staticAtoms[ j ].z - zM;                     
                                                
                 d2 = dx * dx + dy * dy + dz * dz;

		 if(d2<distCutoff) movingAtomIds.push_back(id);

                 double rSum = movingAtoms[ i ].r + staticAtoms[ j ].r;
                 double qiqj = movingAtoms[ i ].q * staticAtoms[ j ].q;
         
                 if ( d2 <= ( clashFrac * rSum ) * ( clashFrac * rSum ) ) clash = true; //( *nClashes )++;
                 if ( d2 <= ( severeClashFrac * rSum ) * ( severeClashFrac * rSum ) ) severeClash = true; //( *nSevereClashes )++;
         
                 if ( d2 < ( fuzzyFrac * rSum ) * ( fuzzyFrac * rSum ) ) d2 = ( fuzzyFrac * rSum ) * ( fuzzyFrac * rSum );     
                
                 ( *interactionValue ) += ( qiqj / d2 );                         
                }                 
                
              if ( clash ) ( *nClashes )++;
              if ( severeClash ) ( *nSevereClashes )++;
             }  
		
	  for ( int i = staticAtomsOctree[ nodeM ].atomsStartID; i <= staticAtomsOctree[ nodeM ].atomsEndID; i++ )
             {
		Point oldP, newP;
		oldP.x = staticAtoms[ i ].x;
		oldP.y = staticAtoms[ i ].y;
		oldP.z = staticAtoms[ i ].z;

		transformPoint( oldP, trans, &newP );
       
              double xM = newP.x , yM = newP.y , zM = newP.z;

	      int id = staticAtoms[ i ].id;

              
              bool clash = false, severeClash = false;
             
              for ( int j = staticAtomsOctree[ nodeS ].atomsStartID; j <= staticAtomsOctree[ nodeS ].atomsEndID; j++ )
                {                            
                 dx = movingAtoms[ j ].x - xM;
                 dy = movingAtoms[ j ].y - yM;
                 dz = movingAtoms[ j ].z - zM;                     
                                                
                 d2 = dx * dx + dy * dy + dz * dz;

		 if(d2<distCutoff) staticAtomIds.push_back(id);
                }                 
             } 
         }
       else if ( !staticAtomsOctree[ nodeS ].leaf && !movingAtomsOctree[ nodeM ].leaf )         
              {
                for ( int i = 0; i < 8; i++ )
                  if ( staticAtomsOctree[ nodeS ].cPtr[ i ] >= 0 )
                     for ( int j = 0; j < 8; j++ )
                       if ( movingAtomsOctree[ nodeM ].cPtr[ j ] >= 0 ) 
                         {
                           int nC, nSC;
                           double intVal;
                           
                           approximateInteractions( trans, staticAtomsOctree[ nodeS ].cPtr[ i ], movingAtomsOctree[ nodeM ].cPtr[ j ], &nC, &nSC, &intVal );
                           
                           ( *nClashes ) += nC;
                           ( *nSevereClashes ) += nSC;
                           ( *interactionValue ) += intVal;                         
                         }  
              }
            else if ( !staticAtomsOctree[ nodeS ].leaf )         
                   {
                     for ( int i = 0; i < 8; i++ )
                       if ( staticAtomsOctree[ nodeS ].cPtr[ i ] >= 0 )
                         {
                           int nC, nSC;
                           double intVal;
                           
                           approximateInteractions( trans, staticAtomsOctree[ nodeS ].cPtr[ i ], nodeM, &nC, &nSC, &intVal );
                           
                           ( *nClashes ) += nC;
                           ( *nSevereClashes ) += nSC;
                           ( *interactionValue ) += intVal;
                         }  
                   }
                 else 
                   {
                     for ( int j = 0; j < 8; j++ )
                       if ( movingAtomsOctree[ nodeM ].cPtr[ j ] >= 0 )
                         {
                           int nC, nSC;
                           double intVal;
                           
                           approximateInteractions( trans, nodeS, movingAtomsOctree[ nodeM ].cPtr[ j ], &nC, &nSC, &intVal );
                           
                           ( *nClashes ) += nC;
                           ( *nSevereClashes ) += nSC;
                           ( *interactionValue ) += intVal;
                         }  
                   }              
      }      
}



bool clashFilter::computeInteractions( double *trans, int *nClashes, int *nSevereClashes, double *interactionValue )
{
   if ( !staticAtomsOctreeBuilt || !movingAtomsOctreeBuilt ) return false;

   if ( printStatus ) printf( "\napproximating interactions and computing clashes... " );

   double startT = getTime( );
   
   if ( numThreads == 1 ) approximateInteractions( trans, staticAtomsOctreeRoot, movingAtomsOctreeRoot, nClashes, nSevereClashes, interactionValue );
   else
      {
       initSubtreeRootServer( );
       initFreeNodeServer( numThreads );      
             
       pthread_t p[ numThreads ];         
       THREAD_RESULT threadResults[ numThreads ]; 
       
       for ( int i = 0; i < numThreads; i++ )
         {
           threadResults[ i ].cF = this;
           threadResults[ i ].trans = trans;
           pthread_create( &p[ i ], NULL, approximateInteractionsThread, ( void * ) &threadResults[ i ] );
         }  
          
       for ( int i = 0; i < numThreads; i++ )          
          pthread_join( p[ i ], NULL );          
          
       *nClashes = *nSevereClashes = 0;
       *interactionValue = 0;
       
       for ( int i = 0; i < numThreads; i++ )          
         {
           ( *nClashes ) += threadResults[ i ].nClashes;
           ( *nSevereClashes ) += threadResults[ i ].nSevereClashes;           
           ( *interactionValue ) += threadResults[ i ].interactionValue;           
         }  
      }
         
   double endT = getTime( );
   
   if ( printStatus ) printf( "done ( %lf sec )\n", endT - startT );
   
   return true;
}



bool clashFilter::computeInteractions( int *nClashes, int *nSevereClashes, double *interactionValue )
{
   return computeInteractions( trans, nClashes, nSevereClashes, interactionValue );
}


bool clashFilter::computeInteractionsNaively( double *trans, int *nClashes, int *nSevereClashes, double *interactionValue )
{
   if ( printStatus ) printf( "\ncomputing steric interactions naively... " );

   double startT = getTime( );
   
   *nClashes = *nSevereClashes = 0;
   *interactionValue = 0.0; 
   
//   printf( "\nnStaticAtoms = %d\n", nStaticAtoms );
//
//   for ( int i = 0; i < nStaticAtoms; i++ )
//     printf( "\n< %lf, %lf, %lf, %lf, %lf >\n", staticAtoms[ i ].x, staticAtoms[ i ].y, staticAtoms[ i ].z, staticAtoms[ i ].q, staticAtoms[ i ].r );
//   
//   printf( "\nnMovingAtoms = %d\n", nMovingAtoms );
//
//   for ( int i = 0; i < nMovingAtoms; i++ )
//     printf( "\n< %lf, %lf, %lf, %lf, %lf >\n", movingAtoms[ i ].x, movingAtoms[ i ].y, movingAtoms[ i ].z, movingAtoms[ i ].q, movingAtoms[ i ].r );
//   
//   printf( "\n\n" );

   double frac = 1.5;
   double step = 1.5;
   
   int numClose = 0;
   for ( int i = 0; i < nMovingAtoms; i++ )
     {
	Point oldP, newP;
	oldP.x = movingAtoms[ i ].x;
	oldP.y = movingAtoms[ i ].y;
	oldP.z = movingAtoms[ i ].z;

	transformPoint( oldP, trans, &newP );
       
        double xM = newP.x , yM = newP.y , zM = newP.z;
     
      int k = 0;
      
      for ( int j = 0; j < nStaticAtoms; j++ )
        {                            
         double dx = staticAtoms[ j ].x - xM;
         double dy = staticAtoms[ j ].y - yM;
         double dz = staticAtoms[ j ].z - zM;                     
                
         double d2 = dx * dx + dy * dy + dz * dz;
         double rSum = movingAtoms[ i ].r + staticAtoms[ j ].r;
         double qiqj = movingAtoms[ i ].q * staticAtoms[ j ].q;
         
         if ( d2 <= ( clashFrac * rSum ) * ( clashFrac * rSum ) ) ( *nClashes )++;
         if ( d2 <= ( severeClashFrac * rSum ) * ( severeClashFrac * rSum ) ) ( *nSevereClashes )++;
         
//         if ( d2 < ( fuzzyFrac * rSum ) * ( fuzzyFrac * rSum ) ) d2 = ( fuzzyFrac * rSum ) * ( fuzzyFrac * rSum );     

         double minD2 = ( frac * rSum ) * ( frac * rSum );

         if ( d2 < ( 1 / ( frac * frac ) ) * minD2 ) k++;
         
         if ( d2 < minD2 ) d2 = minD2; 
         
//         d2 = ceil( d2 / ( step * step ) );
//         d2 = ( step * step ) * d2;
               
         ( *interactionValue ) += ( qiqj / d2 );       
//         ( *interactionValue ) += ( minD2 / d2 );// ( ( minD2 * minD2 * minD2 ) / ( d2 * d2 * d2 ) );       
        }           
        
      if ( k > 0 ) numClose++;                
     }   
     
//  ( *interactionValue ) /= numClose;  

   double endT = getTime( );
   
   if ( printStatus ) printf( "done ( %lf sec )\n", endT - startT );
   
   return true;
}


bool clashFilter::computeInteractionsNaively( int *nClashes, int *nSevereClashes, double *interactionValue )
{
   return computeInteractionsNaively( trans, nClashes, nSevereClashes, interactionValue );
}


void clashFilter::getInterfaceAtoms( double *trans, char *staticFile, char *movingFile)
{
  int nClashes;
  int nSevereClashes;
  double interactionValue;

  computeInteractions( trans, &nClashes, &nSevereClashes, &interactionValue );

  FILE *fp = fopen( staticFile, "wt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open output file (%s)!", staticFile );
      return;
     }

   int size = staticAtomIds.size();

   for(int i=0;i<size;i++)
	fprintf(fp, "%d\n", staticAtomIds[i]);

   fclose(fp);

  fp = fopen( movingFile, "wt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open output file (%s)!", movingFile );
      return;
     }

   size = movingAtomIds.size();

   for(int i=0;i<size;i++)
	fprintf(fp, "%d\n", movingAtomIds[i]);

   fclose(fp);
}



bool clashFilter::readAtoms( char *atomsFile, int *nAtoms, double **atoms )
{
   FILE *fp;
   
   fp = fopen( atomsFile, "rt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open atoms file (%s)!", atomsFile );
      return false;
     }
   
   *nAtoms = 0;
   double x, y, z, r, h;
          
   while ( fscanf( fp, "%lf %lf %lf %lf", &x, &y, &z, &r ) == 4 ) ( *nAtoms )++; 

   fclose( fp );  
   
   ( *atoms ) = ( double * ) malloc( 4 * ( *nAtoms ) * sizeof( double ) );
   
   if ( *atoms == NULL )
     {
      printError( "Failed to allocate memory for atoms!" );
      return false;
     }
     
   fp = fopen( atomsFile, "rt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open atoms file (%s)!", atomsFile );
      return false;
     }

   for ( int i = 0; i < *nAtoms; i++ )   
     {          
      if ( fscanf( fp, "%lf %lf %lf %lf", &x, &y, &z, &r ) != 4 )
        {
         printError( "Failed to read the atoms file (%s)!", atomsFile );
         return false;
        }
      
      ( *atoms )[ 4 * i + 0 ] = x;         
      ( *atoms )[ 4 * i + 1 ] = y;         
      ( *atoms )[ 4 * i + 2 ] = z;         
      ( *atoms )[ 4 * i + 3 ] = r;                                      
     }    
     
   fclose( fp );

         
   return true;
}
