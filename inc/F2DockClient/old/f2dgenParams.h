#ifndef F2DPAR
#define F2DPAR

#include <qvariant.h>
#include <string>

using namespace std;

typedef struct
{
	QString receptorFile;
	QString ligandFile;
	QString hostName;
	int port;
	string jobid;
	bool submitted;
	bool completed;
} F2dParams;

#endif
