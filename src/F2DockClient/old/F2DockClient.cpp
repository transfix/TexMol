#include "F2DockClient/F2DockClient.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace XmlRpc;
using namespace std;

F2DockClient::F2DockClient()
{
	FILE* parfile;
	parfile = fopen("host.ini", "rt");
	if (!parfile)
	{
		cout<<"Host file not found. Using default values"<<endl;
		sprintf(hostName,"prism2.ices.utexas.edu");
		port = 8091;
	}
	else
	{
		fscanf(parfile, "%s", hostName);
		fscanf(parfile, "%d", &port);
		fclose(parfile);
		cout<<"Hostname =  "<<hostName<<endl;
		cout<<"Port = "<< port<<endl;
	}
}

F2DockClient::~F2DockClient()
{
}

void F2DockClient::setValues(BasicParams* bp)
{
	baspar = bp;
}

bool F2DockClient::submitRequest(void)
{
	XmlRpcClient c(hostName, port);
	ifstream receptorFP, ligandFP, rmsdFP, inputFP;
	string recepBuffer, ligandBuffer, rmsdBuffer, inputBuffer, outputBuffer;
	string line;
	// receptor file
	receptorFP.open(baspar->receptorFile.ascii());
	if (!receptorFP)
	{
		cerr << "Unable to open receptor File ";
		return false;
	}
	while (getline(receptorFP, line))
	{
		recepBuffer += line + "\n";
	}
	receptorFP.close();
	// ligand file
	ligandFP.open(baspar->ligandFile.ascii());
	if (!ligandFP)
	{
		cerr << "Unable to open ligand File";
		return false;
	}
	while (getline(ligandFP, line))
	{
		ligandBuffer += line + "\n";
	}
	ligandFP.close();
	// input file
	inputFP.open(baspar->inputFile.ascii());
	if (!inputFP)
	{
		cerr << "Unable to open input File";
		return false;
	}
	while (getline(inputFP, line))
	{
		inputBuffer += line + "\n";
	}
	inputFP.close();
	// rmsd file
	rmsdFP.open(baspar->rmsdFile.ascii());
	if (!rmsdFP)
	{
		cerr << "Unable to open input File";
		return false;
	}
	while (getline(rmsdFP, line))
	{
		rmsdBuffer += line + "\n";
	}
	rmsdFP.close();
	args[0] = inputBuffer;
	args[1] = recepBuffer;
	args[2] = ligandBuffer;
	args[3] = rmsdBuffer;
	cout<<"Prepared the XMLRPC values. Submitting request"<<endl;
	if (c.execute("Exec", args, id))
	{
		cout<<"Request submitted"<<endl;
		baspar->jobid = string(id);
		return true;
	}
	else
	{
		std::cout << "Error calling server\n\n";
		return false;
	}
	return true;
}

bool F2DockClient::getResult(void)
{
	XmlRpcClient c(hostName, port);
	ofstream outputFP;
	id = baspar->jobid.c_str();
	if (c.execute("GetOutput", id, result))
	{
		string file = string(result);
		if (file == "INCOMPLETE")
		{
			return false;
		}
		else
		{
			outputFP.open(baspar->outputFile.ascii());
			if (!outputFP)
			{
				cerr << "Error creating output file.";
				return false;
			}
			outputFP << file;
			outputFP.close();
			return true;
		}
	}
	return false;
}

void F2DockClient::setF2dGenValues(F2dParams* fp)
{
	f2dpar = fp;
}

bool F2DockClient::submitF2dGenRequest(void)
{
	XmlRpcClient c(hostName, port);
	ifstream receptorFP, ligandFP;
	string recepBuffer, ligandBuffer;
	string line;
	// receptor file
	receptorFP.open(f2dpar->receptorFile.ascii());
	if (!receptorFP)
	{
		cerr << "Unable to open receptor File ";
		return false;
	}
	while (getline(receptorFP, line))
	{
		recepBuffer += line + "\n";
	}
	receptorFP.close();
	// ligand file
	ligandFP.open(f2dpar->ligandFile.ascii());
	if (!ligandFP)
	{
		cerr << "Unable to open ligand File";
		return false;
	}
	while (getline(ligandFP, line))
	{
		ligandBuffer += line + "\n";
	}
	ligandFP.close();
	args[0] = recepBuffer;
	args[1] = ligandBuffer;
	cout<<"Prepared the XMLRPC values. Submitting request"<<endl;
	if (c.execute("ExecF2dGen", args, id))
	{
		cout<<"Request submitted"<<endl;
		f2dpar->jobid = string(id);
		return true;
	}
	else
	{
		std::cout << "Error calling server\n\n";
		return false;
	}
	return true;
}

bool F2DockClient::getF2dGenResult(void)
{
	XmlRpcClient c(hostName, port);
	ofstream outputFP;
	char outfile1[100], outfile2[100];
	strncpy(outfile1, f2dpar->receptorFile.ascii(), strlen(f2dpar->receptorFile.ascii())-4);
	strncpy(outfile2, f2dpar->ligandFile.ascii(), strlen(f2dpar->ligandFile.ascii())-4);
	strcat(outfile1, ".f2d");
	strcat(outfile2, ".f2d");
	cout<<"Here"<<endl;
	id = f2dpar->jobid.c_str();
	if (c.execute("GetF2dR", id, result))
	{
		cout<<"here too"<<endl;
		string file = string(result);
		cout<<"here as well"<<endl;
		if (file == "INCOMPLETE")
		{
			return false;
		}
		else
		{
			outputFP.open(outfile1);
			if (!outputFP)
			{
				cerr << "Error creating output file.";
				return false;
			}
			outputFP << file;
			outputFP.close();
		}
	}
	if (c.execute("GetF2dL", id, result))
	{
		cout<<"here too"<<endl;
		string file = string(result);
		cout<<"here as well"<<endl;
		if (file == "INCOMPLETE")
		{
			return false;
		}
		else
		{
			outputFP.open(outfile2);
			if (!outputFP)
			{
				cerr << "Error creating output file.";
				return false;
			}
			outputFP << file;
			outputFP.close();
		}
	}
	return true;
}

void F2DockClient::setGBRerankValues(RerankBasicParams* bp)
{
	gbpar = bp;
}

bool F2DockClient::submitGBRerankRequest(void)
{
	XmlRpcClient c(hostName, port);
	ifstream receptorPDBFP, ligandPDBFP, receptorQUADFP, ligandQUADFP, f2dockOutputFP, gbrerankInputFP;
	string receptorPDBBuffer, ligandPDBBuffer, receptorQUADBuffer, ligandQUADBuffer, f2dockOutputBuffer, gbrerankInputBuffer, gbrerankOutputBuffer;
	string line;
	// receptor PDB file
	receptorPDBFP.open(gbpar->receptorPDB.ascii());
	if (!receptorPDBFP)
	{
		cerr << "Unable to open receptor PDB file ";
		return false;
	}
	while (getline(receptorPDBFP, line))
	{
		receptorPDBBuffer += line + "\n";
	}
	receptorPDBFP.close();
	// ligand PDB file
	ligandPDBFP.open(gbpar->ligandPDB.ascii());
	if (!ligandPDBFP)
	{
		cerr << "Unable to open ligand PDB file ";
		return false;
	}
	while (getline(ligandPDBFP, line))
	{
		ligandPDBBuffer += line + "\n";
	}
	ligandPDBFP.close();
	// receptor QUAD file
	receptorQUADFP.open(gbpar->receptorQUAD.ascii());
	if (!receptorQUADFP)
	{
		cerr << "Unable to open receptor QUAD file ";
		return false;
	}
	while (getline(receptorQUADFP, line))
	{
		receptorQUADBuffer += line + "\n";
	}
	receptorQUADFP.close();
	// ligand QUAD file
	ligandQUADFP.open(gbpar->ligandQUAD.ascii());
	if (!ligandQUADFP)
	{
		cerr << "Unable to open ligand QUAD file ";
		return false;
	}
	while (getline(ligandQUADFP, line))
	{
		ligandQUADBuffer += line + "\n";
	}
	ligandQUADFP.close();
	// F2Dock output file
	f2dockOutputFP.open(gbpar->f2dockOutputFile.ascii());
	if (!f2dockOutputFP)
	{
		cerr << "Unable to open F2Dock output file ";
		return false;
	}
	while (getline(f2dockOutputFP, line))
	{
		f2dockOutputBuffer += line + "\n";
	}
	f2dockOutputFP.close();
	// GBRerank input file
	gbrerankInputFP.open(gbpar->rerankInputFile.ascii());
	if (!gbrerankInputFP)
	{
		cerr << "Unable to open GBRerank output file ";
		return false;
	}
	while (getline(gbrerankInputFP, line))
	{
		gbrerankInputBuffer += line + "\n";
	}
	gbrerankInputFP.close();
	args[0] = receptorPDBBuffer;
	args[1] = ligandPDBBuffer;
	args[2] = receptorQUADBuffer;
	args[3] = ligandQUADBuffer;
	args[4] = f2dockOutputBuffer;
	args[5] = gbrerankInputBuffer;
	cout<<"Prepared the XMLRPC values. Submitting request"<<endl;
	if (c.execute("ExecGBRerank", args, id))
	{
		cout<<"Request submitted"<<endl;
		gbpar->jobid = string(id);
		return true;
	}
	else
	{
		std::cout << "Error calling server\n\n";
		return false;
	}
	return true;
}

bool F2DockClient::getGBRerankResult(void)
{
	XmlRpcClient c(hostName, port);
	ofstream gbrerankOutputFP;
	cout<<"Here"<<endl;
	id = gbpar->jobid.c_str();
	if (c.execute("GetGBRerankOutput", id, result))
	{
		cout<<"here too"<<endl;
		string file = string(result);
		cout<<"here as well"<<endl;
		if (file == "INCOMPLETE")
		{
			return false;
		}
		else
		{
			gbrerankOutputFP.open(gbpar->rerankOutputFile.ascii());
			if (!gbrerankOutputFP)
			{
				cerr << "Error creating output file.";
				return false;
			}
			gbrerankOutputFP << file;
			gbrerankOutputFP.close();
			return true;
		}
	}
	return false;
}

void F2DockClient::setQuadGenValues(QuadParams* qp)
{
	quadpar = qp;
}

bool F2DockClient::submitQuadGenRequest(void)
{
	XmlRpcClient c(hostName, port);
	ifstream receptorFP, ligandFP;
	string recepBuffer, ligandBuffer;
	string line;
	// receptor file
	receptorFP.open(quadpar->receptorFile.ascii());
	if (!receptorFP)
	{
		cerr << "Unable to open receptor File ";
		return false;
	}
	while (getline(receptorFP, line))
	{
		recepBuffer += line + "\n";
	}
	receptorFP.close();
	// ligand file
	ligandFP.open(quadpar->ligandFile.ascii());
	if (!ligandFP)
	{
		cerr << "Unable to open ligand File";
		return false;
	}
	while (getline(ligandFP, line))
	{
		ligandBuffer += line + "\n";
	}
	ligandFP.close();
	args[0] = recepBuffer;
	args[1] = ligandBuffer;
	args[2] = quadpar->fine?"64":"128";
	cout<<"Prepared the XMLRPC values. Submitting request"<<endl;
	if (c.execute("ExecQuadGen", args, id))
	{
		cout<<"Request submitted"<<endl;
		quadpar->jobid = string(id);
		return true;
	}
	else
	{
		std::cout << "Error calling server\n\n";
		return false;
	}
	return true;
}

bool F2DockClient::getQuadGenResult(void)
{
	XmlRpcClient c(hostName, port);
	ofstream outputFP;
	char outfile1[100], outfile2[100];
	strncpy(outfile1, quadpar->receptorFile.ascii(), strlen(quadpar->receptorFile.ascii())-4);
	strncpy(outfile2, quadpar->ligandFile.ascii(), strlen(quadpar->ligandFile.ascii())-4);
	strcat(outfile1, ".quad");
	strcat(outfile2, ".quad");
	cout<<"Here"<<endl;
	id = quadpar->jobid.c_str();
	if (c.execute("GetQuadR", id, result))
	{
		cout<<"here too"<<endl;
		string file = string(result);
		cout<<"here as well"<<endl;
		if (file == "INCOMPLETE")
		{
			return false;
		}
		else
		{
			outputFP.open(outfile1);
			if (!outputFP)
			{
				cerr << "Error creating output file.";
				return false;
			}
			outputFP << file;
			outputFP.close();
		}
	}
	if (c.execute("GetQuadL", id, result))
	{
		cout<<"here too"<<endl;
		string file = string(result);
		cout<<"here as well"<<endl;
		if (file == "INCOMPLETE")
		{
			return false;
		}
		else
		{
			outputFP.open(outfile2);
			if (!outputFP)
			{
				cerr << "Error creating output file.";
				return false;
			}
			outputFP << file;
			outputFP.close();
		}
	}
	return true;
}

bool F2DockClient::isComplete(string str)
{
	XmlRpcClient c(hostName, port);
	id = str.c_str();
	string res;
	string done("done");
	if (c.execute("IsComplete", id, result))
	{
		res = string(result);
		if (res.compare(done) != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
