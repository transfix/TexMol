#include <Blurmaps/BlurMapsDataManager.h>
#include <Blurmaps/GOABlur.h>
#include <Docking/Complex.h>
#include <Docking/Interface.h>
#include <Geometry/Geometry.h>
#include <iostream>
#include <math.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <SimpleVolumeData/SimpleVolumeDataIsocontourer.h>

using namespace Docking;
//using PDBParser::Atom;

Interface::Interface()
{
}

Interface::~Interface()
{
}

bool Interface::markRelevantTriangles(Geometry* surface, std::vector<int> *relevantTriangles, SimpleVolumeData* sData, double minFuncVal)
{
	if (!surface || !relevantTriangles || !sData)
	{
		return false;
	}
	//		go through all triangles
	//	    if triangle inside box
	//            mark as relevant
	//      end if
	for (int i=0; i<surface->m_NumTris; i++)
	{
		// triangle is much smaller than the box
		// so we dont need any expensive accurate intersection check.
		for (int j=0; j<3; j++)
		{
			int v = surface->m_Tris[3*i+j];
			float x = surface->m_TriVerts[3*v+0];
			float y = surface->m_TriVerts[3*v+1];
			float z = surface->m_TriVerts[3*v+2];
			if (sData->getValueAt(0, x, y, z) >= minFuncVal)
			{
				relevantTriangles->push_back(i);
				break;
			}
		}
	}
	return true;
}

double Interface::determinant(float a1, float b1,    // 1
							  float a2, float b2,   // 1
							  float a3, float b3)   // 1
{
	return a1*(b2 - b3) - b1*(a2 - a3) + a2*b3 - a3*b2;
}

double Interface::triArea(Geometry* surface, int idx1, int idx2, int idx3)
{
	float x1 = surface->m_TriVerts[3*idx1+0];
	float y1 = surface->m_TriVerts[3*idx1+1];
	float z1 = surface->m_TriVerts[3*idx1+2];
	float x2 = surface->m_TriVerts[3*idx2+0];
	float y2 = surface->m_TriVerts[3*idx2+1];
	float z2 = surface->m_TriVerts[3*idx2+2];
	float x3 = surface->m_TriVerts[3*idx3+0];
	float y3 = surface->m_TriVerts[3*idx3+1];
	float z3 = surface->m_TriVerts[3*idx3+2];
	double det1 = determinant(y1, z1, y2, z2, y3, z3);
	double det2 = determinant(z1, x1, z2, x2, z3, x3);
	double det3 = determinant(x1, y1, x2, y2, x3, y3);
	return 1.0/2.0*sqrt(det1*det1 + det2*det2 + det3*det3);
}

bool Interface::getInterfaceMarkedGeometry(Geometry** surface1, Geometry** surface2,
		Geometry** surface1blue, Geometry** surface2blue,
		Geometry** surface1NoInterface, Geometry** surface2NoInterface,
		Geometry** interface1, Geometry** interface2,
		DockComplex* complex, double distanceCutOff)
{
	if (!complex)
	{
		return 0;
	}
	if (!complex->getProtein() || !complex->getLigand())
	{
		return 0;
	}
	// blur both
	// isocontour both
	// int dim1 =64, dim2 =64, dim3 =64;
	int dim1 = 128, dim2 = 128, dim3 = 128;
	double isovalue = 1;
	double blobbiness = -2.3;
	// create the volumes //
	SimpleVolumeData* sData1 = 0;
	SimpleVolumeData* sData1Hydro = 0;
	{
		sData1 = BlurMapsDataManager::getVolume(complex->getProtein(), "",
												dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
												false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
		sData1Hydro = BlurMapsDataManager::getVolume(complex->getProtein(), "",
					  dim1, dim2, dim3, PDBParser::GroupOfAtoms::PER_ATOM_HYDROPHOBICITY,
					  false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
		if (!sData1)
		{
			return 0;
		}
	}
	SimpleVolumeData* sData2 = 0;
	SimpleVolumeData* sData2Hydro = 0;
	{
		sData2 = BlurMapsDataManager::getVolume(complex->getLigand(), "",
												dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
												false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
		sData2Hydro = BlurMapsDataManager::getVolume(complex->getLigand(), "",
					  dim1, dim2, dim3, PDBParser::GroupOfAtoms::PER_ATOM_HYDROPHOBICITY,
					  false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
		if (!sData2)
		{
			return 0;
		}
	}
	// create the isosurface //
	std::cerr << "Isosurfacing...\n";
	(*surface1) = SimpleVolumeDataIsocontourer::getIsocontour(sData1, isovalue);
	(*surface2) = SimpleVolumeDataIsocontourer::getIsocontour(sData2, isovalue);
	// build hydrophobicity info
	double surface1Hydro[(*surface1)->m_NumTriVerts];
	double surface2Hydro[(*surface2)->m_NumTriVerts];
	SimpleVolumeDataIsocontourer::getFunctionValues(sData1Hydro, (*surface1), surface1Hydro);
	SimpleVolumeDataIsocontourer::getFunctionValues(sData2Hydro, (*surface2), surface2Hydro);
	const float maxHydroColor = .5f;
	const float minHydroColor = .25f;
	float r,g,b;
	std::vector<int> relevantTriangles1;
	std::vector<int> relevantTriangles2;
	double minFuncVal = exp(blobbiness*distanceCutOff*distanceCutOff);
	markRelevantTriangles(*surface1, &relevantTriangles1, sData2, minFuncVal);
	markRelevantTriangles(*surface2, &relevantTriangles2, sData1, minFuncVal);
	(*surface1NoInterface) = new Geometry(**surface1);
	(*surface2NoInterface) = new Geometry(**surface2);
	(*surface1blue) = new Geometry(**surface1);
	(*surface2blue) = new Geometry(**surface2);
	for (int i =0; i<relevantTriangles1.size(); i++)
	{
		const int triangleIndex = relevantTriangles1[i];
		for (int j=0; j<3; j++)
		{
			const int vertexIndex = (*surface1)->m_Tris[3*triangleIndex + j];
			(*surface1blue)->m_TriVertColorsTransparent[4*vertexIndex + 0] = 0;
			(*surface1blue)->m_TriVertColorsTransparent[4*vertexIndex + 1] = 0;
			(*surface1blue)->m_TriVertColorsTransparent[4*vertexIndex + 2] = 1;
			(*surface1blue)->m_TriVertColorsTransparent[4*vertexIndex + 3] = 1;
		}
	}
	for (int i =0; i<relevantTriangles2.size(); i++)
	{
		const int triangleIndex = relevantTriangles2[i];
		for (int j=0; j<3; j++)
		{
			const int vertexIndex = (*surface2)->m_Tris[3*triangleIndex + j];
			(*surface2blue)->m_TriVertColorsTransparent[4*vertexIndex + 0] = 0;
			(*surface2blue)->m_TriVertColorsTransparent[4*vertexIndex + 1] = 1;
			(*surface2blue)->m_TriVertColorsTransparent[4*vertexIndex + 2] = 0;
			(*surface2blue)->m_TriVertColorsTransparent[4*vertexIndex + 3] = 1;
		}
	}
	// now set interfaces to a certain color (this one is hydrophobicity)
	for (int i =0; i<relevantTriangles1.size(); i++)
	{
		const int triangleIndex = relevantTriangles1[i];
		for (int j=0; j<3; j++)
		{
			const int vertexIndex = (*surface1)->m_Tris[3*triangleIndex + j];
			r = g = b = maxHydroColor;
			double v = surface1Hydro[vertexIndex];
			if (v < 0)
			{
				g = maxHydroColor + v*5;
				b = maxHydroColor + v*5;
				if (g < minHydroColor)
				{
					g = minHydroColor;
				}
				if (b < minHydroColor)
				{
					b = minHydroColor;
				}
				if (v < -0.2)
				{
					g = minHydroColor;
					b = minHydroColor;
				}
			}
			if (v > 0)
			{
				r = maxHydroColor - v*5 + minHydroColor;
				b = maxHydroColor - v*5 + minHydroColor;
				if (r < minHydroColor)
				{
					r = minHydroColor;
				}
				if (b < minHydroColor)
				{
					b = minHydroColor;
				}
				if (v > 0.2)
				{
					r = minHydroColor;
					b = minHydroColor;
				}
			}
			(*surface1)->m_TriVertColorsTransparent[4*vertexIndex+0] = r;
			(*surface1)->m_TriVertColorsTransparent[4*vertexIndex+1] = g;
			(*surface1)->m_TriVertColorsTransparent[4*vertexIndex+2] = b;
			(*surface1)->m_TriVertColorsTransparent[4*vertexIndex+3] = 1.0f;
		}
	}
	for (int i =0; i<relevantTriangles2.size(); i++)
	{
		const int triangleIndex = relevantTriangles2[i];
		for (int j=0; j<3; j++)
		{
			const int vertexIndex = (*surface2)->m_Tris[3*triangleIndex + j];
			r = g = b = maxHydroColor;
			double v = surface2Hydro[vertexIndex];
			if (v < 0)
			{
				g = maxHydroColor + v*5 + minHydroColor;
				b = maxHydroColor + v*5 + minHydroColor;
				if (g < minHydroColor)
				{
					g = minHydroColor;
				}
				if (b < minHydroColor)
				{
					b = minHydroColor;
				}
				if (v < -0.2)
				{
					g = minHydroColor;
					b = minHydroColor;
				}
			}
			if (v > 0)
			{
				r = maxHydroColor - v*5 + minHydroColor;
				b = maxHydroColor - v*5 + minHydroColor;
				if (r < minHydroColor)
				{
					r = minHydroColor;
				}
				if (b < minHydroColor)
				{
					b = minHydroColor;
				}
				if (v > 0.2)
				{
					r = minHydroColor;
					b = minHydroColor;
				}
			}
			(*surface2)->m_TriVertColorsTransparent[4*vertexIndex+0] = r;
			(*surface2)->m_TriVertColorsTransparent[4*vertexIndex+1] = g;
			(*surface2)->m_TriVertColorsTransparent[4*vertexIndex+2] = b;
			(*surface2)->m_TriVertColorsTransparent[4*vertexIndex+3] = 1.0f;
		}
	}
	// interfaces have all the same points and colors, but we reduce the triangle list to only
	// relevantTriangles
	(*interface1) = new Geometry();
	(*interface1)->AllocateTris((*surface1blue)->m_NumTriVerts, relevantTriangles1.size());
	(*interface1)->AllocateTriVertColors();
	// set triangles
	for (int i=0; i<relevantTriangles1.size(); i++)
	{
		const int triangleIndex = relevantTriangles1[i];
		(*interface1)->m_Tris[3*i + 0] = (*surface1blue)->m_Tris[3*triangleIndex + 0];
		(*interface1)->m_Tris[3*i + 1] = (*surface1blue)->m_Tris[3*triangleIndex + 1];
		(*interface1)->m_Tris[3*i + 2] = (*surface1blue)->m_Tris[3*triangleIndex + 2];
	}
	// copy verts
	for (int c=0; c<(*surface1blue)->m_NumTriVerts*3; c++)   // for every coordinate of every TriVert
	{
		(*interface1)->m_TriVerts[c] = (*surface1blue)->m_TriVerts[c];
	}
	// copy normals
	for (int c=0; c<(*surface1blue)->m_NumTriVerts*3; c++)   // for every coordinate of every TriVertNormal
	{
		(*interface1)->m_TriVertNormals[c] = (*surface1blue)->m_TriVertNormals[c];
	}
	// copy colors
	for (int c=0; c<(*surface1blue)->m_NumTriVerts*4; c++)
	{
		(*interface1)->m_TriVertColorsTransparent[c] = (*surface1blue)->m_TriVertColorsTransparent[c];
	}
	(*interface2) = new Geometry();
	(*interface2)->AllocateTris((*surface2blue)->m_NumTriVerts, relevantTriangles2.size());
	(*interface2)->AllocateTriVertColors();
	// set triangles
	for (int i=0; i<relevantTriangles2.size(); i++)
	{
		const int triangleIndex = relevantTriangles2[i];
		(*interface2)->m_Tris[3*i + 0] = (*surface2blue)->m_Tris[3*triangleIndex + 0];
		(*interface2)->m_Tris[3*i + 1] = (*surface2blue)->m_Tris[3*triangleIndex + 1];
		(*interface2)->m_Tris[3*i + 2] = (*surface2blue)->m_Tris[3*triangleIndex + 2];
	}
	// copy verts
	for (int c=0; c<(*surface2blue)->m_NumTriVerts*3; c++)   // for every coordinate of every TriVert
	{
		(*interface2)->m_TriVerts[c] = (*surface2blue)->m_TriVerts[c];
	}
	// copy normals
	for (int c=0; c<(*surface2blue)->m_NumTriVerts*3; c++)   // for every coordinate of every TriVertNormal
	{
		(*interface2)->m_TriVertNormals[c] = (*surface2blue)->m_TriVertNormals[c];
	}
	// copy colors
	for (int c=0; c<(*surface2blue)->m_NumTriVerts*4; c++)
	{
		(*interface2)->m_TriVertColorsTransparent[c] = (*surface2blue)->m_TriVertColorsTransparent[c];
	}
}

double Interface::getArea(DockComplex* complex, double distanceCutOff)
{
	if (!complex)
	{
		return 0;
	}
	if (!complex->getProtein() || !complex->getLigand())
	{
		return 0;
	}
	// blur both
	// isocontour both
	int dim1 =64, dim2 =64, dim3 =64;
	double isovalue = 1;
	double blobbiness = -2.3;
	// create the volumes //
	SimpleVolumeData* sData1 = 0;
	{
		sData1 = BlurMapsDataManager::getVolume(complex->getProtein(), "",
												dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
												false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
		if (!sData1)
		{
			return 0;
		}
	}
	SimpleVolumeData* sData2 = 0;
	{
		sData2 = BlurMapsDataManager::getVolume(complex->getLigand(), "",
												dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
												false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
		if (!sData2)
		{
			return 0;
		}
	}
	//
	// create the isosurface //
	Geometry* surface1 = SimpleVolumeDataIsocontourer::getIsocontour(sData1, isovalue);
	Geometry* surface2 = SimpleVolumeDataIsocontourer::getIsocontour(sData2, isovalue);
	// keep indices of tri1 which are close to protein2 and vice versa
	double min1[3], max1[3];
	double min2[3], max2[3];
	surface1->getExtents(&(min1[0]), &(min1[1]), &(min1[2]), &(max1[0]), &(max1[1]), &(max1[2]));
	surface2->getExtents(&(min2[0]), &(min2[1]), &(min2[2]), &(max2[0]), &(max2[1]), &(max2[2]));
	std::vector<int> relevantTriangles1;
	std::vector<int> relevantTriangles2;
	// find function value distanceCutoff away due to atom of radius 1
	double minFuncVal = exp(blobbiness*distanceCutOff*distanceCutOff);
	markRelevantTriangles(surface1, &relevantTriangles1, sData2, minFuncVal);
	markRelevantTriangles(surface2, &relevantTriangles2, sData1, minFuncVal);
	{
		int n1 = surface1->m_NumTris;
		int n2 = relevantTriangles1.size();
		int n3 = surface2->m_NumTris;
		int n4 = relevantTriangles2.size();
		printf("%d %d %d %d\n", n1, n2, n3, n4);
	}
	// n_relevant^2 loop to find all interface triangles
	bool* interfaceTriangles2 = new bool[relevantTriangles2.size()];
	double area = 0;
	for (int i=0; i<relevantTriangles1.size(); i++)
	{
		int p10 = surface1->m_Tris[3*relevantTriangles1[i]+0];
		int p11 = surface1->m_Tris[3*relevantTriangles1[i]+1];
		int p12 = surface1->m_Tris[3*relevantTriangles1[i]+2];
		for (int j=0; j<relevantTriangles2.size(); j++)
		{
			int p20 = surface2->m_Tris[3*relevantTriangles2[j]+0];
			int p21 = surface2->m_Tris[3*relevantTriangles2[j]+1];
			int p22 = surface2->m_Tris[3*relevantTriangles2[j]+2];
			// get distance
			double dist = Geometry::getHausdorffDistance(&(surface1->m_TriVerts[3*p10+0]),
						  &(surface1->m_TriVerts[3*p11+1]),
						  &(surface1->m_TriVerts[3*p12+2]),
						  &(surface2->m_TriVerts[3*p20+0]),
						  &(surface2->m_TriVerts[3*p21+1]),
						  &(surface2->m_TriVerts[3*p22+2]));
			// if close,
			//		mark second as interface triangle
			//      add area of first to total,
			//      add area of second, making sure to do it just once.
			// end if
			if (dist <= distanceCutOff)
			{
				area += triArea(surface1, p10, p11, p12);
				if (!interfaceTriangles2[j])
				{
					area += triArea(surface2, p20, p21, p22);
				}
				interfaceTriangles2[j] = true;
			}
		}
	}
	delete sData1;
	delete sData2;
	delete []interfaceTriangles2;
	interfaceTriangles2 = 0;
	return area/2.0;
}

double Interface::getArea(std::vector<PDBParser::Atom*> molecule1, std::vector<PDBParser::Atom*> molecule2, double distanceCutOff)
{
	if (molecule1.size() < 1 || molecule2.size() < 1)
	{
		return 0;
	}
	// blur both
	// isocontour both
	int dim1 =64, dim2 =64, dim3 =64;
	double isovalue = 1;
	double blobbiness = -2.3;
	// create the volumes //
	SimpleVolumeData* sData1 = new SimpleVolumeData(dim1, dim2, dim3);
	{
		GOABlur* goaBlur = new GOABlur();
		goaBlur->setBlobbyness(blobbiness);
		if (!goaBlur->generateBlurmap2(molecule1, sData1, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
									   "", false, PDBParser::GroupOfAtoms::ATOM, PDBParser::GroupOfAtoms::VDW_RADIUS, 0))
		{
			delete sData1;
			return 0;
		}
		delete goaBlur;
	}
	SimpleVolumeData* sData2 = new SimpleVolumeData(dim1, dim2, dim3);
	{
		GOABlur* goaBlur = new GOABlur();
		goaBlur->setBlobbyness(blobbiness);
		if (!goaBlur->generateBlurmap2(molecule2, sData2, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
									   "", false, PDBParser::GroupOfAtoms::ATOM, PDBParser::GroupOfAtoms::VDW_RADIUS, 0))
		{
			delete sData2;
			delete sData1;
			return 0;
		}
		delete goaBlur;
	}
	//
	// create the isosurface //
	Geometry* surface1 = SimpleVolumeDataIsocontourer::getIsocontour(sData1, isovalue);
	Geometry* surface2 = SimpleVolumeDataIsocontourer::getIsocontour(sData2, isovalue);
	// keep indices of tri1 which are close to protein2 and vice versa
	double min1[3], max1[3];
	double min2[3], max2[3];
	surface1->getExtents(&(min1[0]), &(min1[1]), &(min1[2]), &(max1[0]), &(max1[1]), &(max1[2]));
	surface2->getExtents(&(min2[0]), &(min2[1]), &(min2[2]), &(max2[0]), &(max2[1]), &(max2[2]));
	std::vector<int> relevantTriangles1;
	std::vector<int> relevantTriangles2;
	// find function value distanceCutoff away due to atom of radius 1
	double minFuncVal = exp(blobbiness*distanceCutOff*distanceCutOff);
	markRelevantTriangles(surface1, &relevantTriangles1, sData2, minFuncVal);
	markRelevantTriangles(surface2, &relevantTriangles2, sData1, minFuncVal);
	{
		int n1 = surface1->m_NumTris;
		int n2 = relevantTriangles1.size();
		int n3 = surface2->m_NumTris;
		int n4 = relevantTriangles2.size();
		printf("%d %d %d %d\n", n1, n2, n3, n4);
	}
	// n_relevant^2 loop to find all interface triangles
	bool* interfaceTriangles2 = new bool[relevantTriangles2.size()];
	double area = 0;
	for (int i=0; i<relevantTriangles1.size(); i++)
	{
		int p10 = surface1->m_Tris[3*relevantTriangles1[i]+0];
		int p11 = surface1->m_Tris[3*relevantTriangles1[i]+1];
		int p12 = surface1->m_Tris[3*relevantTriangles1[i]+2];
		for (int j=0; j<relevantTriangles2.size(); j++)
		{
			int p20 = surface2->m_Tris[3*relevantTriangles2[j]+0];
			int p21 = surface2->m_Tris[3*relevantTriangles2[j]+1];
			int p22 = surface2->m_Tris[3*relevantTriangles2[j]+2];
			// get distance
			double dist = Geometry::getHausdorffDistance(&(surface1->m_TriVerts[3*p10+0]),
						  &(surface1->m_TriVerts[3*p11+1]),
						  &(surface1->m_TriVerts[3*p12+2]),
						  &(surface2->m_TriVerts[3*p20+0]),
						  &(surface2->m_TriVerts[3*p21+1]),
						  &(surface2->m_TriVerts[3*p22+2]));
			// if close,
			//		mark second as interface triangle
			//      add area of first to total,
			//      add area of second, making sure to do it just once.
			// end if
			if (dist <= distanceCutOff)
			{
				area += triArea(surface1, p10, p11, p12);
				if (!interfaceTriangles2[j])
				{
					area += triArea(surface2, p20, p21, p22);
				}
				interfaceTriangles2[j] = true;
			}
		}
	}
	delete sData1;
	delete sData2;
	delete []interfaceTriangles2;
	interfaceTriangles2 = 0;
	return area/2.0;
}
