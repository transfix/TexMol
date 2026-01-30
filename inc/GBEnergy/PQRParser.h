#ifndef PQRPARSER_H
#define PQRPARSER_H

#include <stdio.h>
#include "atom.h"

namespace MOLECULE
{
	class GBAtom;
	class GroupOfGBAtoms;
	class PQRParser
	{
	public:
		PQRParser();
		GroupOfGBAtoms* ParserPQRtoGOA(FILE* fp, MOLECULE::RADIUS_TYPE radiusType);
		GroupOfGBAtoms* ParserOptimalPQRtoGOA(FILE* fp, MOLECULE::RADIUS_TYPE radiusType);
	private:
        	void getString( char* line, char* str, int pos, int len );
	        void getChar( char* line, char* str, int pos );
	        void getInt( char* line, int* i, int pos, int len );
	        void getDouble( char* line, double* d, int pos, int len );
	};
}

#endif //PQRPARSER_H
