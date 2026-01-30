/*#include <CGAL/Cartesian_d.h>
//#include <CGAL/Gmpq.h>
#include <CGAL/Min_sphere_of_spheres_d.h>*/
#include <vector>
#include <string.h>
#include <math.h>

#include "GBEnergy/CoarseGrain/coarseGrain.h"
#include "GBEnergy/atom.h"
#include "GBEnergy/groupOfAtoms.h"
/*
const int D = 3;
typedef double						FT;
typedef CGAL::Cartesian_d<FT>				K;
typedef CGAL::Min_sphere_of_spheres_d_traits_d<K,FT,D>  Traits;
typedef CGAL::Min_sphere_of_spheres_d<Traits> 		Min_sphere;
typedef K::Point_d                        		Point;
typedef Traits::Sphere                    		Sphere;
typedef Min_sphere::Cartesian_const_iterator    	CCI;
typedef Min_sphere::Support_iterator      		SI;
*/
using namespace MOLECULE;
using namespace std;

void CoarseGrain::atomClustering(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	generateBackboneBead(GroupOfBeads, head, tail);

        if (strcmp(molecule->m_Atoms[head]->resName, "ARG") == 0 ) {
                generateARGBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "GLN") == 0 ) {
                generateGLNBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "GLU") == 0 ) {
                generateGLUBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "HIS") == 0 ) {
                generateHISBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "LYS") == 0 ) {
                generateLYSBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "MET") == 0 ) {
                generateMETBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "PHE") == 0 ) {
                generatePHEBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "TRP") == 0 ) {
                generateTRPBead(GroupOfBeads, head, tail);
		return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "TYR") == 0 ) {
                generateTYRBead(GroupOfBeads, head, tail);
		return;
        }

	if (strcmp(molecule->m_Atoms[head]->resName, "GLY") == 0 ) {
                generateGLYBead(GroupOfBeads, head, tail);
                return;
        }
// new clustering
        if (strcmp(molecule->m_Atoms[head]->resName, "VAL") == 0 ) {
                generateVALBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "LEU") == 0 ) {
                generateLEUBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "ILE") == 0 ) {
                generateILEBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "ASN") == 0 ) {
                generateASNBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "ASP") == 0 ) {
                generateASPBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "ALA") == 0 ) {
                generateALABead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "SER") == 0 ) {
                generateSERBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "THR") == 0 ) {
                generateTHRBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "CYS") == 0 ) {
                generateCYSBead(GroupOfBeads, head, tail);
                return;
        }
        if (strcmp(molecule->m_Atoms[head]->resName, "PRO") == 0 ) {
                generatePROBead(GroupOfBeads, head, tail);
                return;
        }
/*
        generateNormalBead(GroupOfBeads, head, tail);
*/
}

void CoarseGrain::generateBackboneBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, molecule->m_Atoms[head]->resName);
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_bb1 = new GBAtom();
        GBAtom *bead_bb2 = new GBAtom();
        GBAtom *bead_bb3 = new GBAtom();

        vector<int> BackboneList1;
        vector<int> BackboneList2;
        vector<int> BackboneList3;

        for (i = head; i <= tail; i++)
        {
                if (isBackboneAtom(i))
                {
/*                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "A") != NULL)
*/                                BackboneList1.push_back(i);
/*                        else if ( strstr(atomName, "N") != NULL ||
                                  strstr(atomName, "H") != NULL)
                                BackboneList2.push_back(i);
                        else
                                BackboneList3.push_back(i);
*/                }
        }
        bead_bb1->child = new int[BackboneList1.size()];
//        bead_bb2->child = new int[BackboneList2.size()];
//        bead_bb3->child = new int[BackboneList3.size()];

        for (i = 0; i < BackboneList1.size(); i++)
                bead_bb1->child[i] = BackboneList1[i];
/*        for (i = 0; i < BackboneList2.size(); i++)
                bead_bb2->child[i] = BackboneList2[i];
        for (i = 0; i < BackboneList3.size(); i++)
                bead_bb3->child[i] = BackboneList3[i];
*/
        bead_bb1->numOfChildren = BackboneList1.size();
//        bead_bb2->numOfChildren = BackboneList2.size();
//        bead_bb3->numOfChildren = BackboneList3.size();

        bead_bb1->setResName(resName);
//        bead_bb2->setResName(resName);
//        bead_bb3->setResName(resName);

        bead_bb1->setResIndex(resSeq);
//        bead_bb2->setResIndex(resSeq);
//        bead_bb3->setResIndex(resSeq);

	bead_bb1->setChainID(chainID);
//	bead_bb2->setChainID(chainID);
//	bead_bb3->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_bb1);
//        GroupOfBeads->m_Atoms.push_back(bead_bb2);
//        GroupOfBeads->m_Atoms.push_back(bead_bb3);
}

void CoarseGrain::generateARGBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "ARG");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
		{
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];
	bead_sc1->numOfChildren = SidechainList1.size();
	bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateGLNBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "GLN");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
		{
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateGLUBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "GLU");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateHISBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "HIS");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL || 
			     strstr(atomName, "ND1") != NULL ||
			     strstr(atomName, "HD1") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateLYSBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "LYS");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

        bead_sc1->child = new int[SidechainList1.size()];
        bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateMETBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "MET");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

        bead_sc1->child = new int[SidechainList1.size()];
        bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generatePHEBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "PHE");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL ||
			     strstr(atomName, "D") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

        bead_sc1->child = new int[SidechainList1.size()];
        bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateTRPBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "TRP");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL ||
			     strstr(atomName, "D1") != NULL ||
			     strstr(atomName, "E1") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

        bead_sc1->child = new int[SidechainList1.size()];
        bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateTYRBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, "TYR");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
                        char atomName[10];
                        strcpy(atomName, molecule->m_Atoms[i]->atomName);

                        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL ||
			     strstr(atomName, "D") != NULL)
                                SidechainList1.push_back(i);
                        else SidechainList2.push_back(i);
                }
        }

        bead_sc1->child = new int[SidechainList1.size()];
        bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateGLYBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
	strcpy(resName, "GLY");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;
}

/* new clustering begins */
void CoarseGrain::generateVALBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "VAL");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}


void CoarseGrain::generateLEUBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "LEU");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}


void CoarseGrain::generateILEBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "ILE");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G2") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateASNBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "ASN");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateASPBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "ASP");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL) 
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}


void CoarseGrain::generateALABead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "ALA");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();

        vector<int> SidechainList1;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
                        	SidechainList1.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc1->setResName(resName);
        bead_sc1->setResIndex(resSeq);
	bead_sc1->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
}


void CoarseGrain::generateSERBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "SER");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if (strstr(atomName, "G") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}


void CoarseGrain::generateTHRBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "THR");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "G1") != NULL) 
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}


void CoarseGrain::generateCYSBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "CYS");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "S") != NULL) 
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}


void CoarseGrain::generatePROBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
	char resName[5];
	strcpy(resName, "PRO");
        int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

        int i;
        GBAtom *bead_sc1 = new GBAtom();
        GBAtom *bead_sc2 = new GBAtom();

        vector<int> SidechainList1;
        vector<int> SidechainList2;

        for (i = head; i <= tail; i++)
        {
                if (!isBackboneAtom(i))
                {
		        char atomName[10];
        		strcpy(atomName, molecule->m_Atoms[i]->atomName);
	
		        if ( strstr(atomName, "B") != NULL || 
			     strstr(atomName, "G") != NULL)
                        	SidechainList1.push_back(i);
			else SidechainList2.push_back(i);
		}
        }
	bead_sc1->child = new int[SidechainList1.size()];
	bead_sc2->child = new int[SidechainList2.size()];

        for (i = 0; i < SidechainList1.size(); i++)
                bead_sc1->child[i] = SidechainList1[i];
        for (i = 0; i < SidechainList2.size(); i++)
                bead_sc2->child[i] = SidechainList2[i];

        bead_sc1->numOfChildren = SidechainList1.size();
        bead_sc2->numOfChildren = SidechainList2.size();

        bead_sc1->setResName(resName);
        bead_sc2->setResName(resName);

        bead_sc1->setResIndex(resSeq);
        bead_sc2->setResIndex(resSeq);

	bead_sc1->setChainID(chainID);
	bead_sc2->setChainID(chainID);

        GroupOfBeads->m_Atoms.push_back(bead_sc1);
        GroupOfBeads->m_Atoms.push_back(bead_sc2);
}

void CoarseGrain::generateNormalBead(GroupOfGBAtoms *GroupOfBeads, int head, int tail)
{
        char resName[5];
        strcpy(resName, molecule->m_Atoms[head]->resName);
	int resSeq = molecule->m_Atoms[head]->resSeq;
	char chainID = molecule->m_Atoms[head]->chainID;

	int i;
	GBAtom *bead_bb = new GBAtom();
	GBAtom *bead_sc = new GBAtom();
	vector<int> BackboneList;                  
	vector<int> SidechainList;

	for (i = head; i <= tail; i++)
        {
		if (isBackboneAtom(i))
                        BackboneList.push_back(i);
		else
			SidechainList.push_back(i);
	}

        bead_bb->child = new int[BackboneList.size()];
        bead_sc->child = new int[SidechainList.size()];

        for (i = 0; i < BackboneList.size(); i++)
                bead_bb->child[i] = BackboneList[i];
        for (i = 0; i < SidechainList.size(); i++)
                bead_sc->child[i] = SidechainList[i];
	bead_bb->numOfChildren = BackboneList.size();
	bead_sc->numOfChildren = SidechainList.size();

	bead_bb->setResName(resName);
	bead_sc->setResName(resName);

	bead_bb->setResIndex(resSeq);
	bead_sc->setResIndex(resSeq);
	
	bead_bb->setChainID(chainID);
	bead_sc->setChainID(chainID);

/****** Smallest Enclosing Ball of Balls method **********************
	for (i = head; i <= tail; i++)
	{
		coord[0] = molecule->m_Atoms[i]->position[0];
		coord[1] = molecule->m_Atoms[i]->position[1];
		coord[2] = molecule->m_Atoms[i]->position[2];
	
		Point Pt (D, coord, coord+D);
		//cout << Pt[0] << " " << Pt[1] << " " << Pt[2]<< endl;
   		Sphere sph(Pt, molecule->m_Atoms[i]->radius);
		if (isBackboneAtom(i))
    			SBackbone.push_back(sph);
		else if (!isBackboneAtom(i))
			SSidechain.push_back(sph);
		else
			cout << "Wrong Atom" << endl;
  	}

	Min_sphere ms_bb(SBackbone.begin(),SBackbone.end());       // check in the spheres
	CGAL_assertion(ms_bb.is_valid());

	Min_sphere ms_sc(SSidechain.begin(),SSidechain.end());       // check in the spheres
	CGAL_assertion(ms_sc.is_valid());

	q = 0.0;

	i = 0;
	for (CCI cit = ms_bb.center_cartesian_begin(); cit != ms_bb.center_cartesian_end(); cit++)
	{
//		cout << *cit << endl;
		p[i++] = *cit;
	}
//	cout << p[0] << " " << p[1] << " " << p[2] << endl;
	r = ms_bb.radius();
	bead_bb->getData(p, q, r);

	i = 0; 
 	for (CCI cit = ms_sc.center_cartesian_begin(); cit != ms_sc.center_cartesian_end(); cit++)
                p[i++] = *cit;
	r = ms_sc.radius();
	bead_sc->getData(p, q, r);
*/
	GroupOfBeads->m_Atoms.push_back(bead_bb);
	GroupOfBeads->m_Atoms.push_back(bead_sc);
}

bool CoarseGrain::isBackboneAtom(int a)
{
	char atomName[10];
	strcpy(atomName, molecule->m_Atoms[a]->atomName);
	if ( strcmp(atomName, " N  ") == 0 ||
	     strcmp(atomName, " CA ") == 0 ||
	     strcmp(atomName, " C  ") == 0 ||
	     strcmp(atomName, " O  ") == 0 ||
	     strcmp(atomName, " H  ") == 0 ||
	     strcmp(atomName, " HA ") == 0 ||
	     strcmp(atomName, " H2 ") == 0 ||
	     strcmp(atomName, " H3 ") == 0)
		return true;
	else	return false;
}

void CoarseGrain::CoarseGrainGeometry(GroupOfGBAtoms *GroupOfBeads)
{
	int i;
	for (i = 0; i < GroupOfBeads->numOfAtoms; i++)
	{
		double center[3];
		double radius;
		int *childlist = GroupOfBeads->m_Atoms[i]->child;
		int numOfChild = GroupOfBeads->m_Atoms[i]->numOfChildren;
//		getMassCenter(childlist, center, numOfChild);
		getGeometryCenter(childlist, center, numOfChild);
		radius = getInitialRadius(childlist, center, numOfChild);
		GroupOfBeads->m_Atoms[i]->setPosition(center);
		GroupOfBeads->m_Atoms[i]->setDefinedRadius(radius);
	}
}

void CoarseGrain::getMassCenter(int *AtomList, double *coord, int numOfChild)
{
	int i;
	double c[3], total_mass; 

	c[0] = c[1] = c[2] = total_mass = 0.0;
	for (i = 0; i < numOfChild; i++)
	{
		double p[3], mass;
		int k = AtomList[i];
		p[0] = molecule->m_Atoms[k]->position[0];
		p[1] = molecule->m_Atoms[k]->position[1];
		p[2] = molecule->m_Atoms[k]->position[2];
		mass  = molecule->m_Atoms[k]->mass;

		c[0] += mass*p[0];
		c[1] += mass*p[1];
		c[2] += mass*p[2];

		total_mass += mass;		
	}
	coord[0] = c[0]/total_mass;	coord[1] = c[1]/total_mass;	coord[2] = c[2]/total_mass;
}

void CoarseGrain::getGeometryCenter(int *AtomList, double *coord, int numOfChild)
{
        int i;
        double c[3];

        c[0] = c[1] = c[2] = 0.0;
        for (i = 0; i < numOfChild; i++)
        {
                double p[3], mass;
                int k = AtomList[i];
                p[0] = molecule->m_Atoms[k]->position[0];
                p[1] = molecule->m_Atoms[k]->position[1];
                p[2] = molecule->m_Atoms[k]->position[2];

                c[0] += p[0];
                c[1] += p[1];
                c[2] += p[2];
        }
        coord[0] = c[0]/numOfChild;     coord[1] = c[1]/numOfChild;     coord[2] = c[2]/numOfChild;
}

double CoarseGrain::getInitialRadius(int *AtomList, double *center, int numOfChild)
{
	int i;
	double p[3], r, d, max_d;
	max_d = 0.0;
	for (i = 0; i < numOfChild; i++)
	{
		p[0] = molecule->m_Atoms[AtomList[i]]->position[0];
		p[1] = molecule->m_Atoms[AtomList[i]]->position[1];
		p[2] = molecule->m_Atoms[AtomList[i]]->position[2];
		r = molecule->m_Atoms[AtomList[i]]->radius;
		d = sqrt((p[0] - center[0])*(p[0] - center[0]) + 
			 (p[1] - center[1])*(p[1] - center[1]) + 
			 (p[2] - center[2])*(p[2] - center[2])) + r;
		if (d > max_d)	max_d = d;
	}
	return max_d;
}
