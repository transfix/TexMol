#ifndef RBASICPAR
#define RBASICPAR

#include <qvariant.h>
#include <string>

using namespace std;

typedef struct
{
	QString receptorPDB;
	QString ligandPDB;
	QString receptorQUAD;
	QString ligandQUAD;
	QString f2dockOutputFile;
	QString rerankInputFile;
	QString rerankOutputFile;
	QString hostName;
	int port;
	string jobid;
	double gpolWeight;
	double gnonpolWeight;
	double f2dockWeight;
	double epsilonBR;
	double epsilonGpol;
	int numSol;
	bool submitted;
	bool completed;
} RerankBasicParams;

#endif
