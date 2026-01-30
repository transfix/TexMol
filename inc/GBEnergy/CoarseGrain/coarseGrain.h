#ifndef COARSEGRAIN_H
#define COARSEGRAIN_H
#include <vector>

namespace MOLECULE
{
	class GroupOfGBAtoms;
	class CoarseGrain
	{
	public:
		CoarseGrain();
		CoarseGrain(GroupOfGBAtoms* m);
		GroupOfGBAtoms* CoarseGrainMolecule(GroupOfGBAtoms* m);
		void CoarseGrainCharge(GroupOfGBAtoms *GroupOfBeads);
		void CoarseGrainChargeGB(GroupOfGBAtoms *groupOfBeads);
		void CoarseGrainChargeGB2(GroupOfGBAtoms *groupOfBeads);
		void CoarseGrainGeometry(GroupOfGBAtoms *GroupOfBeads);
	private:
		GroupOfGBAtoms* molecule;
		typedef struct _indexRange {
			int min[3];
			int max[3];
		}IndexRange;

//		void generateBeads(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
		void atomClustering(GroupOfGBAtoms *GroupOfBeads, int head, int tail);

                void generateNormalBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateBackboneBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateARGBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateGLNBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateGLUBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateHISBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateLYSBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateMETBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generatePHEBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateTRPBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateTYRBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
		void generateGLYBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
// new clustering
		void generateVALBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateLEUBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateILEBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateASNBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateASPBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateALABead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateSERBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateTHRBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generateCYSBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);
                void generatePROBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail);

		bool isBackboneAtom(int a);
		void getMassCenter(int *AtomList, double *coord, int numOfChild);
		void getGeometryCenter(int *AtomList, double *coord, int numOfChild);
		double getInitialRadius(int *AtomList, double *center, int numOfChild);

		void getBoundingBox(GroupOfGBAtoms *groupOfBeads, double cutoff, int *Min, int *Max);
		void getBeadRange(int *min, int *max, double cutoff, GroupOfGBAtoms *groupOfBeads, IndexRange* beadRamge);
		void getResidueEnd(GroupOfGBAtoms *groupOfBeads, int *residueEnd);
		void setupLinearSystem(double *A, double *b, GroupOfGBAtoms *groupOfBeads, IndexRange *beadRange, IndexRange *atomRange);
		void setupLinearSystem2bead(double *A, double *b, GroupOfGBAtoms *groupOfBeads, IndexRange *beadRange, IndexRange *atomRange, double *Q);
		void setupLinearSystemAnybead2(double *A, double *b, GroupOfGBAtoms *groupOfBeads, IndexRange *beadRange, IndexRange *atomRange, double *Q, int *residueEnd, int *min, int *max);
		void setupLinearSystemAnybead1(double *A, double *b, GroupOfGBAtoms *groupOfBeads, IndexRange *beadRange, IndexRange *atomRange, double *Q, int *residueEnd);
		double getDistanceSum(int M, int *Mmin, int *Mmax, GroupOfGBAtoms *groupOfBeads);
		double getDistanceSum(int i, int j, int *imin, int *imax, int *jmin, int *jmax, GroupOfGBAtoms *groupOfBeads);
		double getyDistanceSum(int M, int *Mmin, int *Mmax, GroupOfGBAtoms *groupOfBeads, IndexRange *atomRange);
		double getYk(int kx, int ky, int kz, GroupOfGBAtoms *goa, IndexRange *atomRange);
		void LinearSystemSolver(double *AA, double *bb, int N);
		void totalChargeContained(GroupOfGBAtoms *groupOfBeads, double *Q, int N);
		void assignCharge(GroupOfGBAtoms *groupOfBeads, double *b, double *Q, int *residueEnd);
		void check(GroupOfGBAtoms *groupOfBeads, int* min, int *max, IndexRange *beadRange, IndexRange *atomRange);
		bool isInAtomRange(int i, int j, int k, int l, IndexRange *atomRange); 
		int getResidueIndex(int beadIndex, int *residueEnd, int Nr);
		double getBeadSelfEnergy(GroupOfGBAtoms *groupOfBeads, int index);
		double getAtomicGBEnergy();
		double getAtomicColumbEnergy();
	};
}

#endif // COARSEGRAIN_H
