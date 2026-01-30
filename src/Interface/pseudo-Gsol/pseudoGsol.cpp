#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "pseudoGsol.h"

using namespace std;
using namespace DPG;

void pseudoGsol::freeMemory( void )
{
   freeMem( staticAtoms );
   freeMem( movingAtoms );   

   freeMem( staticQPoints );
   freeMem( movingQPoints );  

   freeMem( staticQPointsPG );
   freeMem( movingQPointsPG );  
   
   freeMem( staticAtomsOctree );
   freeMem( movingAtomsOctree );

   freeMem( staticQPointsOctree );
   freeMem( movingQPointsOctree );
   
   freeMem( staticQPointsOctreeFlags );
   freeMem( movingQPointsOctreeFlags );   
   
   delete staticPG;
   delete movingPG;   
}


pseudoGsol::pseudoGsol( char *staticRawFile, char *movingRawFile, char *staticMolFile, char *movingMolFile, double dist, int nThreads )
{
  if ( nThreads <= 0 )
    {
      printError( "Invalid number of threads ( %d )!", numThreads );  
      exit( 1 );                     
    }
    
  numThreads = nThreads;  
    
//  if ( !getParamsFromFile( &params, paramFile ) ) exit( 1 );

  params.staticMoleculeQUAD = staticRawFile;
  params.movingMoleculeQUAD = movingRawFile;
  params.distanceCutoff = dist;

  minRadius = 4.0;
  maxLeafSize = 500;

  numStaticAtoms = numMovingAtoms = 0;  
  staticAtoms = movingAtoms = NULL;

  numStaticQPoints = numMovingQPoints = 0;  
  staticQPoints = movingQPoints = NULL;

  staticQPointsPG = movingQPointsPG = NULL;

  numStaticAtomsOctreeNodes = numMovingAtomsOctreeNodes = 0;
  staticAtomsOctree = movingAtomsOctree = NULL;
  staticAtomsOctreeRoot = movingAtomsOctreeRoot = -1;

  numStaticQPointsOctreeNodes = numMovingQPointsOctreeNodes = 0;
  staticQPointsOctree = movingQPointsOctree = NULL;
  staticQPointsOctreeRoot = movingQPointsOctreeRoot = -1;
  
  staticQPointsOctreeFlags = movingQPointsOctreeFlags = NULL;

  int nStAtoms, nMvAtoms;  
  double *stAtoms, *mvAtoms;  

cout<<"reading atoms"<<endl;
    
  if ( !readAtoms( staticMolFile, &nStAtoms, &stAtoms )
    || !readAtoms( movingMolFile, &nMvAtoms, &mvAtoms ) )  
     exit( 1 );


  if ( !copyAtomsFromArray( nStAtoms, stAtoms, &numStaticAtoms, &staticAtoms ) 
    || !copyAtomsFromArray( nMvAtoms, mvAtoms, &numMovingAtoms, &movingAtoms ) 
    || !readQPoints( params.staticMoleculeQUAD, &numStaticQPoints, &staticQPoints )
    || !readQPoints( params.movingMoleculeQUAD, &numMovingQPoints, &movingQPoints ) )
      {
        exit( 1 );
      }  

cout<<"copied atoms"<<endl;

  staticQPointsPG = ( Point * ) malloc( numStaticQPoints * sizeof( Point ) );
  movingQPointsPG = ( Point * ) malloc( numMovingQPoints * sizeof( Point ) );
               
  if ( ( staticQPointsPG == NULL ) || ( movingQPointsPG == NULL ) )
     {
       printError( "Failed to allocate memory!" );  
       exit( 1 );                 
     }
                 
  double xlatePG = computeXlateForPG( numStaticQPoints, staticQPoints, numMovingQPoints, movingQPoints );

  staticPG = new PG( 10.0, xlatePG, 5.0 );  

  for ( int i = 0; i < numStaticQPoints; i++ )
     {
       staticQPointsPG[ i ].setX(staticQPoints[ i ].x);  
       staticQPointsPG[ i ].setY(staticQPoints[ i ].y);  
       staticQPointsPG[ i ].setZ(staticQPoints[ i ].z);  
                   
       staticPG->addPoint( &staticQPointsPG[ i ] );
     }    

  xlatePG = computeXlateForPG( numMovingQPoints, movingQPoints, numStaticQPoints, staticQPoints );

  movingPG = new PG( 10.0, xlatePG, 5.0 );  
     
  for ( int i = 0; i < numMovingQPoints; i++ )
     {
       movingQPointsPG[ i ].setX(movingQPoints[ i ].x);  
       movingQPointsPG[ i ].setY(movingQPoints[ i ].y);  
       movingQPointsPG[ i ].setZ(movingQPoints[ i ].z);  
       
       movingPG->addPoint( &movingQPointsPG[ i ] );
     }       
     
  if ( !buildAtomsOctree( numStaticAtoms, staticAtoms, &numStaticAtomsOctreeNodes, &staticAtomsOctree, &staticAtomsOctreeRoot ) 
    || !buildAtomsOctree( numMovingAtoms, movingAtoms, &numMovingAtomsOctreeNodes, &movingAtomsOctree, &movingAtomsOctreeRoot )   
    || !buildQPointsOctree( numStaticQPoints, staticQPoints, &numStaticQPointsOctreeNodes, &staticQPointsOctree, &staticQPointsOctreeRoot ) 
    || !buildQPointsOctree( numMovingQPoints, movingQPoints, &numMovingQPointsOctreeNodes, &movingQPointsOctree, &movingQPointsOctreeRoot ) )
      {
        exit( 1 );
      }  
       
  assignHydrophobicityToQPoints( staticAtomsOctree, staticAtomsOctreeRoot, staticAtoms,  
                                 staticQPointsOctree, staticQPointsOctreeRoot, staticQPoints, 4, 0.1 );       
  assignHydrophobicityToQPoints( movingAtomsOctree, movingAtomsOctreeRoot, movingAtoms,
                                 movingQPointsOctree, movingQPointsOctreeRoot, movingQPoints, 4, 0.1 );         
                                 
  staticQPointsOctreeFlags = ( bool * ) malloc( 2 * nThreads * numStaticQPointsOctreeNodes * sizeof( bool ) );
  movingQPointsOctreeFlags = ( bool * ) malloc( 2 * nThreads * numMovingQPointsOctreeNodes * sizeof( bool ) );
               
  if ( ( staticQPointsOctreeFlags == NULL ) || ( movingQPointsOctreeFlags == NULL ) )
     {
       printError( "Failed to allocate memory!" );  
       exit( 1 );                 
     }                                 
}


pseudoGsol::~pseudoGsol( )
{
  freeMemory( );
}


bool pseudoGsol::readQPoints( char *qPtsFile, int *nQPoints, QPOINT **qPoints )
{
   FILE *fp;
   
   fp = fopen( qPtsFile, "rt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open surface points file (%s)!", qPtsFile );
      return false;
     }
   

     
   double nx, ny, nz;

   int nPoints;
          
   int returnVal; // arand... add this to remove compiler warnings...
   returnVal = fscanf( fp, "%d", &nPoints);
   returnVal = fscanf( fp, "%d", nQPoints);

cout<< nPoints << " "<< *nQPoints <<endl;
   
   ATOM *tempPoints = ( ATOM * ) malloc( ( nPoints ) * sizeof( ATOM ) );

   if ( tempPoints == NULL )
     {
      printError( "Failed to allocate memory for points!" );
      return false;
     }
  
   ATOM at;
   for ( int i = 0; i < nPoints; i++ )   
     {          
      if ( fscanf( fp, "%lf %lf %lf %lf %lf %lf", &at.x, &at.y, &at.z, &nx, &ny, &nz ) != 6 )
        {
         printError( "Failed to read the quadrature points file (%s)!", qPtsFile );
         return false;
        }    
      
       tempPoints[i] = at;         
     }    

   ( *qPoints ) = ( QPOINT * ) malloc( ( *nQPoints ) * sizeof( QPOINT ) );
   
   if ( *qPoints == NULL )
     {
      printError( "Failed to allocate memory for quadrature points!" );
      return false;
     }     

   int trPt1, trPt2, trPt3;
   QPOINT qPt;

   for ( int i = 0; i < *nQPoints; i++ )   
     {          
      if ( fscanf( fp, "%d %d %d", &trPt1, &trPt2, &trPt3 ) != 3 )
        {
         printError( "Failed to read the quadrature points file (%s)!", qPtsFile );
         return false;
        }

	qPt.x = (tempPoints[trPt1].x + tempPoints[trPt2].x + tempPoints[trPt3].x)/3.0;
	qPt.y = (tempPoints[trPt1].y + tempPoints[trPt2].y + tempPoints[trPt3].y)/3.0;
	qPt.z = (tempPoints[trPt1].z + tempPoints[trPt2].z + tempPoints[trPt3].z)/3.0;
	     
      ( *qPoints )[ i ] = qPt;         
     }    

   fclose( fp );

   return true;
}


bool pseudoGsol::copyAtomsFromArray( int numAtoms, double *atms, int *nAtoms, ATOM **atoms )
{
   if ( numAtoms <= 0 )
     {
      printError( "No atoms to copy!" );
      return false;
     }
   
   *nAtoms = numAtoms;
   ( *atoms ) = ( ATOM * ) malloc( ( *nAtoms ) * sizeof( ATOM ) );
   
   if ( *atoms == NULL )
     {
      printError( "Failed to allocate memory for atoms!" );
      return false;
     }

   for ( int i = 0; i < *nAtoms; i++ )   
     {          
      ( *atoms )[ i ].x = atms[ 4 * i + 0 ];
      ( *atoms )[ i ].y = atms[ 4 * i + 1 ];
      ( *atoms )[ i ].z = atms[ 4 * i + 2 ];
      ( *atoms )[ i ].r = atms[ 4 * i + 3 ];      

      ( *atoms )[ i ].id = i + 1;
     }      
        
   return true;
}


void pseudoGsol::initFreeNodeServer( int numNodes )
{
   pthread_mutex_init( &nodesLock, NULL );
   curNode = 0;
   maxNode = numNodes;
}


int pseudoGsol::nextFreeNode( void )
{
   int nextNode = -1;
   
   pthread_mutex_lock( &nodesLock );
   if ( curNode < maxNode ) nextNode = curNode++;
   pthread_mutex_unlock( &nodesLock );
   
   return nextNode;
}


void pseudoGsol::countAtomsOctreeNodesAndSortAtoms( ATOM *atoms, int atomsStartID, int atomsEndID, 
                                                    ATOM *atomsT, int *numNodes )
{
   double minX = atoms[ atomsStartID ].x, minY = atoms[ atomsStartID ].y, minZ = atoms[ atomsStartID ].z;
   double maxX = atoms[ atomsStartID ].x, maxY = atoms[ atomsStartID ].y, maxZ = atoms[ atomsStartID ].z;
   
   for ( int i = atomsStartID + 1; i <= atomsEndID; i++ )
     {
      if ( atoms[ i ].x < minX ) minX = atoms[ i ].x;      
      if ( atoms[ i ].x > maxX ) maxX = atoms[ i ].x;      
      
      if ( atoms[ i ].y < minY ) minY = atoms[ i ].y;      
      if ( atoms[ i ].y > maxY ) maxY = atoms[ i ].y;      

      if ( atoms[ i ].z < minZ ) minZ = atoms[ i ].z;      
      if ( atoms[ i ].z > maxZ ) maxZ = atoms[ i ].z;      
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
   
   *numNodes = 1;
      
   if ( ( atomsEndID - atomsStartID +  1 > maxLeafSize ) && ( r2 > minRadius * minRadius ) )
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



int pseudoGsol::constructAtomsOctree( int atomsStartID, int atomsEndID, ATOM *atoms, ATOMS_OCTREE_NODE *atomsOctree )
{
   int nodeID = nextFreeNode( );
   
   atomsOctree[ nodeID ].atomsStartID = atomsStartID;
   atomsOctree[ nodeID ].atomsEndID = atomsEndID;
   
   double minX = atoms[ atomsStartID ].x, minY = atoms[ atomsStartID ].y, minZ = atoms[ atomsStartID ].z;
   double maxX = atoms[ atomsStartID ].x, maxY = atoms[ atomsStartID ].y, maxZ = atoms[ atomsStartID ].z;
   
   for ( int i = atomsStartID + 1; i <= atomsEndID; i++ )
     {
      if ( atoms[ i ].x < minX ) minX = atoms[ i ].x;      
      if ( atoms[ i ].x > maxX ) maxX = atoms[ i ].x;      
      
      if ( atoms[ i ].y < minY ) minY = atoms[ i ].y;      
      if ( atoms[ i ].y > maxY ) maxY = atoms[ i ].y;      

      if ( atoms[ i ].z < minZ ) minZ = atoms[ i ].z;      
      if ( atoms[ i ].z > maxZ ) maxZ = atoms[ i ].z;      
     } 
   
   double cx = atomsOctree[ nodeID ].cx = ( minX + maxX ) / 2;
   double cy = atomsOctree[ nodeID ].cy = ( minY + maxY ) / 2;
   double cz = atomsOctree[ nodeID ].cz = ( minZ + maxZ ) / 2;

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
   
   double cr = atomsOctree[ nodeID ].cr = sqrt( r2 );
         
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


bool pseudoGsol::buildAtomsOctree( int nAtoms, ATOM *atoms, int *numAtomsOctreeNodes, ATOMS_OCTREE_NODE **atomsOctree, int *atomsOctreeRoot )
{  
   ATOM *atomsT;         
   atomsT = ( ATOM * ) malloc( nAtoms * sizeof( ATOM ) );
   
   if ( atomsT == NULL )
     {
      printError( "Failed to allocate temporary memory for atoms!" );
      return false;
     }

   countAtomsOctreeNodesAndSortAtoms( atoms, 0, nAtoms - 1, atomsT, numAtomsOctreeNodes );

   freeMem( atomsT );
   
   ( *atomsOctree ) = ( ATOMS_OCTREE_NODE * ) malloc( ( *numAtomsOctreeNodes ) * sizeof( ATOMS_OCTREE_NODE ) );

   if ( *atomsOctree == NULL )
     {
      printError( "Unable to build atoms octree - memory allocation failed!" );
      return false;
     }
 
   initFreeNodeServer( nAtoms );
   
   *atomsOctreeRoot = constructAtomsOctree( 0, nAtoms - 1, atoms, *atomsOctree );
   
   return true;
}



void pseudoGsol::countQPointsOctreeNodesAndSortQPoints( QPOINT *qPts, int qPtsStartID, int qPtsEndID, 
                                                        QPOINT *qPtsT, int *numNodes )
{
   double minX = qPts[ qPtsStartID ].x, minY = qPts[ qPtsStartID ].y, minZ = qPts[ qPtsStartID ].z;
   double maxX = qPts[ qPtsStartID ].x, maxY = qPts[ qPtsStartID ].y, maxZ = qPts[ qPtsStartID ].z;
   
   for ( int i = qPtsStartID + 1; i <= qPtsEndID; i++ )
     {
      if ( qPts[ i ].x < minX ) minX = qPts[ i ].x;      
      if ( qPts[ i ].x > maxX ) maxX = qPts[ i ].x;      
      
      if ( qPts[ i ].y < minY ) minY = qPts[ i ].y;      
      if ( qPts[ i ].y > maxY ) maxY = qPts[ i ].y;      

      if ( qPts[ i ].z < minZ ) minZ = qPts[ i ].z;      
      if ( qPts[ i ].z > maxZ ) maxZ = qPts[ i ].z;      
     } 
   
   double cx = ( minX + maxX ) / 2,
          cy = ( minY + maxY ) / 2,
          cz = ( minZ + maxZ ) / 2;

   double r2 = ( qPts[ qPtsStartID ].x - cx ) * ( qPts[ qPtsStartID ].x - cx )
             + ( qPts[ qPtsStartID ].y - cy ) * ( qPts[ qPtsStartID ].y - cy )
             + ( qPts[ qPtsStartID ].z - cz ) * ( qPts[ qPtsStartID ].z - cz );
   
   for ( int i = qPtsStartID + 1; i <= qPtsEndID; i++ )
     {
      double r2T = ( qPts[ i ].x - cx ) * ( qPts[ i ].x - cx )
                 + ( qPts[ i ].y - cy ) * ( qPts[ i ].y - cy )
                 + ( qPts[ i ].z - cz ) * ( qPts[ i ].z - cz );
     
      if ( r2T > r2 ) r2 = r2T;
     } 
   
   *numNodes = 1;
      
   if ( ( qPtsEndID - qPtsStartID + 1 > maxLeafSize ) && ( r2 > minRadius * minRadius ) )
     {
      int qPtsCount[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      
      for ( int i = qPtsStartID; i <= qPtsEndID; i++ )
        {
         qPtsT[ i ] = qPts[ i ];
         
         int j = ( zeroIfLess( qPts[ i ].z, cz ) << 2 )
               + ( zeroIfLess( qPts[ i ].y, cy ) << 1 )
               + ( zeroIfLess( qPts[ i ].x, cx ) );
         
         qPtsCount[ j ]++;
        }
      
      int qPtsStartIndex[ 8 ];
      int qPtsCurIndex[ 8 ];              
      
      qPtsCurIndex[ 0 ] = qPtsStartIndex[ 0 ] = qPtsStartID;
      for ( int i = 1; i < 8; i++ )
        qPtsCurIndex[ i ] = qPtsStartIndex[ i ] = qPtsStartIndex[ i - 1 ] + qPtsCount[ i - 1 ];

      for ( int i = qPtsStartID; i <= qPtsEndID; i++ )
        {        
         int j = ( zeroIfLess( qPtsT[ i ].z, cz ) << 2 )
               + ( zeroIfLess( qPtsT[ i ].y, cy ) << 1 )
               + ( zeroIfLess( qPtsT[ i ].x, cx ) );
           
         qPts[ qPtsCurIndex[ j ] ] = qPtsT[ i ];
         qPtsCurIndex[ j ]++;  
        }        
        
      for ( int i = 0; i < 8; i++ ) 
        if ( qPtsCount[ i ] > 0 )
          {
           int numNodesT = 0;
           
           countQPointsOctreeNodesAndSortQPoints( qPts, qPtsStartIndex[ i ], qPtsStartIndex[ i ] + qPtsCount[ i ] - 1, qPtsT, &numNodesT );
         
           *numNodes += numNodesT;
          }
     }  
}



int pseudoGsol::constructQPointsOctree( int qPtsStartID, int qPtsEndID, QPOINT *qPts, QPOINTS_OCTREE_NODE *qPointsOctree )
{
   int nodeID = nextFreeNode( );
   
   qPointsOctree[ nodeID ].qPtsStartID = qPtsStartID;
   qPointsOctree[ nodeID ].qPtsEndID = qPtsEndID;

   double minX = qPts[ qPtsStartID ].x, minY = qPts[ qPtsStartID ].y, minZ = qPts[ qPtsStartID ].z;
   double maxX = qPts[ qPtsStartID ].x, maxY = qPts[ qPtsStartID ].y, maxZ = qPts[ qPtsStartID ].z;
   
   for ( int i = qPtsStartID + 1; i <= qPtsEndID; i++ )
     {
      if ( qPts[ i ].x < minX ) minX = qPts[ i ].x;      
      if ( qPts[ i ].x > maxX ) maxX = qPts[ i ].x;      
      
      if ( qPts[ i ].y < minY ) minY = qPts[ i ].y;      
      if ( qPts[ i ].y > maxY ) maxY = qPts[ i ].y;      

      if ( qPts[ i ].z < minZ ) minZ = qPts[ i ].z;      
      if ( qPts[ i ].z > maxZ ) maxZ = qPts[ i ].z;      
     } 
   
   double cx = qPointsOctree[ nodeID ].cx = ( minX + maxX ) / 2;
   double cy = qPointsOctree[ nodeID ].cy = ( minY + maxY ) / 2;
   double cz = qPointsOctree[ nodeID ].cz = ( minZ + maxZ ) / 2;

   double r2 = ( qPts[ qPtsStartID ].x - cx ) * ( qPts[ qPtsStartID ].x - cx )
             + ( qPts[ qPtsStartID ].y - cy ) * ( qPts[ qPtsStartID ].y - cy )
             + ( qPts[ qPtsStartID ].z - cz ) * ( qPts[ qPtsStartID ].z - cz );
   
   for ( int i = qPtsStartID + 1; i <= qPtsEndID; i++ )
     {
      double r2T = ( qPts[ i ].x - cx ) * ( qPts[ i ].x - cx )
                 + ( qPts[ i ].y - cy ) * ( qPts[ i ].y - cy )
                 + ( qPts[ i ].z - cz ) * ( qPts[ i ].z - cz );
     
      if ( r2T > r2 ) r2 = r2T;
     } 
   
   double cr = qPointsOctree[ nodeID ].cr = sqrt( r2 );
         
   if ( ( qPtsEndID - qPtsStartID +  1 > maxLeafSize ) && ( cr > minRadius ) )   
     {
      qPointsOctree[ nodeID ].leaf = false;     
      
      int qPtsCount[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      
      for ( int i = qPtsStartID; i <= qPtsEndID; i++ )
        {
         int j = ( zeroIfLess( qPts[ i ].z, cz ) << 2 )
               + ( zeroIfLess( qPts[ i ].y, cy ) << 1 )
               + ( zeroIfLess( qPts[ i ].x, cx ) );
         
         qPtsCount[ j ]++;
        }

      int qPtsStartIndex[ 8 ];

      qPtsStartIndex[ 0 ] = qPtsStartID;
      for ( int i = 1; i < 8; i++ )
        qPtsStartIndex[ i ] = qPtsStartIndex[ i - 1 ] + qPtsCount[ i - 1 ];

      for ( int i = 0; i < 8; i++ ) 
        if ( qPtsCount[ i ] > 0 )
          {
           int j = constructQPointsOctree( qPtsStartIndex[ i ], qPtsStartIndex[ i ] + qPtsCount[ i ] - 1, qPts, qPointsOctree );

           qPointsOctree[ nodeID ].cPtr[ i ] = j; 
          }
        else qPointsOctree[ nodeID ].cPtr[ i ] = -1;           
     }  
   else qPointsOctree[ nodeID ].leaf = true;
     
   return nodeID;  
}



bool pseudoGsol::buildQPointsOctree( int nQPoints, QPOINT *qPoints, int *numQPointsOctreeNodes, QPOINTS_OCTREE_NODE **qPointsOctree, int *qPointsOctreeRoot )
{  
   QPOINT *qPointsT;         
   qPointsT = ( QPOINT * ) malloc( nQPoints * sizeof( QPOINT ) );
   
   if ( qPointsT == NULL )
     {
      printError( "Failed to allocate temporary memory for quadrature points!" );
      return false;
     }

   countQPointsOctreeNodesAndSortQPoints( qPoints, 0, nQPoints - 1, qPointsT, numQPointsOctreeNodes );

   freeMem( qPointsT );
   
   ( *qPointsOctree ) = ( QPOINTS_OCTREE_NODE * ) malloc( ( *numQPointsOctreeNodes ) * sizeof( QPOINTS_OCTREE_NODE ) );
   
   if ( *qPointsOctree == NULL )
     {
      printError( "Unable to build quadrature points octree - memory allocation failed!" );
      return false;
     }
 
   initFreeNodeServer( nQPoints );
   
   *qPointsOctreeRoot = constructQPointsOctree( 0, nQPoints - 1, qPoints, *qPointsOctree );
   
   return true;
}



void pseudoGsol::assignHydrophobicityToQPoints( ATOMS_OCTREE_NODE *atomsOctree, int nodeA, ATOM *atoms,
                                                QPOINTS_OCTREE_NODE *qPointsOctree, int nodeQ, QPOINT *qPoints,
                                                double farDist, double rangeExt )
{
   double sumRad = atomsOctree[ nodeA ].cr + qPointsOctree[ nodeQ ].cr;
   double maxD2 = ( sumRad + farDist ) * ( sumRad + farDist );
   double dx = qPointsOctree[ nodeQ ].cx - atomsOctree[ nodeA ].cx,
          dy = qPointsOctree[ nodeQ ].cy - atomsOctree[ nodeA ].cy,
          dz = qPointsOctree[ nodeQ ].cz - atomsOctree[ nodeA ].cz;
   double d2 = dx * dx + dy * dy + dz * dz;
   
   if ( d2 <= maxD2 )
      {
       if ( atomsOctree[ nodeA ].leaf && qPointsOctree[ nodeQ ].leaf )
         {            
           for ( int i = atomsOctree[ nodeA ].atomsStartID; i <= atomsOctree[ nodeA ].atomsEndID; i++ )
            {
              double r2 = ( atoms[ i ].r + rangeExt ) * ( atoms[ i ].r + rangeExt );
              
              for ( int j = qPointsOctree[ nodeQ ].qPtsStartID; j <= qPointsOctree[ nodeQ ].qPtsEndID; j++ )
                {                            
                 dx = qPoints[ j ].x - atoms[ i ].x;
                 dy = qPoints[ j ].y - atoms[ i ].y;
                 dz = qPoints[ j ].z - atoms[ i ].z;                     
                        
                 d2 = dx * dx + dy * dy + dz * dz;       
                 
                 if ( d2 < r2 ) qPoints[ j ].h += atoms[ i ].h;
                }                 
            }   
         }
       else if ( !atomsOctree[ nodeA ].leaf && !qPointsOctree[ nodeQ ].leaf )         
              {
                for ( int i = 0; i < 8; i++ )
                  if ( atomsOctree[ nodeA ].cPtr[ i ] >= 0 )
                     for ( int j = 0; j < 8; j++ )
                       if ( qPointsOctree[ nodeQ ].cPtr[ j ] >= 0 ) 
                          assignHydrophobicityToQPoints( atomsOctree, atomsOctree[ nodeA ].cPtr[ i ], atoms,
                                                         qPointsOctree, qPointsOctree[ nodeQ ].cPtr[ j ], qPoints,
                                                         farDist, rangeExt );                         
              }
            else if ( !atomsOctree[ nodeA ].leaf )         
                   {
                     for ( int i = 0; i < 8; i++ )
                       if ( atomsOctree[ nodeA ].cPtr[ i ] >= 0 )
                          assignHydrophobicityToQPoints( atomsOctree, atomsOctree[ nodeA ].cPtr[ i ], atoms,
                                                         qPointsOctree, nodeQ, qPoints, farDist, rangeExt );                         
                   }
                 else 
                   {
                     for ( int j = 0; j < 8; j++ )
                       if ( qPointsOctree[ nodeQ ].cPtr[ j ] >= 0 )
                          assignHydrophobicityToQPoints( atomsOctree, nodeA, atoms, 
                                                         qPointsOctree, qPointsOctree[ nodeQ ].cPtr[ j ], qPoints, farDist, rangeExt );                         
                   }              
      }      
}


inline void pseudoGsol::transformPoint( double x, double y, double z, double *transMat, double *nx, double *ny, double *nz )
{
   *nx = transMat[  0 ] * x + transMat[  1 ] * y + transMat[  2 ] * z + transMat[  3 ];
   *ny = transMat[  4 ] * x + transMat[  5 ] * y + transMat[  6 ] * z + transMat[  7 ];
   *nz = transMat[  8 ] * x + transMat[  9 ] * y + transMat[ 10 ] * z + transMat[ 11 ];       
}


inline void pseudoGsol::transformPoint( Point p, double *transMat, Point *np )
{
  np->setX(transMat[  0 ] * p.getX() + transMat[  1 ] * p.getY() + transMat[  2 ] * p.getZ() + transMat[  3 ]);
  np->setY(transMat[  4 ] * p.getX() + transMat[  5 ] * p.getY() + transMat[  6 ] * p.getZ() + transMat[  7 ]);
  np->setZ(transMat[  8 ] * p.getX() + transMat[  9 ] * p.getY() + transMat[ 10 ] * p.getZ() + transMat[ 11 ]);
}



double pseudoGsol::computeXlateForPG( int numStQPoints, QPOINT *stQPoints, int numMvQPoints, QPOINT *mvQPoints )
{
   double minXYZ;
   
   for ( int i = 0; i < numStQPoints; i++ )
     {
       if ( i == 0 ) minXYZ = stQPoints[ i ].x;
       else if ( stQPoints[ i ].x < minXYZ ) minXYZ = stQPoints[ i ].x;
       
       if ( stQPoints[ i ].y < minXYZ ) minXYZ = stQPoints[ i ].y;
       if ( stQPoints[ i ].z < minXYZ ) minXYZ = stQPoints[ i ].z;
     }

   double minX, minY, minZ;
   double maxX, maxY, maxZ;

   for ( int i = 0; i < numMvQPoints; i++ )
     {
       if ( i == 0 ) 
         {
           minX = maxX = mvQPoints[ i ].x;
           minY = maxY = mvQPoints[ i ].y;
           minZ = maxZ = mvQPoints[ i ].z;           
         }  
       else  
         {
           if ( mvQPoints[ i ].x < minX ) minX = mvQPoints[ i ].x;
           if ( mvQPoints[ i ].y < minY ) minY = mvQPoints[ i ].y;
           if ( mvQPoints[ i ].z < minZ ) minZ = mvQPoints[ i ].z;                      

           if ( mvQPoints[ i ].x > maxX ) maxX = mvQPoints[ i ].x;
           if ( mvQPoints[ i ].y > maxY ) maxY = mvQPoints[ i ].y;
           if ( mvQPoints[ i ].z > maxZ ) maxZ = mvQPoints[ i ].z;                      
         }         
     }

   double maxD = 0, d;

   d = fabs( maxX - minX );
   if ( d > maxD ) maxD = d; 

   d = fabs( maxY - minY );
   if ( d > maxD ) maxD = d; 
   
   d = fabs( maxZ - minZ );
   if ( d > maxD ) maxD = d; 

   minXYZ -= ( 2 * maxD );
   
   if ( minXYZ < 0 ) return -minXYZ;
   else return 0;  
}



void pseudoGsol::printGsolParamters( FILE* fp )
{
  fprintf( fp, "# \t staticMoleculeQUAD = %s\n", params.staticMoleculeQUAD );
  fprintf( fp, "# \t movingMoleculeQUAD = %s\n", params.movingMoleculeQUAD );  
  
  fprintf( fp, "# \t distanceCutoff = %lf\n", params.distanceCutoff );      
}


double pseudoGsol::determinant( double *trans )
{
  return ( - trans[ LID( 0, 2 ) ] * trans[ LID( 1, 1 ) ] * trans[ LID( 2, 0 ) ]
           + trans[ LID( 0, 1 ) ] * trans[ LID( 1, 2 ) ] * trans[ LID( 2, 0 ) ]
           + trans[ LID( 0, 2 ) ] * trans[ LID( 1, 0 ) ] * trans[ LID( 2, 1 ) ]
           - trans[ LID( 0, 0 ) ] * trans[ LID( 1, 2 ) ] * trans[ LID( 2, 1 ) ]
           - trans[ LID( 0, 1 ) ] * trans[ LID( 1, 0 ) ] * trans[ LID( 2, 2 ) ]
           + trans[ LID( 0, 0 ) ] * trans[ LID( 1, 1 ) ] * trans[ LID( 2, 2 ) ] );
}


void pseudoGsol::invert( double *trans, double *transI )
{
  double d = determinant( trans );
  
  if ( d != 0.0 )
     {
       transI[ LID( 0, 0 ) ] = ( trans[ LID( 1, 1 ) ] * trans[ LID( 2, 2 ) ]
                               - trans[ LID( 1, 2 ) ] * trans[ LID( 2, 1 ) ] ) / d;
                               
       transI[ LID( 0, 1 ) ] = ( trans[ LID( 0, 2 ) ] * trans[ LID( 2, 1 ) ] 
                               - trans[ LID( 0, 1 ) ] * trans[ LID( 2, 2 ) ] ) / d;
                               
       transI[ LID( 0, 2 ) ] = ( trans[ LID( 0, 1 ) ] * trans[ LID( 1, 2 ) ]
                               - trans[ LID( 0, 2 ) ] * trans[ LID( 1, 1 ) ] ) / d;
                               
       transI[ LID( 0, 3 ) ] = ( trans[ LID( 0, 3 ) ] * trans[ LID( 1, 2 ) ] * trans[ LID( 2, 1 ) ] 
                               - trans[ LID( 0, 2 ) ] * trans[ LID( 1, 3 ) ] * trans[ LID( 2, 1 ) ] 
                               - trans[ LID( 0, 3 ) ] * trans[ LID( 1, 1 ) ] * trans[ LID( 2, 2 ) ] 
                               + trans[ LID( 0, 1 ) ] * trans[ LID( 1, 3 ) ] * trans[ LID( 2, 2 ) ] 
                               + trans[ LID( 0, 2 ) ] * trans[ LID( 1, 1 ) ] * trans[ LID( 2, 3 ) ] 
                               - trans[ LID( 0, 1 ) ] * trans[ LID( 1, 2 ) ] * trans[ LID( 2, 3 ) ] ) / d;

       transI[ LID( 1, 0 ) ] = ( trans[ LID( 1, 2 ) ] * trans[ LID( 2, 0 ) ] 
                               - trans[ LID( 1, 0 ) ] * trans[ LID( 2, 2 ) ] ) / d;
                               
       transI[ LID( 1, 1 ) ] = ( trans[ LID( 0, 0 ) ] * trans[ LID( 2, 2 ) ]
                               - trans[ LID( 0, 2 ) ] * trans[ LID( 2, 0 ) ] ) / d;
                               
       transI[ LID( 1, 2 ) ] = ( trans[ LID( 0, 2 ) ] * trans[ LID( 1, 0 ) ]
                               - trans[ LID( 0, 0 ) ] * trans[ LID( 1, 2 ) ] ) / d;
                               
       transI[ LID( 1, 3 ) ] = ( trans[ LID( 0, 2 ) ] * trans[ LID( 1, 3 ) ] * trans[ LID( 2, 0 ) ] 
                               - trans[ LID( 0, 3 ) ] * trans[ LID( 1, 2 ) ] * trans[ LID( 2, 0 ) ] 
                               + trans[ LID( 0, 3 ) ] * trans[ LID( 1, 0 ) ] * trans[ LID( 2, 2 ) ] 
                               - trans[ LID( 0, 0 ) ] * trans[ LID( 1, 3 ) ] * trans[ LID( 2, 2 ) ] 
                               - trans[ LID( 0, 2 ) ] * trans[ LID( 1, 0 ) ] * trans[ LID( 2, 3 ) ] 
                               + trans[ LID( 0, 0 ) ] * trans[ LID( 1, 2 ) ] * trans[ LID( 2, 3 ) ] ) / d;

       transI[ LID( 2, 0 ) ] = ( trans[ LID( 1, 0 ) ] * trans[ LID( 2, 1 ) ]
                               - trans[ LID( 1, 1 ) ] * trans[ LID( 2, 0 ) ] ) / d;
                               
       transI[ LID( 2, 1 ) ] = ( trans[ LID( 0, 1 ) ] * trans[ LID( 2, 0 ) ]
                               - trans[ LID( 0, 0 ) ] * trans[ LID( 2, 1 ) ] ) / d;
                               
       transI[ LID( 2, 2 ) ] = ( trans[ LID( 0, 0 ) ] * trans[ LID( 1, 1 ) ]
                               - trans[ LID( 0, 1 ) ] * trans[ LID( 1, 0 ) ] ) / d;
                               
       transI[ LID( 2, 3 ) ] = ( trans[ LID( 0, 3 ) ] * trans[ LID( 1, 1 ) ] * trans[ LID( 2, 0 ) ] 
                               - trans[ LID( 0, 1 ) ] * trans[ LID( 1, 3 ) ] * trans[ LID( 2, 0 ) ] 
                               - trans[ LID( 0, 3 ) ] * trans[ LID( 1, 0 ) ] * trans[ LID( 2, 1 ) ] 
                               + trans[ LID( 0, 0 ) ] * trans[ LID( 1, 3 ) ] * trans[ LID( 2, 1 ) ] 
                               + trans[ LID( 0, 1 ) ] * trans[ LID( 1, 0 ) ] * trans[ LID( 2, 3 ) ] 
                               - trans[ LID( 0, 0 ) ] * trans[ LID( 1, 1 ) ] * trans[ LID( 2, 3 ) ] ) / d;
     }
  else 
     {
       for ( int i = 0; i < 12; i++ )
         transI[ i ] = ( ( i % 5 ) ? 0.0 : 1.0 );
     }   
}


void pseudoGsol::initOctreeFlags( int threadID )
{
   int offset = 2 * threadID * numStaticQPointsOctreeNodes;
   
   for ( int i = 0; i < numStaticQPointsOctreeNodes; i++ )
     staticQPointsOctreeFlags[ offset + i ] = false;
     
   offset = 2 * threadID * numMovingQPointsOctreeNodes;
   
   for ( int i = 0; i < numMovingQPointsOctreeNodes; i++ )
     movingQPointsOctreeFlags[ offset + i ] = false;     
}


void pseudoGsol::collectPseudoGsol( int threadID, double *trans, double *transI, double *pGsol, 
                                    double *pGsolHStaticPos, double *pGsolHStaticNeg, double *pGsolHMovingPos, double *pGsolHMovingNeg )
{
   *pGsol = *pGsolHStaticPos = *pGsolHStaticNeg = *pGsolHMovingPos = *pGsolHMovingNeg = 0;

   int offset = 2 * threadID * numStaticQPointsOctreeNodes;   
   
   for ( int i = 0; i < numStaticQPointsOctreeNodes; i++ )
     if ( staticQPointsOctreeFlags[ offset + i ] )
        {
           for ( int j = staticQPointsOctree[ i ].qPtsStartID; j <= staticQPointsOctree[ i ].qPtsEndID; j++ )
             {
               Point p, q;
               
               p.setX(staticQPoints[ j ].x);
               p.setY(staticQPoints[ j ].y);
               p.setZ(staticQPoints[ j ].z);
               
               transformPoint( p, transI, &q );
               
               if ( movingPG->pointsWithinRange( &q, params.distanceCutoff ) )
                 {

			staticTriangleIds.push_back(j);
                   ( *pGsol ) += staticQPoints[ j ].w;
                  if ( staticQPoints[ j ].h > 0 ) ( *pGsolHStaticPos ) += staticQPoints[ j ].h * staticQPoints[ j ].w;
                   else ( *pGsolHStaticNeg ) += staticQPoints[ j ].h * staticQPoints[ j ].w;
                 }  
             }            
        } 
     
   offset = 2 * threadID * numMovingQPointsOctreeNodes;
   
   for ( int i = 0; i < numMovingQPointsOctreeNodes; i++ )
     if ( movingQPointsOctreeFlags[ offset + i ] )
        {
           for ( int j = movingQPointsOctree[ i ].qPtsStartID; j <= movingQPointsOctree[ i ].qPtsEndID; j++ )
             {
               Point p, q;
               
               p.setX(movingQPoints[ j ].x);
               p.setY(movingQPoints[ j ].y);
               p.setZ(movingQPoints[ j ].z);
               
               transformPoint( p, trans, &q );
               
               if ( staticPG->pointsWithinRange( &q, params.distanceCutoff ) )
                 {
			movingTriangleIds.push_back(j);
                   ( *pGsol ) += movingQPoints[ j ].w;
	                 if ( movingQPoints[ j ].h > 0 ) ( *pGsolHMovingPos ) += movingQPoints[ j ].h * movingQPoints[ j ].w;
                   else ( *pGsolHMovingNeg ) += movingQPoints[ j ].h * movingQPoints[ j ].w;
                 }  
             }         
        }         
}




void pseudoGsol::markPotentialQPoints( int threadID, int nodeS, int nodeM, double *trans )
{
   double sumRad = staticQPointsOctree[ nodeS ].cr + movingQPointsOctree[ nodeM ].cr;
   double maxD2 = ( sumRad + params.distanceCutoff ) * ( sumRad + params.distanceCutoff );
   Point P, Q;
   
   P.setX(movingQPointsOctree[ nodeM ].cx);
   P.setY(movingQPointsOctree[ nodeM ].cy);
   P.setZ(movingQPointsOctree[ nodeM ].cz);
   
   transformPoint( P, trans, &Q );
   
   double dx = staticQPointsOctree[ nodeS ].cx - Q.getX(),
     dy = staticQPointsOctree[ nodeS ].cy - Q.getY(),
     dz = staticQPointsOctree[ nodeS ].cz - Q.getZ();
   double d2 = dx * dx + dy * dy + dz * dz;

   if ( d2 <= maxD2 )
      {
       if ( staticQPointsOctree[ nodeS ].leaf && movingQPointsOctree[ nodeM ].leaf )
         {            
           staticQPointsOctreeFlags[ 2 * threadID * numStaticQPointsOctreeNodes + nodeS ] = true;
           movingQPointsOctreeFlags[ 2 * threadID * numMovingQPointsOctreeNodes + nodeM ] = true;
         }
       else if ( !staticQPointsOctree[ nodeS ].leaf && !movingQPointsOctree[ nodeM ].leaf )         
              {
                for ( int i = 0; i < 8; i++ )
                  if ( staticQPointsOctree[ nodeS ].cPtr[ i ] >= 0 )
                     for ( int j = 0; j < 8; j++ )
                       if ( movingQPointsOctree[ nodeM ].cPtr[ j ] >= 0 ) 
                           markPotentialQPoints( threadID, staticQPointsOctree[ nodeS ].cPtr[ i ], movingQPointsOctree[ nodeM ].cPtr[ j ], trans );                         
              }
            else if ( !staticQPointsOctree[ nodeS ].leaf )         
                   {
                     for ( int i = 0; i < 8; i++ )
                       if ( staticQPointsOctree[ nodeS ].cPtr[ i ] >= 0 )
                           markPotentialQPoints( threadID, staticQPointsOctree[ nodeS ].cPtr[ i ], nodeM, trans );                         
                   }
                 else 
                   {
                     for ( int j = 0; j < 8; j++ )
                       if ( movingQPointsOctree[ nodeM ].cPtr[ j ] >= 0 )
                           markPotentialQPoints( threadID, nodeS, movingQPointsOctree[ nodeM ].cPtr[ j ], trans );                         
                   }              
      }      
}



void pseudoGsol::getPseudoGsol( int threadID, double *trans, double *pGsol,
                                double *pGsolHStaticPos, double *pGsolHStaticNeg, double *pGsolHMovingPos, double *pGsolHMovingNeg )
{
   if ( ( threadID < 0 ) || ( threadID >= numThreads ) )
     {
       printError( "Invalid thread id!" );
       exit( 1 );
     }
   
   printf( "\nentered getPseudoGsol ( numStaticQPointsOctreeNodes = %d, numMovingQPointsOctreeNodes = %d )\n", 
            numStaticQPointsOctreeNodes, numMovingQPointsOctreeNodes );            
   fflush( stdout );
   
   initOctreeFlags( threadID );   
   
   markPotentialQPoints( threadID, staticQPointsOctreeRoot, movingQPointsOctreeRoot, trans );

   double transI[ 12 ];
   
   invert( trans, transI );  

//   printf( "trans:\n" );   
//   for ( int i = 0; i < 12; i++ )
//       printf( "%12.6lf%s", trans[ i ], ( ( i + 1 ) % 4 ) ? " " : "\n" );
//
//   printf( "transI:\n" );   
//   for ( int i = 0; i < 12; i++ )
//       printf( "%12.6lf%s", transI[ i ], ( ( i + 1 ) % 4 ) ? " " : "\n" );
//       
//   double transT[ 12 ];
//   
//   for ( int i = 0; i < 3; i++ )    
//     for ( int j = 0; j < 4; j++ )
//       {
//         int ij = i * 4 + j;
//         
//         transT[ ij ] = 0;
//         
//         for ( int k = 0; k < 4; k++ )
//           {
//             int ik = i * 4 + k;
//             int kj = k * 4 + j;
//             
//             transT[ ij ] += trans[ ik ] * ( ( k == 3 ) ? ( ( j == 3 ) ? 1.0 : 0.0 ) : transI[ kj ] );
//           }
//       }  
//
//   printf( "transT:\n" );   
//   for ( int i = 0; i < 12; i++ )
//       printf( "%12.6lf%s", transT[ i ], ( ( i + 1 ) % 4 ) ? " " : "\n" );
   
   collectPseudoGsol( threadID, trans, transI, pGsol, pGsolHStaticPos, pGsolHStaticNeg, pGsolHMovingPos, pGsolHMovingNeg );
   
   printf( "\nexiting getPseudoGsol ( numStaticQPointsOctreeNodes = %d, numMovingQPointsOctreeNodes = %d )\n", 
            numStaticQPointsOctreeNodes, numMovingQPointsOctreeNodes );
   fflush( stdout );   
}


void pseudoGsol::getPseudoGsol( int threadID, double *pGsol, double *pGsolHStaticPos, double *pGsolHStaticNeg, double *pGsolHMovingPos, double *pGsolHMovingNeg )
{
  double trans[ ] = { 1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0 };

  getPseudoGsol( threadID, trans, pGsol, pGsolHStaticPos, pGsolHStaticNeg, pGsolHMovingPos, pGsolHMovingNeg );   
}


void pseudoGsol::getPseudoGsol( int threadID, double *trans, double *pGsol, double *pGsolH )
{
   double pGsolHStaticPos, pGsolHStaticNeg, pGsolHMovingPos, pGsolHMovingNeg;
   
   getPseudoGsol( threadID, trans, pGsol, &pGsolHStaticPos, &pGsolHStaticNeg, &pGsolHMovingPos, &pGsolHMovingNeg );
   
   ( *pGsolH ) = pGsolHStaticPos + pGsolHStaticNeg + pGsolHMovingPos + pGsolHMovingNeg;
}


void pseudoGsol::getPseudoGsol( int threadID, double *pGsol, double *pGsolH )
{
  double trans[ ] = { 1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0 };

  getPseudoGsol( threadID, trans, pGsol, pGsolH );
}



bool pseudoGsol::getParamsFromFile( PARAMS_IN *p, char *paramFile )
{
  char s[ 2000 ];
  char key[ 500 ], val[ 500 ];
  FILE *fp;

  fp = fopen( paramFile, "r" );

  if ( fp == NULL )
    {
      printError( "Failed to open parameter file %s!", paramFile );
      return false;
    }

  p->staticMoleculeQUAD = p->staticMoleculeQUAD = NULL;
  p->distanceCutoff = 1.5;
  
  while ( fgets( s, 1999, fp ) != NULL )
    {
      if ( sscanf( s, "%s %s", key, val ) != 2 ) continue;
    
      if ( !strcasecmp( key, "staticMoleculeQUAD" ) ) p->staticMoleculeQUAD = strdup( val );
      else if ( !strcasecmp( key, "movingMoleculeQUAD" ) ) p->movingMoleculeQUAD = strdup( val );
      else if ( !strcasecmp( key, "distanceCutoff" ) )
             {
               double v = atof( val );
               
               if ( v < 0 )
                 {
                   printError( "%s must be a non-negative float!", key );
                   fclose( fp );
                   return false;
                 }
                 
               p->distanceCutoff = v;                 
             }  
    }

  fclose( fp );
    
  if ( p->staticMoleculeQUAD == NULL )  
    { 
      printError( "Missing QUAD file name for the static molecule!" );
      return false;
    }

  if ( p->movingMoleculeQUAD == NULL )  
    { 
      printError( "Missing QUAD file name for the moving molecule!" );
      return false;
    }
    
  return true;
}



bool pseudoGsol::readAtoms( char *atomsFile, int *nAtoms, double **atoms )
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

void pseudoGsol::getInterfacePoints(double *trans, char *staticFile, char *movingFile)
{
  double u, v;
  getPseudoGsol( 0, trans, &u, &v );

  FILE *fp = fopen( staticFile, "wt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open output file (%s)!", staticFile );
      return;
     }

   int size = staticTriangleIds.size();

   for(int i=0;i<size;i++)
	fprintf(fp, "%d\n", staticTriangleIds[i]);

   fclose(fp);

  fp = fopen( movingFile, "wt" );
   
   if ( fp == NULL )
     {
      printError( "Failed to open output file (%s)!", movingFile );
      return;
     }

   size = movingTriangleIds.size();

   for(int i=0;i<size;i++)
	fprintf(fp, "%d\n", movingTriangleIds[i]);

   fclose(fp);

}
