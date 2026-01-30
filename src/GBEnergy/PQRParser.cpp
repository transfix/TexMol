#include <iostream>
#include <cstdio>
#include <cstring>
#include <assert.h>

#include "GBEnergy/PQRParser.h"
#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"

using namespace MOLECULE;
using namespace std;

PQRParser::PQRParser()
{

}

GroupOfGBAtoms* PQRParser::ParserPQRtoGOA(FILE* fp, MOLECULE::RADIUS_TYPE radiusType)
{
	char atomname[10];
	int serial;
	char name[10];
	char resName[10];
	char chainID;
	int resSeq;
	double p[3];
	double q;
	double r;
	char line[82];
	int numofres = 0;
	int resSeq0 = -1;

	GroupOfGBAtoms* molecule = new GroupOfGBAtoms();

	int atomIndex=0;
	while( fgets( line, 81, fp ) != 0 )
	{
		char* str;
		if( (str = strstr( line, "ATOM" ))  && (strstr(line, "REMARK")) == NULL)
		{
		        getString( line, atomname, 0, 6 );
		        getInt( line, &serial, 6, 5 );
		        getString( line, name, 12, 4 );
		        getString( line, resName, 17, 3 );
		        getChar( line, &chainID, 21 );
		        getInt( line, &resSeq, 22, 4 );
		        getDouble( line, &p[0], 30, 8 );
		        getDouble( line, &p[1], 38, 8 );
		        getDouble( line, &p[2], 46, 8 );
		        getDouble( line, &q, 54, 9 );
		        getDouble( line, &r, 63, 7 );

			GBAtom* a = new GBAtom();
			a->getData(atomIndex, name, resName, chainID, resSeq, p, q, r, radiusType);//translate resName to resID
			molecule->addAtom(a);
			atomIndex++;
			// count number of residues
			if (resSeq != resSeq0)
			{
				numofres++;
				resSeq0 = resSeq;
			}
		}
	}

	molecule->numOfAtoms = atomIndex;
	molecule->numOfRes = numofres;
/*
	cout << atomIndex << " atoms, " << residueIndex << " residues, " << molecule->specialResidue.size() << " special residues." << endl;

	int i;
	cout << "Special Residues:" << endl;
	for(i=0;i<molecule->specialResidue.size();i++)
		cout << molecule->specialResidue[i][0] << endl;
*/
	return molecule;
}

GroupOfGBAtoms* PQRParser::ParserOptimalPQRtoGOA(FILE* fp, MOLECULE::RADIUS_TYPE radiusType)
{
        char atomname[10];
        int serial;
        char name[10];
        char resName[10];
	char chainID;
        int resSeq;
        double p[3];
        double q;
        double r;
        char line[82];

        GroupOfGBAtoms* molecule = new GroupOfGBAtoms();

        int atomIndex=0;
        while( fgets( line, 81, fp ) != 0 )
        {
                char* str;
                if( (str = strstr( line, "ATOM" ))  && (strstr(line, "REMARK")) == NULL)
                {
                        getString( line, atomname, 0, 6 );
                        getInt( line, &serial, 6, 5 );
                        getString( line, name, 12, 4 );
                        getString( line, resName, 17, 3 );
                        getChar( line, &chainID, 21 );
                        getInt( line, &resSeq, 22, 4 );
                        getDouble( line, &p[0], 30, 8 );
                        getDouble( line, &p[1], 38, 8 );
                        getDouble( line, &p[2], 46, 8 );
                        getDouble( line, &q, 54, 9 );
                        getDouble( line, &r, 63, 7 );

                        GBAtom* a = new GBAtom();
                        a->getData(atomIndex, name, resName, chainID, resSeq, p, q, r, radiusType);//translate resName to resID
                        molecule->addAtom(a);
                        atomIndex++;
                }
        }

        molecule->numOfAtoms = atomIndex;
        return molecule;
}


void PQRParser::getString( char* line, char* str, int pos, int len )
{
        char buffer[80];

        assert( len < 80 );
        strcpy( str, "" );
        strncpy( buffer, &line[pos], len );
        buffer[len] = '\0';

        int i;
        for( i=0; i<len; i++ )
                str[i] = buffer[i];
        str[len] = '\0';
}

void PQRParser::getChar( char* line, char* str, int pos )
{
        char buffer[80];

        *str = ' ';
        strncpy( buffer, &line[pos], 1 );
        buffer[1] = '\0';
        sscanf( buffer, "%c", str );
}

void PQRParser::getInt( char* line, int* i, int pos, int len )
{
        char buffer[80];

        *i = -1;
        strncpy( buffer, &line[pos], len );
        buffer[len] = '\0';
        sscanf( buffer, "%d", i );
}

void PQRParser::getDouble( char* line, double* d, int pos, int len )
{
        char buffer[80];

        *d = -1;
        strncpy( buffer, &line[pos], len );
        buffer[len] = '\0';
        sscanf( buffer, "%lf", d );
}

