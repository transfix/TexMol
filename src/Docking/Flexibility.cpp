#include <Docking/Connector.h>
#include <Docking/DomainComplex.h>
#include <Docking/Domain.h>
#include <Docking/Flexibility.h>
#include <Docking/FlexibleLoop.h>
#include <Docking/Interface.h>
#include <Docking/Segment.h>
#include <PDBParser/Atom.h>
#include <UsefulMath/Quaternion.h>
#include <Utility/utility.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using PDBParser::GroupOfAtoms;
using PDBParser::Atom;
using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;
using CCVOpenGLMath::Quaternion;
using Docking::DomainComplex;
using Docking::Domain;
using Docking::FlexibleLoop;
using Docking::Flexibility;
using Docking::Interface;
using Docking::Connector;

Flexibility::Flexibility(DomainComplex* domainComplex, Domain* d1, Domain* d2)
{
	m_DomainComplex = domainComplex;
	m_Domain1 = d1;
	m_Domain2 = d2;
	m_ImportantConnectorIndex = -1;
	initValues();
	m_TotalDiscreteConformations = 2;
}

Flexibility::~Flexibility()
{
}

void Flexibility::initValues()
{
	m_Area = m_ShearMagnitude = m_MinBendingRange = m_MaxBendingRange = m_SecondaryMinBendingRange = m_SecondaryMaxBendingRange = m_MinTwistingRange = m_MaxTwistingRange = 0;
	m_Shear = m_Bend = m_SecondaryBend = m_Twist = false;
}

bool Flexibility::computeInterfaceArea()
{
	// get density of one, surface of another.
	// find all triangles within some distance
	// sum area of all those triangles
	Interface* interfac = new Interface();
	std::vector<Atom*> atomList1;
	m_Domain1->getAtoms(&atomList1);
	std::vector<Atom*> atomList2;
	m_Domain2->getAtoms(&atomList2);
	m_Area = interfac->getArea(atomList1, atomList2, 2);
	{
		for (int i=0; i<atomList1.size(); i++)
		{
			delete atomList1[i];
		}
		atomList1.clear();
		for (int i=0; i<atomList2.size(); i++)
		{
			delete atomList2[i];
		}
		atomList2.clear();
	}
	delete interfac;
	return true;
}

bool Flexibility::readShear(FILE* fp)
{
	float pnx, pny, pnz, pnw;
	int shearOrNot;
	if (fscanf(fp, "Shear: %d Normal [%f %f %f %f], disp %lf\n",
			   &shearOrNot, &pnx, &pny, &pnz, &pnw, &m_ShearMagnitude) != 6)
	{
		return false;
	}
	if (shearOrNot)
	{
		m_Shear = true;
	}
	else
	{
		m_Shear = false;
	}
	m_PlaneNormal.set(pnx, pny, pnz, pnw);
	return true;
}

bool Flexibility::printShear(FILE* fp)
{
	fprintf(fp, "Shear: %d Normal [%f %f %f %f], disp %lf\n",
			m_Shear, m_PlaneNormal[0], m_PlaneNormal[1], m_PlaneNormal[2], m_PlaneNormal[3],
			m_ShearMagnitude);
	return true;
}

bool Flexibility::readBending(FILE* fp)
{
	float bax, bay, baz, baw, bpx, bpy, bpz, bpw;
	int mBendOrNot;
	if (fscanf(fp, "Bending: %d Axis [%f %f %f %f], Point [%f %f %f %f], disp [%lf %lf]\n",
			   &mBendOrNot, &bax, &bay, &baz, &baw, &bpx, &bpy, &bpz, &bpw, &m_MinBendingRange, &m_MaxBendingRange) != 11)
	{
		return false;
	}
	if (mBendOrNot)
	{
		m_Bend = true;
	}
	else
	{
		m_Bend = false;
	}
	m_BendingAxis.set(bax, bay, baz, baw);
	m_BendingAxisHingePoint.set(bpx, bpy, bpz, bpw);
	return true;
}

bool Flexibility::printBending(FILE* fp)
{
	fprintf(fp, "Bending: %d Axis [%f %f %f %f], Point [%f %f %f %f], disp [%lf %lf]\n",
			m_Bend, m_BendingAxis[0], m_BendingAxis[1], m_BendingAxis[2], m_BendingAxis[3],
			m_BendingAxisHingePoint[0], m_BendingAxisHingePoint[1], m_BendingAxisHingePoint[2], m_BendingAxisHingePoint[3],
			m_MinBendingRange, m_MaxBendingRange);
	return true;
}

bool Flexibility::readSecondaryBending(FILE* fp)
{
	float bax, bay, baz, baw, bpx, bpy, bpz, bpw;
	int sbendOrNot;
	if (fscanf(fp, "Secondary bending: %d Axis [%f %f %f %f], Point [%f %f %f %f], disp [%lf %lf]\n",
			   &sbendOrNot, &bax, &bay, &baz, &baw, &bpx, &bpy, &bpz, &bpw, &m_SecondaryMinBendingRange, &m_SecondaryMaxBendingRange) != 11)
	{
		return false;
	}
	if (sbendOrNot)
	{
		m_SecondaryBend = true;
	}
	else
	{
		m_SecondaryBend = false;
	}
	m_SecondaryBendingAxis.set(bax, bay, baz, baw);
	m_SecondaryBendingAxisHingePoint.set(bpx, bpy, bpz, bpw);
	return true;
}

bool Flexibility::printSecondaryBending(FILE* fp)
{
	fprintf(fp, "Secondary bending: %d Axis [%f %f %f %f], Point [%f %f %f %f], disp [%lf %lf]\n",
			m_SecondaryBend, m_SecondaryBendingAxis[0], m_SecondaryBendingAxis[1], m_SecondaryBendingAxis[2], m_SecondaryBendingAxis[3],
			m_SecondaryBendingAxisHingePoint[0], m_SecondaryBendingAxisHingePoint[1], m_SecondaryBendingAxisHingePoint[2], m_SecondaryBendingAxisHingePoint[3],
			m_SecondaryMinBendingRange, m_SecondaryMaxBendingRange);
	return true;
}

bool Flexibility::readTwist(FILE* fp)
{
	int twistOrNot;
	if (fscanf(fp, "Twist: %d disp [%lf %lf]\n",
			   &twistOrNot, &m_MinTwistingRange, &m_MaxTwistingRange) != 3)
	{
		return false;
	}
	if (twistOrNot)
	{
		m_Twist = true;
	}
	else
	{
		m_Twist = false;
	}
	return true;
}

bool Flexibility::printTwist(FILE* fp)
{
	fprintf(fp, "Twist: %d disp [%lf %lf]\n",
			m_Twist, m_MinTwistingRange, m_MaxTwistingRange);
	return true;
}

bool Flexibility::print(FILE* fp)
{
	int cut = m_Cut ? 1 : 0;
	fprintf(fp, "Domains %d %d\n", m_Domain1->m_Id, m_Domain2->m_Id);
	fprintf(fp, "Area %lf\n", m_Area);
	fprintf(fp, "ImportantConnector %d\n", m_ImportantConnectorIndex);
	fprintf(fp, "Cut %d\n", cut);
	if (m_Cut)
	{
		fprintf(fp, "Right side domains %lu: ", m_RightDomains.size());
		for (int i=0; i<m_RightDomains.size(); i++)
		{
			fprintf(fp, " %d ", m_RightDomains[i]);
		}
		fprintf(fp, "\n");
	}
	if (!printShear(fp))
	{
		return false;
	}
	if (!printBending(fp))
	{
		return false;
	}
	if (!printSecondaryBending(fp))
	{
		return false;
	}
	if (!printTwist(fp))
	{
		return false;
	}
	return true;
}

bool Flexibility::read(FILE* fp)
{
	int cut = 0;
	if (fscanf(fp, "Area %lf\n", &m_Area) != 1)
	{
		return false;
	}
	fscanf(fp, "ImportantConnector %d\n", &m_ImportantConnectorIndex);
	fscanf(fp, "Cut %d\n", &cut);
	m_Cut = cut ? true : false;
	if (m_Cut)
	{
		int size;
		fscanf(fp, "Right side domains %d: ", &size);
		for (int i=0; i<size; i++)
		{
			int id;
			fscanf(fp, " %d ", &id);
			m_RightDomains.push_back(id);
		}
		fscanf(fp, "\n");
	}
	if (!readShear(fp))
	{
		return false;
	}
	if (!readBending(fp))
	{
		return false;
	}
	if (!readSecondaryBending(fp))
	{
		return false;
	}
	if (!readTwist(fp))
	{
		return false;
	}
	return true;
}

bool Flexibility::computeFlexibility()
{
	// if area above threshold, add shear, range should depend on length of smallest connector, plane given by best fit of interface
	if (!computeShearProperties())
	{
		return false;
	}
	// for hinge, twist, use bond angle and torsion angles. Sample them in correct directions to produce desired effect!
	// depending on area, whether its a cut or not and number of connectors, give ranges
	// compute main axis for bending angle (hinge)
	if (!computeBending())
	{
		return false;
	}
	// compute secondary axis for bending angle (hinge)
	if (!computeSecondaryBendingAxis())
	{
		return false;
	}
	// compute perp axis for torsion angle (twist)
	if (!computeTwistingAxis())
	{
		return false;
	}
	return true;
}

int Flexibility::getNumberOfConnectors()
{
	int numConnectors = 0;
	// go through all connectors, find how many at this level and between these 2 domains.
	for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
	{
		Connector* c = m_DomainComplex->m_Connectors[i];
		if (!((c->m_Level == 0 && c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id) ||
				(c->m_Level == 0 && c->m_Domain1 == m_Domain2->m_Id && c->m_Domain2 == m_Domain1->m_Id)))
		{
			continue;
		}
		numConnectors++;
	}
	return numConnectors;
}

float Flexibility::get3DDistance(Vector v1, Vector v2)
{
	return sqrt(fabs((v1[0]-v2[0])*(v1[0]-v2[0]) +
					 (v1[1]-v2[1])*(v1[1]-v2[1]) +
					 (v1[2]-v2[2])*(v1[2]-v2[2])));
}

float Flexibility::getSmallestConnectorLength()
{
	float minLen = 100000000;
	for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
	{
		Connector* c = m_DomainComplex->m_Connectors[i];
		if (!((c->m_Level == 0 && c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id) ||
				(c->m_Level == 0 && c->m_Domain1 == m_Domain2->m_Id && c->m_Domain2 == m_Domain1->m_Id)))
		{
			continue;
		}
		Vector p1, p2;
		if (!c->computeEndPoints(&p1, &p2))
		{
			return false;
		}
		float len = get3DDistance(p1, p2);
		if (len < minLen)
		{
			minLen = len;
		}
	}
	minLen += 3; // account for others in backbone
	return minLen;
}

bool Flexibility::computeShearProperties()
{
	m_Shear = false;
	if (m_Area + getNumberOfConnectors()*AREA_WEIGHT_OF_CONNECTOR < SHEAR_AREA_THRESHOLD)
	{
		return true;
	}
	// if shear, compute the centers, find direction of shear.
	// magnitude is given by 0.25 x length of smallest connector, and limited by a max value.
	m_ShearMagnitude = MAX_SHEAR;
	float lengthSmallestConnector = getSmallestConnectorLength();
	if (lengthSmallestConnector*0.25 < m_ShearMagnitude)
	{
		m_ShearMagnitude = lengthSmallestConnector*0.25;
	}
	// if there is no connector, shear in plane perp to line connecting centers.
	m_Shear = true;
	return true;
}

int Flexibility::getNumberOfResidueInShortestConnector()
{
	int minLen = 1000000;
	for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
	{
		Connector* c = m_DomainComplex->m_Connectors[i];
		if (!((c->m_Level == 0 && c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id) ||
				(c->m_Level == 0 && c->m_Domain1 == m_Domain2->m_Id && c->m_Domain2 == m_Domain1->m_Id)))
		{
			continue;
		}
		int nr = c->m_EndResidueIndex - c->m_StartResidueIndex + 1;
		if (nr < minLen)
		{
			minLen = nr;
		}
	}
	return minLen;
}

bool Flexibility::computeBending()
{
	m_Bend = false;
	// if shear then no bending!
	//if( m_Shear )
	//{
	//	return true;
	//}
	// if no connector, then no bending
	int nConn = getNumberOfConnectors();
	if (nConn == 0)
	{
		return true;
	}
	// if shear, then strongly limit these rotation angles : redundant now!!
	if (m_Shear)
	{
		m_MinBendingRange = -LIMITED_BEND_ANGLE;
		m_MaxBendingRange = LIMITED_BEND_ANGLE;
	}
	else
	{
		int nRes = getNumberOfResidueInShortestConnector();
		double r = nRes*BEND_ANGLE_PER_RESIDUE/nConn;
		if (r > 175)
		{
			r = 175;
		}
		m_MinBendingRange = -r;
		m_MaxBendingRange =  r;
	}
	// find distance of end points of connectors to their avg. Hence find central connector. Use that for center and axis of bending.
	Vector e1, e2, m;
	int midConnector=0;
	{
		// get 3 points, define normal of plane. Thats the axis.
		Vector avgEndPoint1, avgEndPoint2;
		{
			float minLen = 100000000;
			for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
			{
				Connector* c = m_DomainComplex->m_Connectors[i];
				if (!((c->m_Level == 0 && c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id) ||
						(c->m_Level == 0 && c->m_Domain1 == m_Domain2->m_Id && c->m_Domain2 == m_Domain1->m_Id)))
				{
					continue;
				}
				// careful, we are adding w also
				Vector e1, e2;
				if (!c->computeEndPoints(&e1, &e2))
				{
					return false;
				}
				avgEndPoint1 = avgEndPoint1 + e1;
				avgEndPoint2 = avgEndPoint2 + e2;
			}
			avgEndPoint1 *= 1.0/nConn;
			avgEndPoint2 *= 1.0/nConn;
			if (avgEndPoint1[0] == avgEndPoint2[0] &&
					avgEndPoint1[1] == avgEndPoint2[1] &&
					avgEndPoint1[2] == avgEndPoint2[2])
			{
				m_MinBendingRange = m_MaxBendingRange = 0;
				return true; // bad luck
			}
		}
		{
			// find which was closest
			float minConnectorDisp = 1000000;
			for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
			{
				Connector* c = m_DomainComplex->m_Connectors[i];
				if (!((c->m_Level == 0 && c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id) ||
						(c->m_Level == 0 && c->m_Domain1 == m_Domain2->m_Id && c->m_Domain2 == m_Domain1->m_Id)))
				{
					continue;
				}
				// careful, we are adding w also
				Vector t_e1, t_e2;
				if (!c->computeEndPoints(&t_e1, &t_e2))
				{
					return false;
				}
				float d1 = get3DDistance(avgEndPoint1, t_e1);
				float d2 = get3DDistance(avgEndPoint2, t_e2);
				float d = (d1+d2)/2.0;
				if (d < minConnectorDisp)
				{
					minConnectorDisp = d;
					midConnector = i;
					if (!c->computeEndPoints(&e1, &e2))
					{
						return false;
					}
					if (!c->computeMidPoint(&m, m_DomainComplex->m_ResidueList))
					{
						return false;
					}
					m_ImportantConnectorIndex = i;
				}
			}
		}
	}
	// axis is perp to plane of end points, mid of middle connector.
	{
		m_BendingAxisHingePoint = m;
		Vector v1 = e1 - e2;
		Vector v2 = e1 - m;
		m_BendingAxis = v2.cross(v1);
		m_BendingAxis.normalize();
	}
	m_Bend = true;
	// now set the shear direction to primary bending axis direction
	if (m_Shear)
	{
		m_PlaneNormal = m_BendingAxis;
	}
	// lets also set the secondary bending here itself!
	m_SecondaryBendingAxisHingePoint = m_BendingAxisHingePoint;
	m_SecondaryMinBendingRange = m_MinBendingRange / nConn;
	m_SecondaryMaxBendingRange = m_MaxBendingRange / nConn;
	{
		Vector v1 = e1 - e2;
		m_SecondaryBendingAxis = v1.cross(m_BendingAxis);
		m_SecondaryBendingAxis.normalize();
	}
	m_SecondaryBend = true;
	return true;
}

bool Flexibility::computeSecondaryBendingAxis()
{
	// axis perp to prev two above
	// magnitude determined by variance in other direction.
	//m_SecondaryBendingAxisHingePoint = m_BendingAxisHingePoint;
	return true;
}

bool Flexibility::computeTwistingAxis()
{
	// only if one connector. Just twist along torsion angles once in all one direction, then all in another.
	// since only one connector, full range of motion
	int nConn = getNumberOfConnectors();
	if (nConn != 1)
	{
		m_Twist = false;
		return true;
	}
	Connector* conn = 0;
	{
		for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
		{
			Connector* c = m_DomainComplex->m_Connectors[i];
			if (!((c->m_Level == 0 && c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id) ||
					(c->m_Level == 0 && c->m_Domain1 == m_Domain2->m_Id && c->m_Domain2 == m_Domain1->m_Id)))
			{
				continue;
			}
			conn = c;
		}
	}
	if (!conn)
	{
		return true;    // wierd, but anyway
	}
	int numRes = conn->m_EndResidueIndex - conn->m_StartResidueIndex;
	m_MinTwistingRange = -numRes*TWIST_PER_RESIDUE/2.0;
	m_MaxTwistingRange = numRes*TWIST_PER_RESIDUE/2.0;
	m_Twist = true;
	return true;
}

bool Flexibility::isLeftRightConnector(Connector* c)
{
	if (!c)
	{
		return false;
	}
	if (c->m_Level != 0)
	{
		return false;
	}
	if (c->m_Domain1 == m_Domain1->m_Id && c->m_Domain2 == m_Domain2->m_Id)
	{
		return true;
	}
	if (c->m_Domain2 == m_Domain1->m_Id && c->m_Domain1 == m_Domain2->m_Id)
	{
		return true;
	}
	return false;
}

bool Flexibility::isRightSideConnector(Connector* c)
{
	if (!c)
	{
		return false;
	}
	if (c->m_Level != 0)
	{
		return false;
	}
	if (c->m_Domain1 == m_Domain1->m_Id || c->m_Domain2 == m_Domain1->m_Id)
	{
		return false;
	}
	if (c->m_Domain1 == m_Domain2->m_Id || c->m_Domain2 == m_Domain2->m_Id)
	{
		return true;
	}
	return false;
}

bool Flexibility::isConnectorOfRightDomain(Connector* c)
{
	if (!c)
	{
		return false;
	}
	if (c->m_Level != 0)
	{
		return false;
	}
	int numFound = 0;
	for (int d=0; d<m_RightDomains.size(); d++)
	{
		if (c->m_Domain1 == m_RightDomains[d])
		{
			numFound++;
		}
		if (c->m_Domain2 == m_RightDomains[d])
		{
			numFound++;
		}
		if (numFound >= 2)
		{
			return true;    // should only be 2, not greater than!
		}
	}
	return false;
}

// take current molecule, using shear direction, add these values to entire right domain
// For each connector:
//    Find length of connector, distribute the displacement
bool Flexibility::shearMolecule(int p1)
{
	if (p1 >= m_TotalShear)
	{
		return false;
	}
	if (m_TotalShear == 1)
	{
		return true;    // no need to do anything
	}
	Vector disp;
	if (m_ImportantConnectorIndex == -1)
	{
		// there is no connector, just shear ...
		// if there is no connector, shear in plane perp to line connecting centers.
		Vector c1 = m_Domain1->getCenter();
		Vector c2 = m_Domain2->getCenter();
		Vector line = c1-c2;
		line.normalize();
		float x, y, z;
		if (fabs(line[2]) > 0.001)
		{
			x = (float)rand()/RAND_MAX;
			y = (float)rand()/RAND_MAX;
			z = (line[0]*x + line[1]*y)/line[2];
		}
		else if (fabs(line[1]) > 0.001)
		{
			x = (float)rand()/RAND_MAX;
			z = (float)rand()/RAND_MAX;
			y = (line[0]*x + line[2]*z)/line[1];
		}
		else
		{
			y = (float)rand()/RAND_MAX;
			z = (float)rand()/RAND_MAX;
			x = (line[1]*y + line[2]*z)/line[0];
		}
		Vector disp1(x, y, z, 0);
		disp1.normalize();
		Vector disp2 = disp1.cross(line);
		disp2.normalize();
		disp = disp1+disp2;
	}
	else
	{
		// use important connector to find all info. Use the permutation index to see what to apply
		Connector* c = m_DomainComplex->m_Connectors[m_ImportantConnectorIndex];
		Vector e1, e2, m;
		if (!c->computeEndPoints(&e1, &e2))
		{
			return false;
		}
		if (!c->computeMidPoint(&m, m_DomainComplex->m_ResidueList))
		{
			return false;
		}
		// use important connector to find all info. Use the permutation index to see what to apply
		Vector bendingAxisHingePoint = m;
		Vector v1 = e1 - e2;
		Vector v2 = e1 - m;
		Vector bendingAxis = v2.cross(v1);
		bendingAxis.normalize();
		Vector planeNormal = bendingAxis;
		// mark out region. Randomly sample it.
		double r1 = (double)rand()/RAND_MAX - 0.5; // -0.5 to 0.5
		double r2 = (double)rand()/RAND_MAX - 0.5; // -0.5 to 0.5
		r1 *= m_ShearMagnitude;
		r2 *= m_ShearMagnitude;
		Vector disp1 = planeNormal;
		Vector disp2 = v1.cross(planeNormal);
		disp1.normalize();
		disp2.normalize();
		disp1 = disp1*r1;
		disp2 = disp2*r2;
		disp = disp1+disp2;
	}
	// if cut, shear all domains and connectors to right. Otherwise, shear one and adjust its connectors
	if (m_Cut)
	{
		for (int i=0; i<m_RightDomains.size(); i++)
		{
			int d;
			for (int d=0; d<m_DomainComplex->m_Domains.size(); d++)
			{
				if (m_DomainComplex->m_Domains[d]->m_Id == m_RightDomains[i])
				{
					m_DomainComplex->m_Domains[d]->translate(disp);
				}
			}
		}
	}
	else
	{
		// for all atoms in second domain, move it in given direction.
		m_Domain2->translate(disp);
	}
	// move all residue in connectors by appropriate amounts
	{
		for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
		{
			Connector* c = m_DomainComplex->m_Connectors[i];
			if (isLeftRightConnector(c))
			{
				c->translate(disp, m_Domain1->m_Id);
			}
			if (m_Cut)
			{
				if (isConnectorOfRightDomain(c))
				{
					c->translate(disp);
				}
			}
			else
			{
				if (isRightSideConnector(c))
				{
					// get the fixed domain id
					int fixedDomainId = c->m_Domain1;
					if (c->m_Domain1 == m_Domain2->m_Id)
					{
						fixedDomainId = c->m_Domain2;
					}
					c->translate(disp, fixedDomainId);
				}
			}
		}
	}
	return true;
}

// Given axis and angle, find quaternion matrix and apply to all in right domain.
bool Flexibility::bendMolecule(int p2)
{
	if (p2 >= m_TotalBend)
	{
		return false;
	}
	if (m_TotalBend == 1)
	{
		return true;    // no need to do anything
	}
	if (m_ImportantConnectorIndex == -1)
	{
		return true;    // we need a connector
	}
	Connector* c = m_DomainComplex->m_Connectors[m_ImportantConnectorIndex];
	Vector e1, e2, m;
	if (!c->computeEndPoints(&e1, &e2))
	{
		return false;
	}
	if (!c->computeMidPoint(&m, m_DomainComplex->m_ResidueList))
	{
		return false;
	}
	// use important connector to find all info. Use the permutation index to see what to apply
	Vector bendingAxisHingePoint = m;
	Vector v1 = e1 - e2;
	Vector v2 = e1 - m;
	Vector bendingAxis = v2.cross(v1);
	bendingAxis.normalize();
	// rotate all of domain 2 about the hinge point by given amount.
	// move connectors by displaced amount (this will be linear and look wierd.)
	// mark out region. Randomly sample it.
	double r = (double)rand()/RAND_MAX;
	r = (m_MaxBendingRange - m_MinBendingRange)*r + m_MinBendingRange;
	char fnameOld[1000];
	sprintf(fnameOld, "movedDomainOld%d.txt", p2);
	char fnameNew[1000];
	sprintf(fnameNew, "movedDomainNew%d.txt", p2);
	FILE* fpOld = fopen(fnameOld,"w");
	FILE* fpNew = fopen(fnameNew,"w");
	Matrix t1 = Matrix::translation(bendingAxisHingePoint*-1);
	Matrix t2 = Matrix::translation(bendingAxisHingePoint);
	Matrix rot = Quaternion::rotation(r*3.141592/180.0, bendingAxis[0], bendingAxis[1], bendingAxis[2]).buildMatrix();
	Matrix rotationMatrix = t2*rot*t1;
	if (m_Cut)
	{
		printf("Going to rotate all on right hand side\n");
		printf("Bending axis = [%f %f %f %f], hinge point = [%f %f %f %f], angle = %f\n",
			   bendingAxis[0], bendingAxis[1], bendingAxis[2], bendingAxis[3],
			   bendingAxisHingePoint[0], bendingAxisHingePoint[1], bendingAxisHingePoint[2], bendingAxisHingePoint[3],
			   r);
		// rotate all domains on the right
		for (int i=0; i<m_RightDomains.size(); i++)
		{
			printf("Searching for domain %d\n", m_RightDomains[i]);
			for (int d=0; d<m_DomainComplex->m_Domains.size(); d++)
			{
				if (m_DomainComplex->m_Domains[d]->m_Id == m_RightDomains[i])
				{
					printf("Found it\n");
					m_DomainComplex->m_Domains[d]->write(fpOld);
					m_DomainComplex->m_Domains[d]->rotate(rotationMatrix);
					m_DomainComplex->m_Domains[d]->write(fpNew);
				}
			}
		}
	}
	else
	{
		// for all atoms in second domain, rotate it
		m_Domain2->write(fpOld);
		m_Domain2->rotate(rotationMatrix);
		m_Domain2->write(fpNew);
	}
	fclose(fpOld);
	fclose(fpNew);
	// rotate all residue in connectors by appropriate amounts
	{
		for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
		{
			Connector* c = m_DomainComplex->m_Connectors[i];
			if (isLeftRightConnector(c))
			{
				c->rotate(rotationMatrix, m_Domain1->m_Id);
			}
			if (m_Cut)
			{
				if (isConnectorOfRightDomain(c))
				{
					c->rotate(rotationMatrix);
				}
			}
			else
			{
				if (isRightSideConnector(c))
				{
					// get the fixed domain id
					int fixedDomainId = c->m_Domain1;
					if (c->m_Domain1 == m_Domain2->m_Id)
					{
						fixedDomainId = c->m_Domain2;
					}
					c->rotate(rotationMatrix, fixedDomainId);
				}
			}
		}
	}
	return true;
}

// Given axis and angle, find quaternion matrix and apply to all in right domain.
bool Flexibility::secondaryBendMolecule(int p3)
{
	if (p3 >= m_TotalSecondaryBend)
	{
		return false;
	}
	if (m_TotalSecondaryBend == 1)
	{
		return true;    // no need to do anything
	}
	if (m_ImportantConnectorIndex == -1)
	{
		return true;    // we need a connector
	}
	// use important connector to find all info. Use the permutation index to see what to apply
	Connector* c = m_DomainComplex->m_Connectors[m_ImportantConnectorIndex];
	Vector e1, e2, m;
	if (!c->computeEndPoints(&e1, &e2))
	{
		return false;
	}
	if (!c->computeMidPoint(&m, m_DomainComplex->m_ResidueList))
	{
		return false;
	}
	// use important connector to find all info. Use the permutation index to see what to apply
	Vector bendingAxisHingePoint = m;
	Vector v1 = e1 - e2;
	Vector v2 = e1 - m;
	Vector bendingAxis = v2.cross(v1);
	bendingAxis.normalize();
	// lets also set the secondary bending here itself!
	Vector secondaryBendingAxisHingePoint = bendingAxisHingePoint;
	Vector secondaryBendingAxis = v1.cross(bendingAxis);
	secondaryBendingAxis.normalize();
	// rotate all of domain 2 about the hinge point by given amount.
	// move connectors by displaced amount (this will be linear and look wierd.)
	// mark out region. Randomly sample it.
	double r = (double)rand()/RAND_MAX;
	r = (m_SecondaryMaxBendingRange - m_SecondaryMinBendingRange)*r + m_SecondaryMinBendingRange;
	Matrix t1 = Matrix::translation(secondaryBendingAxisHingePoint*-1);
	Matrix t2 = Matrix::translation(secondaryBendingAxisHingePoint);
	Matrix rot = Quaternion::rotation(r*3.141592/180.0,
									  secondaryBendingAxis[0], secondaryBendingAxis[1], secondaryBendingAxis[2]).buildMatrix();
	Matrix rotationMatrix = t2*rot*t1;
	if (m_Cut)
	{
		// rotate all domains on the right
		for (int i=0; i<m_RightDomains.size(); i++)
		{
			for (int d=0; d<m_DomainComplex->m_Domains.size(); d++)
			{
				if (m_DomainComplex->m_Domains[d]->m_Id == m_RightDomains[i])
				{
					m_DomainComplex->m_Domains[d]->rotate(rotationMatrix);
				}
			}
		}
	}
	else
	{
		// for all atoms in second domain, rotate it
		m_Domain2->rotate(rotationMatrix);
	}
	// rotate all residue in connectors by appropriate amounts
	{
		for (int i=0; i<m_DomainComplex->m_Connectors.size(); i++)
		{
			Connector* c = m_DomainComplex->m_Connectors[i];
			if (isLeftRightConnector(c))
			{
				c->rotate(rotationMatrix, m_Domain1->m_Id);
			}
			if (m_Cut)
			{
				if (isConnectorOfRightDomain(c))
				{
					c->rotate(rotationMatrix);
				}
			}
			else
			{
				if (isRightSideConnector(c))
				{
					// get the fixed domain id
					int fixedDomainId = c->m_Domain1;
					if (c->m_Domain1 == m_Domain2->m_Id)
					{
						fixedDomainId = c->m_Domain2;
					}
					c->rotate(rotationMatrix, fixedDomainId);
				}
			}
		}
	}
	return true;
}

// We had already assumed single connector!
// Distribute angle along length of connector and twist phi psi angles
bool Flexibility::twistMolecule(int p4)
{
	// use important connector to find all info. Use the permutation index to see what to apply
	if (p4 >= m_TotalTwist)
	{
		return false;
	}
	if (m_TotalTwist == 1)
	{
		return true;    // no need to do anything
	}
	if (m_ImportantConnectorIndex == -1)
	{
		return true;    // we need a connector
	}
	// compute how many phi psi, divide up the angles into them and rotate!
	// PROBLEM: may twist wrong half of chain, not necessarily domain2.. so ?
	return true;
}

// See which ones to apply and call with appropriate values
bool Flexibility::applyTransformations(int p1, int p2, int p3, int p4)
{
	// apply them
	if (!shearMolecule(p1))
	{
		return false;
	}
	if (!bendMolecule(p2))
	{
		return false;
	}
	if (!secondaryBendMolecule(p3))
	{
		return false;
	}
	if (!twistMolecule(p4))
	{
		return false;
	}
	return true;
}

// WARNING : takes ownership in current version of code
bool Flexibility::initNewGOA()
{
	m_CurrentShear = m_CurrentBend = m_CurrentSecondaryBend = m_CurrentTwist = 0;
	// initialize molecule to extreme end of all movements
	return true;
}

// There are 4 types of movements, and each have been allocated a fixed set.
// try all permutations. With each new call, return next permutation.
bool Flexibility::getUpdatedGOA(int permutationIndex)
{
	// use permutation index and compute which index to be applied in the 4 categories
	// permutation index is 0 based.
	// indices are also 0 based.
	int p1, p2, p3, p4;
	int n1, n2, n3, n4;
	n1 = m_TotalShear;
	n2 = m_TotalBend;
	n3 = m_TotalSecondaryBend;
	n4 = m_TotalTwist;
	p1 = permutationIndex / n2*n3*n4;
	permutationIndex = permutationIndex - p1*n2*n3*n4;
	p2 = permutationIndex / n3*n4;
	permutationIndex = permutationIndex - p2*n3*n4;
	p3 = permutationIndex / n4;
	permutationIndex = permutationIndex - p3*n4;
	p4 = permutationIndex;
	applyTransformations(p1, p2, p3, p4);
	return true;
}

int Flexibility::getDomainSize()
{
	return m_Domain1->m_NumberOfResidues + m_Domain2->m_NumberOfResidues;
}

bool Flexibility::allocateNumberOfDiscreteFlexibilities(int n)
{
	if (n < 1)
	{
		return false;
	}
	// break this up to 4 diff cases depending on mutual weights times their ranges
	double shear_weight = SHEAR_WEIGHT * m_ShearMagnitude / MAX_SHEAR;
	double bending_weight = BENDING_WEIGHT * (m_MaxBendingRange-m_MinBendingRange) / 360.0;
	double secondary_bending_weight = SECONDARY_BENDING_WEIGHT * (m_SecondaryMaxBendingRange-m_SecondaryMinBendingRange) / 360.0;
	double twisting_weight = TWIST_WEIGHT * (m_MaxTwistingRange-m_MinTwistingRange) / 360.0;
	double total_weight = shear_weight + bending_weight + secondary_bending_weight + twisting_weight;
	m_TotalShear = pow(n, shear_weight / total_weight);
	if (m_TotalShear < 1)
	{
		m_TotalShear = 1;
	}
	m_TotalSecondaryBend = pow(n, secondary_bending_weight / total_weight);
	if (m_TotalSecondaryBend < 1)
	{
		m_TotalSecondaryBend = 1;
	}
	m_TotalTwist = pow(n, twisting_weight / total_weight);
	if (m_TotalTwist < 1)
	{
		m_TotalTwist = 1;
	}
	m_TotalBend = n / (m_TotalShear * m_TotalSecondaryBend * m_TotalTwist);
	if (m_TotalBend < 1)
	{
		return false;
	}
	m_TotalDiscreteConformations = n;
	return true;
}

void Flexibility::computeCutProperty()
{
	m_RightDomains.clear();
	// go right and see if any of them link to the left domain.
	std::stack<int> rightSideDomains;
	int leftId = m_Domain1->m_Id;
	int rightId = m_Domain2->m_Id;
	rightSideDomains.push(rightId);
	m_RightDomains.push_back(rightId);
	while (!rightSideDomains.empty())
	{
		// take out domain, find all domains attached to it which we have not yet seen and add it.
		int domainId = rightSideDomains.top();
		rightSideDomains.pop();
		for (int i=0; i<m_DomainComplex->m_Flexibility.size(); i++)
		{
			Flexibility* f = m_DomainComplex->m_Flexibility[i];
			if (f->m_ImportantConnectorIndex == -1 && f->m_Area < SHEAR_AREA_THRESHOLD/2.0)
			{
				continue;    // we dont use this connector!
			}
			if (f != this && (f->m_Domain1->m_Id == domainId || f->m_Domain2->m_Id == domainId))
			{
				// this flexibility is pointing to the right domain, so put into a stack.
				// if both sides have already been seen, forget it
				// if either side is leftId, we got a chain, so no cut!
				int otherId = f->m_Domain1->m_Id;
				if (f->m_Domain1->m_Id == domainId)
				{
					otherId = f->m_Domain2->m_Id;
				}
				if (otherId == leftId)
				{
					m_Cut = false;
					m_RightDomains.clear();
					return;
				}
				// check if other id has been seen, if not put it.
				int j;
				bool seen = false;
				for (j=0; j<m_RightDomains.size(); j++)
				{
					if (otherId == m_RightDomains[j])
					{
						seen = true;
						break;
					}
				}
				if (!seen)
				{
					rightSideDomains.push(otherId);
					m_RightDomains.push_back(otherId);
				}
			}
		}
	}
	{
		printf("Flexibility [%d %d] Residues seen are: ", m_Domain1->m_Id,m_Domain2->m_Id);
		for (int i=0; i<m_RightDomains.size(); i++)
		{
			printf(" %d ", m_RightDomains[i]);
		}
		printf("\n");
	}
	m_Cut = true;
}

bool Flexibility::isCut()
{
	return m_Cut;
}
