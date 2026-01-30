#ifndef QUADPAR
#define QUADPAR

#include <qvariant.h>
#include <string>

using namespace std;

typedef struct
{
	QString receptorFile;
	QString ligandFile;
	bool fine;
	string jobid;
	bool submitted;
	bool completed;
} QuadParams;

#endif
