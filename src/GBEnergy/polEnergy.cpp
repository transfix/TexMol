#include "GBEnergy/polEnergy.h"
#include "FastSummation/FastSummation.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/atom.h"
#include "GBEnergy/PBunit.h"

#include <iostream>
using namespace std;

#include <vector>
#include <math.h>

#define _AMBER_
//#define _PDB2PQR_

#define _ORIGINAL_GB_
//#define _FASTENERGY_
//#define _GRIDAPPROX_

using namespace MOLECULE;
using namespace FUNCTION;
using std::vector;

PolEnergy::PolEnergy()
{
	pol_energy = 0.0;
}

PolEnergy::PolEnergy(GroupOfGBAtoms *m)
{
	molecule = m;
        pol_energy = 0.0;
}

PolEnergy::~PolEnergy()
{
	for (int i = 0; i < groups.size(); i++)
		delete []groups[i].member;
}

void PolEnergy::getPolEnergy(double eps, int threshold)
{
    const double e_in = 1.0;
    const double e_out = 80.0;
    const double ka = 0.316;
    double tau = 1.0/e_in - 1.0/e_out;
    double fGB, f, ff, rij_square, sum = 0.0;
    int i, j, k, h;

#ifdef _ORIGINAL_GB_
    cout << "using original GB" << endl;

    for (i = 0; i < molecule->numOfAtoms; i++)
    {
        for (j = i; j < molecule->numOfAtoms; j++)
        {
                // Wenqi modified on 4/23 to avoid zero born radius
            double RiRj = molecule->m_Atoms[i]->Born_radius*molecule->m_Atoms[j]->Born_radius;
            if (RiRj == 0)
	    {
		printf("error in Born Radius\n");
                ff = 0.0;
	    }
            else
            {
                rij_square = (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[j]->position[0]) * 
			     (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[j]->position[0]) +
                             (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[j]->position[1]) * 
			     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[j]->position[1]) +
                             (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[j]->position[2]) * 
			     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[j]->position[2]);
                ff = -1.0*rij_square/(4.0*RiRj);
		fGB = sqrt( rij_square + RiRj*exp(ff) );
//		printf("%f\n", fGB);

                f = molecule->m_Atoms[i]->charge*molecule->m_Atoms[j]->charge/fGB; // * (1 - exp(-k*fGB)/e_out);
                if( j == i )
                   sum += f;
                else
                   sum += f * 2.0f;
            }
        }
    }
#endif // _ORIGINALGB_

#ifdef _FASTENERGY_
       cout << "using fast GB" << endl;

	FastSummation *m_fastsum;
	double s[molecule->numOfAtoms];

        double R_min = 99999.0;
        double R_max = -99999.0;
     
        cout << "num atoms: " << molecule->numOfAtoms << endl;

        for (i = 0; i < molecule->numOfAtoms; i++)
        {
                if (molecule->m_Atoms[i]->Born_radius < R_min)
                        R_min = molecule->m_Atoms[i]->Born_radius;
                if (molecule->m_Atoms[i]->Born_radius > R_max)
                        R_max = molecule->m_Atoms[i]->Born_radius;
        }
        cout << "R_min: " << R_min << " R_max: " << R_max << endl;
	groupingAtoms(eps, R_min, R_max);
	cout <<  "eps = " << eps << ", groups = " <<  groups.size() << endl;
/*
	FILE *fp = fopen("check", "w");
	for (i = 0; i < groups.size(); i++)
	{
        	int M = groups[i].numOfMember;
		fprintf(fp, "%d: ", M);
		for (j = 0; j < M; j++)
		{
			fprintf(fp, "%d  ", groups[i].member[j]);
		}
		fprintf(fp, "\n");
	}
*/
	double max[3], min[3], center[3];
        for (i = 0; i < 3; i++)
        {
		max[i] = -99999.0;
		min[i] = 99999.0;
	}
	for (i = 0; i < molecule->numOfAtoms; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (molecule->m_Atoms[i]->position[j] < min[j])	min[j] = molecule->m_Atoms[i]->position[j];
			if (molecule->m_Atoms[i]->position[j] > max[j]) max[j] = molecule->m_Atoms[i]->position[j];
		}
		s[i] = 0.0;
	}
	for (i = 0; i < 3; i++)	center[i] = (min[i] + max[i]) / 2.0;
	double abs_max = sqrt((max[0]-min[0])*(max[0]-min[0])+(max[1]-min[1])*(max[1]-min[1])+(max[2]-min[2])*(max[2]-min[2]));

        int n = 30;     //100;
        int p = 2;
        int m = 4;
        double param[2];
        double eps_I = 0.0;//0.1; //0.01; // for x_4th and x_7th
        double eps_B = 0.0; // 0.01;

	bool need_fastsum;
	cout <<  "start fast summation" << endl;

	for (i = 0; i < groups.size(); i++)
	{
        	int M =  groups[i].numOfMember;
        	double *y = (double *)malloc(sizeof(double)*3*M);
        	double *temp_sum = (double *)malloc(sizeof(double)*M);
		for (k = 0; k < M; k++)
		{
			y[3*k  ] = (molecule->m_Atoms[groups[i].member[k]]->position[0] - center[0]) / (2*abs_max);
			y[3*k+1] = (molecule->m_Atoms[groups[i].member[k]]->position[1] - center[1]) / (2*abs_max);
			y[3*k+2] = (molecule->m_Atoms[groups[i].member[k]]->position[2] - center[2]) / (2*abs_max);
			if (fabs(y[3*k]) > 0.25 || fabs(y[3*k+1]) > 0.25 || fabs(y[3*k+2]) > 0.25)
				printf("y out of range\n");
		}
		for (j = 0; j < groups.size(); j++)
		{
			int N =  groups[j].numOfMember;
			if (N > threshold)
				need_fastsum = true;
			else
				need_fastsum = false;
			double *x = (double *)malloc(sizeof(double)*3*N);
        		double *coeff = (double *)malloc(sizeof(double)*N);
			param[0] = 2*abs_max;
			param[1] = R_min*pow(1+eps, i) * R_min*pow(1+eps, j);
			for (k = 0; k < N; k++)
			{
				x[3*k  ] = (molecule->m_Atoms[groups[j].member[k]]->position[0] - center[0]) / (2*abs_max);
				x[3*k+1] = (molecule->m_Atoms[groups[j].member[k]]->position[1] - center[1]) / (2*abs_max);
				x[3*k+2] = (molecule->m_Atoms[groups[j].member[k]]->position[2] - center[2]) / (2*abs_max);
				coeff[k] = molecule->m_Atoms[groups[j].member[k]]->charge;
	                        if (fabs(x[3*k]) > 0.25 || fabs(x[3*k+1]) > 0.25 || fabs(x[3*k+2]) > 0.25)
        	                        printf("x out of range\n");
			}
			double temp_min = 99999.0;
			for (k = 0; k < M; k++)
			{
				for (h = 0; h < N; h++)
				{
					double rrr = sqrt((x[3*h  ]-y[3*k  ])*(x[3*h  ]-y[3*k  ]) +
                                             (x[3*h+1]-y[3*k+1])*(x[3*h+1]-y[3*k+1]) +
                                             (x[3*h+2]-y[3*k+2])*(x[3*h+2]-y[3*k+2]));
					if (rrr < temp_min)
						temp_min = rrr;
				}
			}
	//		printf("%f\n", temp_min);
			for (k = 0; k < M; k++)	temp_sum[k] = 0.0;
			if (need_fastsum == true)
			{
	//			printf("$$$\n");
        			m_fastsum = new FastSummation(3, N, M, n, m, p, "Still_GB", param, eps_I, eps_B);
				m_fastsum->fastSum(x, y, coeff, temp_sum);
				delete m_fastsum;
			} else {
				for (k = 0; k < M; k++)
				{
					for (h = 0;  h < N; h++)
					{
						double rhk = (x[3*h  ]-y[3*k  ])*(x[3*h  ]-y[3*k  ]) + 
							     (x[3*h+1]-y[3*k+1])*(x[3*h+1]-y[3*k+1]) + 
							     (x[3*h+2]-y[3*k+2])*(x[3*h+2]-y[3*k+2]);
						temp_sum[k] += coeff[h] / sqrt(param[0]*param[0]*rhk + param[1]*exp(-0.25*rhk/param[1]));
					}
				}
			}
			for (k = 0; k < M; k++)
				s[groups[i].member[k]] += temp_sum[k];
			free(x);
			free(coeff);
		}
		free(y);
		free(temp_sum);
	}
	sum = 0.0;
	for (i = 0; i < molecule->numOfAtoms; i++)
		sum += molecule->m_Atoms[i]->charge * s[i];

#endif // _FASTENERGY_

#ifdef _GRIDAPPROX_
        cout << "using grid approximated GB" << endl;

	double Ri, Rj;
        double s[molecule->numOfAtoms];
        double R_min = 99999.0;
        double R_max = -99999.0;
        for (i = 0; i < molecule->numOfAtoms; i++)
        {
                if (molecule->m_Atoms[i]->Born_radius < R_min)
                        R_min = molecule->m_Atoms[i]->Born_radius;
                if (molecule->m_Atoms[i]->Born_radius > R_max)
                        R_max = molecule->m_Atoms[i]->Born_radius;
        }
        groupingAtoms(eps, R_min, R_max);
	for (i = 0; i < molecule->numOfAtoms; i++)
		s[i] = 0.0;
        for (i = 0; i < groups.size(); i++)
        {
                int M =  groups[i].numOfMember;
		double *x = (double *)malloc(sizeof(double)*3*M);
		Ri = R_min*pow(1+eps, i);
		for (k = 0; k < M; k++)
                {
                	x[3*k  ] = molecule->m_Atoms[groups[i].member[k]]->position[0];
                        x[3*k+1] = molecule->m_Atoms[groups[i].member[k]]->position[1];
                        x[3*k+2] = molecule->m_Atoms[groups[i].member[k]]->position[2];
                        coeff[k] = molecule->m_Atoms[groups[i].member[k]]->charge;
                }

	        double max[3], min[3], center[3];
	        for (j = 0; j < 3; j++)
        	{
                	max[j] = -99999.0;
                	min[j] = 99999.0;
        	}
        	for (k = 0; k < M; k++)
        	{
                	for (j = 0; j < 3; j++)
                	{
                        	if (molecule->m_Atoms[groups[i].member[k]]->position[j] < min[j]) min[j] = molecule->m_Atoms[groups[i].member[k]]->position[j];
                        	if (molecule->m_Atoms[groups[i].member[k]]->position[j] > max[j]) max[j] = molecule->m_Atoms[groups[i].member[k]]->position[j];
                	}
        	}
        	for (j = 0; j < 3; j++) center[j] = (min[j] + max[j]) / 2.0;
		for (j = 0; j < groups.size(); j++)
		{
			int N = groups[j].numOfMember;
			Rj = R_min*pow(1+eps, j);
			int dim[3] = {20, 20, 20};
			double *sum_y = (double *)malloc(sizeof(double)*dim[0]*dim[1]*dim[2]);

			for (k = 0; k < dim[0]*dim[1]*dim[2]; k++)
				sum_y[k] = 0.0;
			for (k = 0; k < dim[0]; k++)
			{
				double y1 = center[0]-10+k;
				for (h = 0; h < dim[1]; h++)
				{
					double y2 = center[1]-10+h;
					for(int t = 0; t < dim[2]; t++)
					{
						double y3 = center[2]-10+t;
						for (int a = 0; a < M; a++)
						{
							double rij2 = (x[3*a  ] - y1) * (x[3*a  ] - y1) + 
							      	      (x[3*a+1] - y2) * (x[3*a+1] - y2) + 
							              (x[3*a+2] - y3) * (x[3*a+2] - y3);
							sum_y [k*dim[0]*dim[1]+h*dim[0]+t] += coeff[a] / sqrt(rij2+ Ri*Rj*exp(-0.25*rij2/(Ri*Rj)));
						}
					}
				}
			}
			for (k = 0; k < N; k++)
			{
				double d1, d2, d3;
				int i1, i2, i3;
				d1 = molecule->m_Atoms[groups[j].member[k]]->position[0]-center[0];
				d2 = molecule->m_Atoms[groups[j].member[k]]->position[1]-center[1];
				d3 = molecule->m_Atoms[groups[j].member[k]]->position[2]-center[2];
				if (fabs(d1) > 20.0 || fabs(d2) > 20.0 || fabs(d3) > 20.0) continue;
				i1 = (int)d1;	i2 = (int)d2;	i3 = (int)d3;
				if (d1 - i1 == 0 && d2 - i2 == 0 && d3 - i3 ==0) // on grid point
					s[groups[j].member[k]] += molecule->m_Atoms[groups[j].member[k]]->charge * sumy[]

				else	d1 = (int)d1 - 1;
				
			}
		}
	
	}
#endif //_GRIDAPPROX_


#ifdef _AMBER_
    pol_energy = -0.5*tau*sum;
//    pol_energy = (-166.0)*tau*sum;
#endif

#ifdef _PDB2PQR_
   double zmagic = Vunit_ec * Vunit_ec / (Vunit_eps0*1.0e-10) * Vunit_Na * 1.0e-3 *  2.3900574e-01;
   pol_energy = -0.5*tau*zmagic*sum;
#endif
   
}

void PolEnergy::groupingAtoms(double eps, double R_min, double R_max)
{
	int i;
        for (double r = R_min; r < R_max; r *= (1+eps))
        {
                vector<int> temp_group;
                for (i = 0; i < molecule->numOfAtoms; i++)
                {
                        if (molecule->m_Atoms[i]->Born_radius >= r && molecule->m_Atoms[i]->Born_radius < r*(1+eps))
                                temp_group.push_back(i);
                }
                //printf("%d ", temp_group.size());
                if (temp_group.size() == 0)	continue;
                group group_k;
                group_k.numOfMember = temp_group.size();
                group_k.member = new int[temp_group.size()];
                for (i = 0; i < temp_group.size(); i++)
                        group_k.member[i] = temp_group[i];
                groups.push_back(group_k);
//                for (i = 0; i < temp_group.size(); i++)
//                        printf("%d ", groups[groups.size()-1].member[i]);
//                printf("\n");
                temp_group.clear();
        }
}
