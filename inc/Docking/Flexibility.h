#ifndef CCV_TEXMOL_DOCKING_FLEXIBILITY_H
#define CCV_TEXMOL_DOCKING_FLEXIBILITY_H

#include <stdio.h>
#include <vector>
#include <stack>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Vector.h>
#include <Docking/Complex.h>

namespace PDBParser
{
	class GroupOfAtoms;
	class Atom;
};

namespace Docking
{
	const double AREA_WEIGHT_OF_CONNECTOR = 30;
	const double SHEAR_AREA_THRESHOLD = 200;
	const double MAX_SHEAR = 5;
	const double LIMITED_BEND_ANGLE = 10;
	const double BEND_ANGLE_PER_RESIDUE = 10;
	const double TWIST_PER_RESIDUE = 5;
	const double SHEAR_WEIGHT = 0.125;
	const double BENDING_WEIGHT = 0.5;
	const double SECONDARY_BENDING_WEIGHT = 0.125;
	const double TWIST_WEIGHT = 0.25;

	class DomainComplex;

	class Flexibility
	{
		public:
			Flexibility(DomainComplex* domainComplex, Domain* d1, Domain* d2);
			virtual ~Flexibility();
			bool allocateNumberOfDiscreteFlexibilities(int n);
			bool computeFlexibility();
			bool computeInterfaceArea();
			bool getUpdatedGOA(int permutationIndex);
			bool initNewGOA();
			bool isCut();
			bool print(FILE* fp);
			bool read(FILE* fp);
			int getDomainSize();
			int m_TotalDiscreteConformations;
			void computeCutProperty();
			DomainComplex* m_DomainComplex;
			std::vector<int> m_RightDomains;

		protected:
			bool isLeftRightConnector(Connector* c);
			bool isRightSideConnector(Connector* c);
			bool isConnectorOfRightDomain(Connector* c);
			void initValues();
			bool computeShearProperties();
			bool computeBending();
			bool computeSecondaryBendingAxis();
			bool computeTwistingAxis();
			bool readShear(FILE* fp);
			bool printShear(FILE* fp);
			bool readBending(FILE* fp);
			bool printBending(FILE* fp);
			bool readSecondaryBending(FILE* fp);
			bool printSecondaryBending(FILE* fp);
			bool readTwist(FILE* fp);
			bool printTwist(FILE* fp);
			int getNumberOfConnectors();
			float getSmallestConnectorLength();
			int getNumberOfResidueInShortestConnector();
			float get3DDistance(CCVOpenGLMath::Vector v1, CCVOpenGLMath::Vector v2);
			bool applyTransformations(int p1, int p2, int p3, int p4);
			bool shearMolecule(int p1);
			bool bendMolecule(int p2);
			bool secondaryBendMolecule(int p3);
			bool twistMolecule(int p4);
			bool m_Shear, m_Bend, m_SecondaryBend, m_Twist;
			Domain* m_Domain1;
			Domain* m_Domain2;
			double m_Area;
			// IMP: angles given in degrees
			// this connector will provide all info!!
			int m_ImportantConnectorIndex;
			// params for shear;
			CCVOpenGLMath::Vector m_PlaneNormal;
			double m_ShearMagnitude;
			// params for main bending
			CCVOpenGLMath::Vector m_BendingAxis;
			CCVOpenGLMath::Vector m_BendingAxisHingePoint;
			double m_MinBendingRange;
			double m_MaxBendingRange;
			// params for secondary bending
			CCVOpenGLMath::Vector m_SecondaryBendingAxis;
			CCVOpenGLMath::Vector m_SecondaryBendingAxisHingePoint;
			double m_SecondaryMinBendingRange;
			double m_SecondaryMaxBendingRange;
			// params for twisting
			double m_MinTwistingRange;
			double m_MaxTwistingRange;
			int m_TotalShear;
			int m_TotalBend;
			int m_TotalSecondaryBend;
			int m_TotalTwist;
			int m_CurrentShear;
			int m_CurrentBend;
			int m_CurrentSecondaryBend;
			int m_CurrentTwist;
			bool m_Cut;
	};
};

#endif
