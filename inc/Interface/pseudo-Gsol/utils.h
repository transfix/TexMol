#ifndef UTILS_H

#define UTILS_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

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

#ifndef M_PI
   #define M_PI 3.1415926535897932384626433832795
#endif

#ifndef INV_SQRT_TWO
   #define INV_SQRT_TWO 0.70710678118654752440084436210485
#endif

void printError( const char *format, ... );
void f_printf( std::FILE *fp, const char *format, ... );
double getTime( void );
int skipWhiteSpaces( char *buf, int i );
int skipInitial( char *s1, char *s2, char *p );
int getInt( char *buf, int i, int *v );
int getDouble( char *buf, int i, double *v );
bool getDoublesInRange( char *buf, int i1, int i2, double *v );
int getAlphaString( char *buf, int i, char *s );
int getString( char *buf, int i, char *s );

#endif

