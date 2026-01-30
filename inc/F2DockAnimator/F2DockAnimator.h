// F2Dock Animation Builder
// This class does two things:
// 1. Create PDBs and Surfaces from F2Dock results.
// 2. Build a keyframe video (quick hack at the end of summer)

#ifndef __F2DOCK_ANIMATOR_H__
#define __F2DOCK_ANIMATOR_H__

#include <PDBParser/GroupOfAtoms.h>
#include <UsefulMath/Vector.h>
#include <UsefulMath/Matrix.h>
#include <DataManager/DataManager.h>
#include <TexMol/RendererSet.h>
#include <string>
#include <qstring.h>
#include <cmath>

class F2DockAnimator
{
	public:
		// Main Interface
		F2DockAnimator(const char* F2DockOutput, const char* ProteinA_PDB, const char* ProteinB_PDB, const char* OutputDir);
		~F2DockAnimator();
		void buildFiles();
		void parseF2DockOutput(const char* filename);
		static void buildMovie(DataManager* dataManager, RendererSet* rendererSet);
	private:
		PDBParser::GroupOfAtoms* m_ProteinA;
		PDBParser::GroupOfAtoms* m_ProteinB;
		std::vector< CCVOpenGLMath::Matrix > m_Transformations;
		bool m_Loaded;
		std::string m_OutputDir;
		// helper methods
		void buildFrame(const int frameNo);
		static void saveScreenshot(int& frameNo, RendererSet* rendererSet);
};

#endif
