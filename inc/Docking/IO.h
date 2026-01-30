#ifndef CCV_TEXMOL_DOCKING_IO_H
#define CCV_TEXMOL_DOCKING_IO_H

namespace PDBParser
{
	class GroupOfAtoms;
};

namespace Docking
{
	class IO
	{
		public:
			IO();
			virtual ~IO();
			bool createProteinsDockingInputFile(PDBParser::GroupOfAtoms* molecule,
												const char* outputFile,
												bool printMiscFiles = false,
												bool useLargestComponent = false);
			bool createLigandsDockingInputFile(PDBParser::GroupOfAtoms* molecule,
											   const char* outputFile,
											   bool printMiscFiles);
		protected:
			bool isClose(double x1, double y1, double z1, double r, double x2, double y2, double z2, double* d);
	};
};

#endif
