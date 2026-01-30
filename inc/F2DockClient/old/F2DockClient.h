#ifndef F2DOCKCLIENT_H
#define F2DOCKCLIENT_H

#include "XmlRPC/XmlRpc.h"
#include "basicParams.h"
#include "f2dgenParams.h"
#include "rerankBasicParams.h"
#include "quadgenParams.h"
#include <iostream>
#include <fstream>

using namespace XmlRpc;
using namespace std;

class F2DockClient
{
	private:
		BasicParams* baspar;
		F2dParams* f2dpar;
		RerankBasicParams* gbpar;
		QuadParams* quadpar;
		int port;
		char hostName[100];
		XmlRpcValue args, id, result;
		string pid;

	public:
		F2DockClient();
		~F2DockClient();
		void setValues(BasicParams* bp);
		bool submitRequest(void);
		bool getResult();
		void setF2dGenValues(F2dParams* fp);
		bool submitF2dGenRequest(void);
		bool getF2dGenResult();
		void setGBRerankValues(RerankBasicParams* gp);
		bool submitGBRerankRequest(void);
		bool getGBRerankResult();
		void setQuadGenValues(QuadParams* qp);
		bool submitQuadGenRequest(void);
		bool getQuadGenResult();
		bool isComplete(string jobid);
};

#endif
