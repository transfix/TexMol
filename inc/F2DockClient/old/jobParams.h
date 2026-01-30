#ifndef JOBPAR
#define JOBPAR

#include "basicParams.h"
#include "f2dgenParams.h"
#include "rerankBasicParams.h"
#include "quadgenParams.h"
#include <string>

using namespace std;

typedef struct
{
	BasicParams baspar;
	F2dParams f2dpar;
	RerankBasicParams gbpar;
	QuadParams quadpar;
	QString hostName;
	int port;
	string jobid;
	int jobtype;
	bool submitted;
	bool running;
	bool completed;
	bool available;
} JobParams;

#endif
