#include <Blurmaps/BlurMapsDataManager.h>
#include <Docking/Complex.h>
#include <Docking/IO.h>
#include <Geometry/Geometry.h>
#include <PDBParser/Atom.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/FlattenGOA.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <SimpleVolumeData/SimpleVolumeDataIsocontourer.h>
#include <Utility/utility.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

//Include ordering dependency
#include <Blurmaps/AtomPopulator.h>


using PDBParser::GroupOfAtoms;
//using PDBParser::Atom;
using Docking::IO;

IO::IO()
{
}

IO::~IO()
{
}

bool IO::isClose(double x1, double y1, double z1, double r, double x2, double y2, double z2, double* d)
{
	double d1 = (x1-x2)*(x1-x2) +
				(y1-y2)*(y1-y2) +
				(z1-z2)*(z1-z2);
	*d = sqrt(d1);
	if (d1 < r*r)
	{
		return true;
	}
	return false;
}

bool IO::createProteinsDockingInputFile(PDBParser::GroupOfAtoms* molecule, const char* outputFile, bool printMiscFiles, bool useLargestComponent)
{
	if (!molecule)
	{
		return false;
	}
	double probeRadius = 1.4;
	int depth = 1;
	int dim1 = 128;
	int dim2 = 128;
	int dim3 = 128;
	float blobbiness = -1.0f;
	bool* boundaryAtom = 0;
	Geometry* largestComponent = 0;
	{
		SimpleVolumeData* sData = 0;
		{
			sData = BlurMapsDataManager::getVolume(molecule, "",dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, PDBParser::ATOM_TYPE);
			if (!sData)
			{
				return false;
			}
		}
		largestComponent = SimpleVolumeDataIsocontourer::getIsocontour(sData, 1.0);
		if (useLargestComponent)
		{
			// the code below breaks up isocontour into separate components
			vector<Geometry*> components;
			largestComponent->separateComponents(&components);
			delete largestComponent;
			if (components.size() < 1)
			{
				for (int i=0; i<components.size(); i++)
				{
					delete components[i];
				}
				components.clear();
				return false;
			}
			{
				int curSize = components[0]->m_NumTris;
				largestComponent = components[0];
				for (int i=1; i<components.size(); i++)
				{
					if (components[i]->m_NumTris > curSize)
					{
						curSize = components[i]->m_NumTris;
						largestComponent = components[i];
					}
				}
				for (int i=1; i<components.size(); i++)
				{
					if (components[i] != largestComponent)
					{
						delete components[i];
					}
				}
				components.clear();
			}
		}
	}
	// find surface atoms of molecule as those close to the largest component
	vector<PDBParser::Atom*> atomList;
	{
		PDBParser::CollectionData* collectionData = 0;
		if (molecule->type == PDBParser::COLLECTION_TYPE)
		{
			collectionData = molecule->m_CollectionData;
		}
		PDBParser::FlattenGOA(molecule, atomList, collectionData, 0, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, PDBParser::ATOM_TYPE, false);
	}
	boundaryAtom = new bool[atomList.size()];
	{
		for (int i=0; i<atomList.size(); i++)
		{
			boundaryAtom[i] = false;
			for (int v=0; v<largestComponent->m_NumTriVerts; v++)
			{
				double dist = 100;
				if (isClose(atomList[i]->m_Position[0],
							atomList[i]->m_Position[1],
							atomList[i]->m_Position[2],
							atomList[i]->getRadius() + 1.0,
							largestComponent->m_TriVerts[v*3+0],
							largestComponent->m_TriVerts[v*3+1],
							largestComponent->m_TriVerts[v*3+2],
							&dist
						   ))
				{
					boundaryAtom[i] = true;
					break;
				}
			}
		}
	}
	delete largestComponent;
	{
		if (printMiscFiles)
		{
			// the boundary atoms file takes in surface area exposed for the given atom.
			char bdyFile[256];
			strcpy(bdyFile, outputFile);
			strcat(bdyFile, ".bdy");
			FILE* fp = fopen(bdyFile, "w");
			char interiorfname[256];
			strcpy(interiorfname, outputFile);
			strcat(interiorfname, "_interior.pdb");
			FILE* fpOut = fopen(interiorfname, "w");
			char skinfname[256];
			strcpy(skinfname, outputFile);
			strcat(skinfname, "_skin.pdb");
			FILE* fpSkin = fopen(skinfname,"w");
			for (int i=0; i<atomList.size(); i++)
			{
				if (boundaryAtom[i])
				{
					fprintf(fp, "100000.0\n");
					fprintf(fpSkin, "ATOM  11111  CA  GLU A1111    %8.3f%8.3f%8.3f%6.2f                \n", atomList[i]->m_Position[0], atomList[i]->m_Position[1], atomList[i]->m_Position[2], atomList[i]->getRadius());
				}
				else
				{
					fprintf(fp, "0.0\n");
					fprintf(fpOut, "ATOM  11111  N   GLU A1111    %8.3f%8.3f%8.3f%6.2f                \n", atomList[i]->m_Position[0], atomList[i]->m_Position[1], atomList[i]->m_Position[2], atomList[i]->getRadius());
				}
			}
			fclose(fp);
			fclose(fpOut);
			fclose(fpSkin);
		}
		{
			// write docking input file
			FILE* fp = fopen(outputFile, "w");
			if (!fp)
			{
				return false;
			}
			fprintf(fp, "%lu\n", atomList.size());
			for (int i=0; i<atomList.size(); i++)
			{
				if (boundaryAtom[i])
				{
					continue;
				}
				PDBParser::Atom* at = atomList[i];
				fprintf(fp, "I     %12.5lf %12.5lf %12.5lf %12.5f\n", at->m_Position[0], at->m_Position[1], at->m_Position[2], at->getCharge());
			}
			for (int i=0; i<atomList.size(); i++)
			{
				if (boundaryAtom[i])
				{
					PDBParser::Atom* at = atomList[i];
					fprintf(fp, "E     %12.5lf %12.5lf %12.5lf %12.5f\n", at->m_Position[0], at->m_Position[1], at->m_Position[2], at->getCharge());
				}
			}
			fclose(fp);
		}
	}
	{
		int n = atomList.size();
		for (int i=0; i<n; i++)
		{
			delete atomList[i];
		}
		atomList.clear();
	}
	if (boundaryAtom)
	{
		delete [] boundaryAtom;
		boundaryAtom = 0;
	}
	return true;
}

bool IO::createLigandsDockingInputFile(PDBParser::GroupOfAtoms* molecule, const char* outputFile, bool printMiscFiles)
{
	int dim1 = 64, dim2 = 64, dim3 = 64;
	double probeRadius = 1.4;
	AtomPopulator* atomPopulator = new AtomPopulator(molecule, dim1, dim2, dim3, probeRadius, PDBParser::GroupOfAtoms::VDW_RADIUS);
	bool ret = atomPopulator->populate(outputFile, printMiscFiles);
	return ret;
}
