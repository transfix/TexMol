// Some common-purpose utility code

#ifndef UTILITY_CPP_
#define UTILITY_CPP_

#include <string>
#include <math.h>
#include <iostream>
#include <stdlib.h> 

using namespace std;

#define PI	3.14159265358979323846264338327
#define TRUE	1
#define FALSE	0

// Flip a switch
void toggle(bool &flag, string name)
{
	flag = !flag;
	if (flag) cout << name << " on\n";
	else      cout << name << " off\n";
}

// Clamps a value between 0 and 1
double clamp(double f)
{
	if(f < 0.0) return 0.0;
	if(f > 1.0) return 1.0;
	else 	    return f;
}

// Convert angles between degrees and radians
double degreesToRadians(double angle)
{
	return (PI/180)*angle;
}
double radiansToDegrees(double angle)
{
	return 180/(PI*angle);
}

// Test endianness
int endianTest()
{
	union
	{
		int testWord;
		char testByte[4];
	} endianTest;
	endianTest.testWord = 1;
	if (endianTest.testByte[0] == 1) {return 1;}
	else				 {return 0;}
}

// Print an error message and crash
void error(string s)
{
	cerr << s << "\n";
	exit(1);
}

// C-style memory allocation
// typecast the pointer
int* allocate(int size)
{
	int* temp = (int*) malloc(size);
	if(temp == NULL) error("Out of memory!");
	return temp;
}


#endif
