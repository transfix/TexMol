#ifndef UTILS_H

#define UTILS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

#if ! defined(__APPLE__)
#include <stdlib.h>
#endif

#include <stdarg.h>
#include <time.h>

#ifdef _WIN32
   #include <sys/types.h>
   #include <sys/timeb.h>
#else
   #include <sys/time.h>
#endif

#ifdef freeMem
   #undef freeMem
#endif
#define freeMem( ptr ) { if ( ptr != NULL ) free( ptr ); }

#ifdef zeroIfLess   
   #undef zeroIfLess
#endif
#define zeroIfLess( a, b ) ( ( ( a ) < ( b ) ) ? 0 : 1 )

#define transformUtil( ox, oy, oz, M, nx, ny, nz ) {                                                                      \
                                                 nx = ( ox ) * M[ 0 ] + ( oy ) * M[ 1 ] + ( oz ) * M[  2 ] + M[  3 ]; \
                                                 ny = ( ox ) * M[ 4 ] + ( oy ) * M[ 5 ] + ( oz ) * M[  6 ] + M[  7 ]; \
                                                 nz = ( ox ) * M[ 8 ] + ( oy ) * M[ 9 ] + ( oz ) * M[ 10 ] + M[ 11 ]; \
                                               }

#ifndef M_PI
   #define M_PI 3.1415926535897932384626433832795
#endif

#ifndef INV_SQRT_TWO
   #define INV_SQRT_TWO 0.70710678118654752440084436210485
#endif

#ifdef USE_SSE

  #include <xmmintrin.h>

  #ifdef _MSC_VER
    # define ALIGN16_BEG __declspec(align(16))
    # define ALIGN16_END 
  #else
    # define ALIGN16_BEG
    # define ALIGN16_END __attribute__((aligned(16)))
  #endif

  typedef __m128 v4sf;

  typedef ALIGN16_BEG union 
    {
      float f[ 4 ];
      int i[ 4 ];
      v4sf  v;
    } ALIGN16_END V4SF;

#endif


void printError( const char *format, ... );
void printError( char * format, ... );
void f_printf( FILE *fp, const char *format, ... );
double getTime2( void );
int skipWhiteSpaces( char *buf, int i );
int skipInitial( char *s1, char *s2, char *p );
int getInt( char *buf, int i, int *v );
int getDouble( char *buf, int i, double *v );
bool getDoublesInRange( char *buf, int i1, int i2, double *v );
int getAlphaString( char *buf, int i, char *s );
int getString( char *buf, int i, char *s );
bool stringToBool(char * s);
#endif
