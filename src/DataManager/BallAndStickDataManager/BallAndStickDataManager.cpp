/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
// BallAndStickDataManager.cpp: implementation of the BallAndStickDataManager class.

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <qfile.h>
//#include <qtextstream.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <DataManager/BallAndStickDataManager/BallAndStickDataManager.h>
#include <PDBParser/ResidueData.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/Atom.h>
#include <PDBParser/parserPDBtoGOA.h>
#include <PDBParser/GOAFileIO.h>
#include <PDBParser/GOAMolGeometry.h>
#include <UsefulMath/Vector.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Quaternion.h>
#include <PDBParser/Residues.h>
#include <PDBParser/ResidueInformation.h>
#include <GOAFileTypes/GOALoader.h>

const int FF_UPDATE_PHI_PSI = 0;
const int FF_UPDATE_RESIDUE = 1;

BallAndStickDataManager::BallAndStickDataManager()
{
}

BallAndStickDataManager::~BallAndStickDataManager()
{
}

// Returns a GOA with all of the models (aka molecules) in the subgroups array of the return value GOA.
// If there are no models in the file (i.e. if the file only contains a single molecule),
// then the single molecule will be in the first slot in the returned GOA.
PDBParser::GroupOfAtoms* BallAndStickDataManager::readSimplePDBModels(const char* fileName)
{
	GOALoader* goaLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(fileName);
	delete goaLoader;
	return molecule;
}

// Global, when we find out the phi psi total rotation, this is set, then used
CCVOpenGLMath::Matrix phi_psi_mat;
// Global, when phi_psi is set, this is true, false otherwise
bool phi_psi_mat_set;
// If twist is performed on the most recent call to chain-twist, this is set to true.  Used for error detection.
bool phi_psi_twist_done;

bool residueContentsCorrect(PDBParser::GroupOfAtoms* residue)
{
	int j, m;
	m = (int) residue->m_Atoms.size();

	if(m > 14)
	{
		return false;
	}

	for(j=0; j < m; j++)
	{
		PDBParser::Atom* currAtom = residue->m_Atoms.at(j);

		if(PDBParser::strcmpCaseInsensitive(currAtom->getName(), PDBParser::AminoAcidResidueContentsOrdering[residue->resID][j+1]))
		{
			return false;
		}
	}

	return true;
}

// notes:
// phi: N - C_alpha
// psi: C_alpha - C'

// Performs type FF_UPDATE_PHI_PSI change:
//		rotate the residue, this updates only the phi psi
//		rotate the rest of the molecule with a constant matrix
void BallAndStickDataManager::twistChainRecursive(PDBParser::GroupOfAtoms* currGOA, int pdbResSeq, double phi, double psi)
{
	int i, n;

	// For chains, print out the backbone atoms instead of the bounding balls
	if(currGOA->type == PDBParser::SS_TYPE)
	{
		n = (int) currGOA->m_SubGroups.size();

		for(i=0; i < n; i++)
		{
			PDBParser::GroupOfAtoms* currRes = currGOA->m_SubGroups.at(i);

			//		go to required residue based on line number the residue ID ( each residue has the first atom's id )
			if(currRes->m_ResSeq == pdbResSeq)
			{
				if(!currRes->m_ResidueData)
				{
					continue;
				}

				// Find the deltas we need
				PDBParser::Atom* atomNi = currRes->m_Atoms.at(0);	// assume first atom is N
				PDBParser::Atom* atomCa = currRes->m_Atoms.at(1);	// assume second atom is C_alpha
				PDBParser::Atom* atomCp = currRes->m_Atoms.at(2);	// assume third atom is C prime
				double old_phi = currRes->m_ResidueData->m_Phi;
				double old_psi = currRes->m_ResidueData->m_Psi;
				// changes in phi / psi
				double d_phi = phi - old_phi;
				double d_psi = psi - old_psi;
				/*				if( currRes->m_ResSeq < 70 ||  currRes->m_ResSeq > 80)
								{
									d_phi = 0.000;
									d_psi = 0.000;
								}
				*/
				// update with new torsion angles
				currRes->m_ResidueData -> m_Phi = phi;
				currRes->m_ResidueData -> m_Psi = psi;
				// vectors of rotation
				double phi_x = atomCa->m_Position[0] - atomNi->m_Position[0];
				double phi_y = atomCa->m_Position[1] - atomNi->m_Position[1];
				double phi_z = atomCa->m_Position[2] - atomNi->m_Position[2];
				double psi_x = atomCp->m_Position[0] - atomCa->m_Position[0];
				double psi_y = atomCp->m_Position[1] - atomCa->m_Position[1];
				double psi_z = atomCp->m_Position[2] - atomCa->m_Position[2];
				// origins
				double phi_ox = atomCa->m_Position[0];
				double phi_oy = atomCa->m_Position[1];
				double phi_oz = atomCa->m_Position[2];
				double psi_ox = atomCp->m_Position[0];
				double psi_oy = atomCp->m_Position[1];
				double psi_oz = atomCp->m_Position[2];
				CCVOpenGLMath::Matrix phi_ptrans = CCVOpenGLMath::Matrix::translation(phi_ox,  phi_oy,  phi_oz);
				CCVOpenGLMath::Matrix phi_ntrans = CCVOpenGLMath::Matrix::translation(-phi_ox, -phi_oy, -phi_oz);
				CCVOpenGLMath::Matrix psi_ptrans = CCVOpenGLMath::Matrix::translation(psi_ox,  psi_oy,  psi_oz);
				CCVOpenGLMath::Matrix psi_ntrans = CCVOpenGLMath::Matrix::translation(-psi_ox, -psi_oy, -psi_oz);
				// Create the two matrices needed
				CCVOpenGLMath::Matrix phi_rot = CCVOpenGLMath::Quaternion::rotation(d_phi, phi_x, phi_y, phi_z).buildMatrix();
				CCVOpenGLMath::Matrix psi_rot = CCVOpenGLMath::Quaternion::rotation(d_psi, psi_x, psi_y, psi_z).buildMatrix();
				// Overall transformation
				CCVOpenGLMath::Matrix phi_mat = phi_ptrans * phi_rot * phi_ntrans;
				// Set the globals.  Fix someday.  pcd
				phi_psi_mat = psi_ptrans * psi_rot * psi_ntrans * phi_mat;	// first twist by phi, then apply psi twist
				phi_psi_mat_set = true;
				phi_psi_twist_done = true;
				// Transform points in the residue
				int j, m;
				m = (int) currRes->m_Atoms.size();

				for(j=0; j < m; j++)
				{
					PDBParser::Atom* currAtom = currRes->m_Atoms.at(j);
					CCVOpenGLMath::Vector v(currAtom->m_Position[0], currAtom->m_Position[1], currAtom->m_Position[2], 1);
					CCVOpenGLMath::Vector v2;
					v2.set(phi_mat * v);
					// pcd debug
					//					printf("prev: %6.2f now: %6.2f\n", currAtom->m_Position[0], v2[0]);
					/*					if( j<3 )
										{
											printf("%s : [%6.2f,%6.2f,%6.2f] -> [%6.2f,%6.2f,%6.2f]\n",
												currAtom->getName(),
												currAtom->m_Position[0],
												currAtom->m_Position[1],
												currAtom->m_Position[2],
												v2[0],
												v2[1],
												v2[2] );
										}*/
					currAtom->m_Position[0] = v2[0];
					currAtom->m_Position[1] = v2[1];
					currAtom->m_Position[2] = v2[2];
				}
			}
			// Go through remaining residues in the current SS if the phi-psi matrix is set (we are past
			// the residue we were looking for, but still in the same chain)
			else if(phi_psi_mat_set)
			{
				// Transform points in the residue
				int j, m;
				m = (int) currRes->m_Atoms.size();

				for(j=0; j < m; j++)
				{
					PDBParser::Atom* currAtom = currRes->m_Atoms.at(j);
					CCVOpenGLMath::Vector v(currAtom->m_Position[0], currAtom->m_Position[1], currAtom->m_Position[2], 1);
					CCVOpenGLMath::Vector v2;
					v2.set(phi_psi_mat * v);
					/*					if( j<3 )
										{
											printf("%s : [%6.2f,%6.2f,%6.2f] -> [%6.2f,%6.2f,%6.2f]\n",
												currAtom->getName(),
												currAtom->m_Position[0],
												currAtom->m_Position[1],
												currAtom->m_Position[2],
												v2[0],
												v2[1],
												v2[2] );
										}*/
					currAtom->m_Position[0] = v2[0];
					currAtom->m_Position[1] = v2[1];
					currAtom->m_Position[2] = v2[2];
				}
			}
		}
	}

	// The phi-psi matrix set flag only applies to a chain, so on the start of a new chain, reset it
	if(currGOA->type == PDBParser::CHAIN_TYPE)
	{
		phi_psi_mat_set = false;
	}

	// Iterate through the subgroups array and descend down the tree.
	n = (int) currGOA->m_SubGroups.size();

	for(i=0; i < n; i++)
	{
		PDBParser::GroupOfAtoms* currSubGroup = currGOA->m_SubGroups.at(i);
		twistChainRecursive(currSubGroup, pdbResSeq, phi, psi);
	}
}


// Set to true if twistAndOrReplaceResidueRecursive() finds the residue it is looking for.  Should be false, otherwise.
bool twistAndOrReplaceDone = false;

// Performs type FF_UPDATE_RESIDUE change:
// A residue specified by its first atom's sequence number may or may not be replaced with any other residue.
// Regardless, the chi angles in that residue are adjusted to the given chi angles.
void BallAndStickDataManager::twistAndOrReplaceResidueRecursive(PDBParser::GroupOfAtoms* currGOA, int pdbResSeq, int oldResId, PDBParser::GroupOfAtoms* newRes, double chi1, double chi2, double chi3, double chi4, double chi5)
{
	int i, n;

	// For chains, print out the backbone atoms instead of the bounding balls
	if(currGOA->type == PDBParser::SS_TYPE)
	{
		n = (int) currGOA->m_SubGroups.size();

		for(i=0; i < n; i++)
		{
			PDBParser::GroupOfAtoms* currRes = currGOA->m_SubGroups.at(i);

			//		go to required residue based on line number the residue ID ( each residue has the first atom's id )
			if(currRes->resPos == pdbResSeq)
			{
				// Error check, this residue name (i.e. ARG, VAL, etc) should be what we expect it to be
				if(currRes->resID != oldResId)
				{
					printf("Error: residue w/ FSN %d, name does not match given name\n", pdbResSeq);
					return;
				}

				// First replace the old residue with the new one if a new residue is provided.
				// Apply the chi angles to the new residue later.
				if(newRes != 0)
				{
					// Origin of the old residue (the location of the old CA)
					double ox, oy, oz;
					PDBParser::Residues::getRotationOrigin(currRes, " CA ", &ox, &oy, &oz);
					// Direction of the old residue (the vector from CA to CB)
					double dx, dy, dz;
					PDBParser::Residues::getRotationVector(currRes, " CA ", " CB ", &dx, &dy, &dz);
					CCVOpenGLMath::Vector old_dv(dx, dy, dz, 0);
					old_dv.normalize();
					// Assume the direction of the new residue is (0, 0, 1) and the CA is at (0, 0, 0)
					CCVOpenGLMath::Vector new_dv(0, 0, 1, 0);
					// Find the axis and angle of rotation (assuming positive rotation is clockwise, use new x old)
					CCVOpenGLMath::Vector rAxis = new_dv.cross(old_dv);
					double rAngle = acos(new_dv.dot(old_dv));
					// Find the transformation matrix.
					// Rotate the canonical residue's CA to CB bond so that it lines up with the old residue's CA to CB bond
					// Also, translate new residue's origin to the old residue's CA
					CCVOpenGLMath::Matrix rotMat = CCVOpenGLMath::Quaternion::rotation((float)rAngle, (float)rAxis[0], (float)rAxis[1], (float)rAxis[2]).buildMatrix();
					CCVOpenGLMath::Matrix fullMat = CCVOpenGLMath::Matrix::translation((float)ox, (float)oy, (float)oz) * rotMat;
					// Apply the transformation to each atom in the new residue
					int m = newRes->m_Atoms.size();
					int j;

					for(j=0; j < m; j++)
					{
						PDBParser::Atom* currAtom = newRes->m_Atoms.at(j);
						CCVOpenGLMath::Vector v((float)currAtom->m_Position[0], (float)currAtom->m_Position[1], (float)currAtom->m_Position[2], 1.0f);
						CCVOpenGLMath::Vector v3;
						v3.set(fullMat * v);
						currAtom->m_Position[0] = v3[0];
						currAtom->m_Position[1] = v3[1];
						currAtom->m_Position[2] = v3[2];
					}

					// The non backbone atoms + CA is correctly done at this point wrt rotations
					// if we apply the chi angles.
					// The canonical residue however does not have the N along the z axis etc. and applying
					// chi angles does not correct it.
					// We reuse the N and the C` atoms positions from the old residue.
					PDBParser::Atom* atomN  = PDBParser::findAtom(currRes, " N  ");
					PDBParser::Atom* atomCp = PDBParser::findAtom(currRes, " C  ");
					//Check					newRes->m_Atoms.set(atomN,  0);		// Assume: N is always at location 0
					newRes->m_Atoms[0] = atomN;
					//Check					newRes->m_Atoms.set(atomCp, 2);		// Assume: Cp is always at location 2
					newRes->m_Atoms[2] = atomCp;
					// Replace the old residue with the new one in the GOA
					//currGOA->m_SubGroups.set(newRes, i);
					currGOA->m_SubGroups[i] = newRes;
					currRes = newRes;		// Update the currRes pointer
				}

				// Verify atoms in the residue appear in the expected order
				if(!residueContentsCorrect(currRes))
				{
					printf("Residue with FSN %d did not contain the right atoms in the right order\n", pdbResSeq);
					return;
				}

				// Signal that the twist and/or replace was done
				twistAndOrReplaceDone = true;

				// apply the chi angles to the existing residue (this is always done, whether or not there is a replacement)
				//                                               Tail of vector                         Pivot atom and head of vector
				if(!PDBParser::Residues::transform(currRes, PDBParser::AA_CHI1, chi1, PDBParser::AminoAcidChi1Table[currRes->resID][2], PDBParser::AminoAcidChi1Table[currRes->resID][3]))
				{
					return;
				}

				if(!PDBParser::Residues::transform(currRes, PDBParser::AA_CHI2, chi2, PDBParser::AminoAcidChi2Table[currRes->resID][2], PDBParser::AminoAcidChi1Table[currRes->resID][3]))
				{
					return;
				}

				if(!PDBParser::Residues::transform(currRes, PDBParser::AA_CHI3, chi3, PDBParser::AminoAcidChi3Table[currRes->resID][2], PDBParser::AminoAcidChi1Table[currRes->resID][3]))
				{
					return;
				}

				if(!PDBParser::Residues::transform(currRes, PDBParser::AA_CHI4, chi4, PDBParser::AminoAcidChi4Table[currRes->resID][2], PDBParser::AminoAcidChi1Table[currRes->resID][3]))
				{
					return;
				}

				if(!PDBParser::Residues::transform(currRes, PDBParser::AA_CHI5, chi5, PDBParser::AminoAcidChi5Table[currRes->resID][2], PDBParser::AminoAcidChi1Table[currRes->resID][3]))
				{
					return;
				}
			}
		}
	}

	// Iterate through the subgroups array and descend down the tree.
	n = (int) currGOA->m_SubGroups.size();

	for(i=0; i < n; i++)
	{
		PDBParser::GroupOfAtoms* currSubGroup = currGOA->m_SubGroups.at(i);
		twistAndOrReplaceResidueRecursive(currSubGroup, pdbResSeq, oldResId, newRes, chi1, chi2, chi3, chi4, chi5);
	}
}

bool BallAndStickDataManager::evolve(const char* pdbOrPqrFileName, const char* changesFileName, const char* outputPdbOrPqrFileName)
{
	PDBParser::GroupOfAtoms* currGoa = 0;
	currGoa = readSimplePDBModels(pdbOrPqrFileName);

	if(!currGoa)
	{
		return false;
	}

	PDBParser::GroupOfAtoms* canonicalGoa = 0;
	// SKVINAY this is required if we need to rotate side chains.
	//canonicalGoa = readSimplePDBModels("../Dataset/CanonicalResidues.pdb");
	//if( !canonicalGoa ) return false;
	//	QFile changesFile( changesFileName );
	//	if( !changesFile.open( IO_ReadOnly ) ) return false;
	//	QTextStream changesFin( &changesFile );
	std::ifstream changesFin;
	changesFin.open(changesFileName, std::ios::in);

	//  go through the changes file
	//  for each change:
	while(true)
	{
		int pdbResSeq;
		int angleType;
		// The input can either be one to change phi,psi or the chi angles
		// If the first value is 0, then we are trying to change the phi psi angles
		// else, if its 1, we are trying to update the chi angles.
		changesFin >> angleType;

		if(changesFin.eof())
		{
			break;
		}

		if(angleType == FF_UPDATE_PHI_PSI)
		{
			// <sequence number> <new phi angle> <new psi angle>
			double phi = 0, psi = 0;
			changesFin >> pdbResSeq >> phi >> psi;
			double rad_phi = phi;
			double rad_psi = psi;
			// set global!!!! ugly, ugly hack.  fix someday. pcd.
			phi_psi_mat_set = false;		// may be redundant (pqr with no chains?)
			phi_psi_twist_done = false;
			twistChainRecursive(currGoa, pdbResSeq, rad_phi, rad_psi);

			// error check if the twist was done or not
			if(phi_psi_twist_done == false)
			{
				printf("Warning: phi-psi twist was not performed, res w/ FSN %d not found\n", pdbResSeq);
			}
		}
		else if(angleType == FF_UPDATE_RESIDUE)
		{
			double chi1=0;
			double chi2=0;
			double chi3=0;
			double chi4=0;
			double chi5=0;
			char oldResName[20];
			char newResName[20];
			// <sequence number> <residue old name> <residue new name> <chi1> <chi2> <chi3> <chi4> <chi5>
			changesFin >> pdbResSeq >> oldResName >> newResName >> chi1 >> chi2 >> chi3 >> chi4 >> chi5;
			// degrees to radians
			double radChi1 = ((3.14159265 * chi1) / 180.0);
			double radChi2 = ((3.14159265 * chi2) / 180.0);
			double radChi3 = ((3.14159265 * chi3) / 180.0);
			double radChi4 = ((3.14159265 * chi4) / 180.0);
			double radChi5 = ((3.14159265 * chi5) / 180.0);
			int oldResId = PDBParser::Residues::mapResidue(oldResName);
			int newResId = PDBParser::Residues::mapResidue(newResName);

			if(oldResId == PDBParser::RES_TYPE_DEFAULT || newResId == PDBParser::RES_TYPE_DEFAULT)
			{
				printf("Warning: either %s or %s is not a valid residue\n", oldResName, newResName);
				continue;
			}

			// If old and new do not match, then lookup the new residue template from the list of canonical residues
			PDBParser::GroupOfAtoms* newRes = 0;		// pointer to the residue in the canonical list (should not be assigned or modified)
			PDBParser::GroupOfAtoms* newResCopy = 0;	// a copy of the canonical residue that can be assigned and modified

			if(oldResId != newResId)
			{
				newRes = PDBParser::findResidue(canonicalGoa, newResId);
				// Make a copy of newRes
				newResCopy = new PDBParser::GroupOfAtoms(*newRes);
				newResCopy->resPos = pdbResSeq; // this is what we need to search for during later changes.
				// The canonical value is not useful, so its overwritten.
			}

			twistAndOrReplaceDone = false;
			twistAndOrReplaceResidueRecursive(currGoa, pdbResSeq, oldResId, newResCopy, radChi1, radChi2, radChi3, radChi4, radChi5);

			if(twistAndOrReplaceDone == false)
			{
				printf("Warning: replacement and/or chi change was not performed, res w/ FSN %d not found\n", pdbResSeq);
				continue;
			}

			// debug: current problems: after twistChainRecursive() and twistAndOrReplaceResidueRecursive() are done, the resulting
			// GOA is not quite right
			//  - Multiresolution atoms need to all be recalculated
			//  - m_Position arrays within the Residue Types need to be set according to the new
			//    values inside of the Atom object's m_Position arrays
			//  - all m_* arrays are possibly wrong
			// possible quick fix: dump to file, reparse
		}
		else
		{
			printf("The angle type was neither 1 nor 0\n");
			continue;
		}
	}

	changesFin.close();
	FILE* stream = fopen(outputPdbOrPqrFileName, "w");

	if(stream == 0)
	{
		return false;
	}

	PDBParser::writeGOA2PDB(stream, currGoa, PDBParser::ATOM_TYPE, 0);
	fclose(stream);
	return true;
}

bool BallAndStickDataManager::writePDB(const char* inputFileName, const char* outputFileName, int outputLevel)
{
	// get the molecule
	PDBParser::GroupOfAtoms* currGoa = readSimplePDBModels(inputFileName);
	// write the output
	FILE* stream = fopen(outputFileName, "w");

	if(stream == 0)
	{
		delete currGoa;
		currGoa = 0;
		return false;
	}

	PDBParser::writeGOA2PDB(stream, currGoa, outputLevel, 0);
	fclose(stream);
	delete currGoa;
	currGoa = 0;
	return true;
}

bool BallAndStickDataManager::writeGOA(const char* inputFileName, const char* outputFileName)
{
	// get the molecule
	PDBParser::GroupOfAtoms* currGoa = readSimplePDBModels(inputFileName);
	// write the output
	FILE* stream = fopen(outputFileName, "w");

	if(stream == 0)
	{
		delete currGoa;
		currGoa = 0;
		return false;
	}

	PDBParser::writeGOAtoFile(stream, currGoa);
	fclose(stream);
	delete currGoa;
	currGoa = 0;
	return true;
}

void BallAndStickDataManager::generateChainsCopy(PDBParser::GroupOfAtoms* newMolecule, PDBParser::GroupOfAtoms* molecule)
{
	if(molecule->type == PDBParser::CHAIN_TYPE)
	{
		// make 60 copies, transform, add to new molecule, return
		int j;

		for(j=0; j<60; j++)
		{
			double transformation[9];
			transformation[0] = PDBParser::virusTransformations[j*9+0];
			transformation[1] = PDBParser::virusTransformations[j*9+1];
			transformation[2] = PDBParser::virusTransformations[j*9+2];
			transformation[3] = PDBParser::virusTransformations[j*9+3];
			transformation[4] = PDBParser::virusTransformations[j*9+4];
			transformation[5] = PDBParser::virusTransformations[j*9+5];
			transformation[6] = PDBParser::virusTransformations[j*9+6];
			transformation[7] = PDBParser::virusTransformations[j*9+7];
			transformation[8] = PDBParser::virusTransformations[j*9+8];
			PDBParser::GroupOfAtoms* chain = new PDBParser::GroupOfAtoms(*molecule);
			chain->transformAtoms(transformation);
			newMolecule->addChain(chain);
		}

		return;
	}

	int i, m;
	m = (int) molecule->m_SubGroups.size();

	for(i = 0; i < m; i++)
	{
		PDBParser::GroupOfAtoms* currSubGroup = molecule->m_SubGroups.at(i);
		generateChainsCopy(newMolecule, currSubGroup);
	}
}

PDBParser::GroupOfAtoms* BallAndStickDataManager::generateFullVirus(PDBParser::GroupOfAtoms* molecule)
{
	// 1. take each chain
	// 2. for each transformation
	//		make a copy of chain, rotate it, add to new molecule.
	PDBParser::GroupOfAtoms* newMolecule = new PDBParser::GroupOfAtoms();
	// step 1:
	generateChainsCopy(newMolecule, molecule);
	return newMolecule;
}

// Print out as much as possible from the pdb, including its header
//	The print type tell us whether to
//   		1. -1 -> print to stdout
//		2. 0  -> write to new file
//		3. 1  -> append to existing file
bool BallAndStickDataManager::printPDBInformation(const char* pdbOrPqrFileName, const char* outputFileName, int printType)
{
	if(!pdbOrPqrFileName)
	{
		return false;
	}

	if(printType != -1 && !outputFileName)
	{
		return false;
	}

	FILE* fpOut = 0;

	if(printType == 0)
	{
		fpOut = fopen(outputFileName, "w");

		if(!fpOut)
		{
			return false;
		}
	}
	else if(printType == 1)
	{
		fpOut = fopen(outputFileName, "a");

		if(!fpOut)
		{
			return false;
		}
	}
	else if(printType == -1)
	{
		fpOut = stdout;

		if(!fpOut)
		{
			return false;
		}
	}

	PDBParser::GroupOfAtoms* molecule = readSimplePDBModels(pdbOrPqrFileName);

	if(printType != -1)
	{
		fclose(fpOut);
	}

	delete molecule;
	molecule = 0;
	return true;
}


// Print out the maximum distance of an atom from the given point to a file.
// Depending on the last parameter, append or create a new file.
bool BallAndStickDataManager::getMaxDistanceFromPoint(const char* pdbOrPqrFileName, const char* outputFileName, double xOrigin, double yOrigin, double zOrigin, bool appendToFile)
{
	if(!pdbOrPqrFileName || !outputFileName)
	{
		return false;
	}

	FILE* fpOut = 0;
	PDBParser::GroupOfAtoms* molecule = readSimplePDBModels(pdbOrPqrFileName);

	if(!molecule)
	{
		return false;
	}

	if(appendToFile)
	{
		fpOut = fopen(outputFileName, "a");
	}
	else
	{
		fpOut = fopen(outputFileName, "w");
	}

	if(!fpOut)
	{
		return false;
	}

	double distance = 0;
	molecule->getMaximumDistanceFromPoint(xOrigin, yOrigin, zOrigin, &distance);
	fprintf(fpOut, "%s %lf\n", pdbOrPqrFileName, distance);
	fclose(fpOut);
	delete molecule;
	return true;
}
