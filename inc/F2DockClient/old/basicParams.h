#ifndef BASIC_PARAMS
#define BASIC_PARAMS

#include <qvariant.h>
#include <string>

using namespace std;

typedef struct
{
	QString receptorFile;
	QString receptorPDB;
	QString ligandFile;
	QString rmsdFile;
	QString outputFile;
	QString inputFile;
	QString hostName;
	int port;
	string jobid;
	int rotSeparation;
	double gridSpacing;
	int numSol;
	double ssWeight;
	double scWeight;
	double ccWeight;
	double elecWeight;
	bool bound;
	bool submitted;
	bool completed;
} BasicParams;

#endif
