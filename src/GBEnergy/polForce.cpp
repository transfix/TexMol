#include "GBEnergy/polForce.h"
#include "GBEnergy/groupOfAtoms.h"
#include "GBEnergy/atom.h"
#include "GBEnergy/surface.h"
#include "GBEnergy/geometry.h"
#include "FastSummation/FastSummation.h"

#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std; 
using namespace MOLECULE;
using namespace GBGEOMETRY;
using namespace FUNCTION;

PolForce::PolForce()
{

}

void PolForce::getPolForce(GroupOfGBAtoms *molecule, Surface *surface)
{
	int k, i, j;
	double Gik_rik;
	double rik_xk[3];

	double Gij_Ri, Gij_Rj;
//	double Ri_xk[3], Rj_xk[3];
	double *Ri_xk = new double[3*molecule->numOfAtoms];

	double Gii_Ri;

	const double e_in = 1.0;
	const double e_out = 80.0;
	double tau = 1.0/e_in - 1.0/e_out;

	const double w = 1.4;
	const double pi = 3.1415926;

	double sumx, sumy, sumz, sum_temp;
//	double *Gpol_R = new double[molecule->numOfAtoms];
//	derivGpolOverR(Gpol_R, molecule);

	double *Rk_xk_2ndInt = new double[3*molecule->numOfAtoms];
	derivRi_xk_2ndInt(molecule, surface, Rk_xk_2ndInt);

	printf("fast summation is done\n");
	surface->m_Vertices.clear();
	surface->m_Faces.clear();

	double xi[3], xj[3], rij;
	for (k = 0; k < molecule->numOfAtoms; k++)
	{
		FORCEFUNC m_force;
		sumx = 0.0;	sumy = 0.0;	sumz = 0.0;
		for (i = 0; i < molecule->numOfAtoms; i++)
		{
			if (i == k)	continue;
			Gik_rik = derivGikOverDistance(molecule, i, k);
			deriv_rik_xk(molecule, i, k, rik_xk);
			sumx += Gik_rik * rik_xk[0];
			sumy += Gik_rik * rik_xk[1];
			sumz += Gik_rik * rik_xk[2];
		}
		// printf("1 is done: %f %f %f\n", sumx, sumy, sumz);

		derivRi_xk(molecule, k, Rk_xk_2ndInt, w, pi, Ri_xk);
		for (i = 0; i < molecule->numOfAtoms; i++)
		{
			xi[0] = molecule->m_Atoms[i]->position[0];
			xi[1] = molecule->m_Atoms[i]->position[1];
			xi[2] = molecule->m_Atoms[i]->position[2];

			sum_temp = 0.0;
			for (j = 0; j < molecule->numOfAtoms; j++)
			{
				if (j == i) continue;
				xj[0] = molecule->m_Atoms[j]->position[0];
				xj[1] = molecule->m_Atoms[j]->position[1];
				xj[2] = molecule->m_Atoms[j]->position[2];
				rij = (xi[0] - xj[0])*(xi[0] - xj[0]) + (xi[0] - xj[0])*(xi[0] - xj[0]) + (xi[0] - xj[0])*(xi[0] - xj[0]);
				if (rij > 25.0) continue;

				Gij_Ri = derivGijOverRi(molecule, i, j);
//				printf("Gij_Ri %f\n", Gij_Ri);
				sum_temp += Gij_Ri;
			}
//			printf("Ri_xk: %f %f %f sum_temp: %f\n", Ri_xk[0], Ri_xk[1], Ri_xk[2], sum_temp);
			sumx += Ri_xk[3*i + 0]*sum_temp;
			sumy += Ri_xk[3*i + 1]*sum_temp;
			sumz += Ri_xk[3*i + 2]*sum_temp;
//			printf("2 is done: %f %f %f\n", sumx, sumy, sumz);

			Gii_Ri = derivGiiOverRi(molecule, i);
			sumx += 0.5 * Gii_Ri * Ri_xk[0];
			sumy += 0.5 * Gii_Ri * Ri_xk[1];
			sumz += 0.5 * Gii_Ri * Ri_xk[2];
//			printf("3 is done: %f %f %f\n", sumx, sumy, sumz);
		}	
		m_force.fx = -1.0 * tau * sumx;
                m_force.fy = -1.0 * tau * sumy;
                m_force.fz = -1.0 * tau * sumz;
		// printf("%f %f %f\n", m_force.fx, m_force.fy, m_force.fz);
		force.push_back(m_force);
		printf("%d is done\n", k);
	}
}


void PolForce::derivRi_xk(GroupOfGBAtoms *molecule, int k, double *Rk_xk_2ndInt, const double w, const double pi, double *Ri_xk)
{
	double Ri_xk_1stInt[3];
	int i;

	for (i = 0; i < molecule->numOfAtoms; i++)
	{
		derivRi_xk_1stInt(molecule, i, k, w, pi, Ri_xk_1stInt);
		double Ri = molecule->m_Atoms[i]->Born_radius;
		Ri_xk[3*i + 0] = -0.25*Ri*Ri/pi*Ri_xk_1stInt[0];
		Ri_xk[3*i + 1] = -0.25*Ri*Ri/pi*Ri_xk_1stInt[1];
		Ri_xk[3*i + 2] = -0.25*Ri*Ri/pi*Ri_xk_1stInt[2];
		if (i == k)
		{
			Ri_xk[3*i + 0] += -0.25*Ri*Ri/pi*Rk_xk_2ndInt[3*k + 0];
			Ri_xk[3*i + 1] += -0.25*Ri*Ri/pi*Rk_xk_2ndInt[3*k + 1];
			Ri_xk[3*i + 2] += -0.25*Ri*Ri/pi*Rk_xk_2ndInt[3*k + 2];
		}
	}
}

void PolForce::derivRi_xk_2ndInt(GroupOfGBAtoms *molecule, Surface *surface, double *Rk_xk_2ndInt)
{
        int i,k;
        double *x, *y, *coeff, *coeffx, *coeffy, *coeffz;
        double *sum, *sumx, *sumy, *sumz;
        double A6;
        FastSummation *m_fastsum;

        int nn = surface->nodes_per_face;
        int N = surface->numbtris*nn;
        int M = molecule->numOfAtoms;
        int n = 100;
        int p = 2;
        int m = 4;
        double param = 1.0;
        double eps_I = 0.01; // for x_4th and x_7th
        double eps_B = 0.01;

        x = (double *)malloc(sizeof(double)*3*N);
        y = (double *)malloc(sizeof(double)*3*M);
        coeff = (double *)malloc(sizeof(double)*N);
        coeffx = (double *)malloc(sizeof(double)*N);
        coeffy = (double *)malloc(sizeof(double)*N);
        coeffz = (double *)malloc(sizeof(double)*N);

        sum = (double *)malloc(sizeof(double)*M);
        sumx = (double *)malloc(sizeof(double)*M);
        sumy = (double *)malloc(sizeof(double)*M);
        sumz = (double *)malloc(sizeof(double)*M);

//        A6 = (double *)malloc(sizeof(double)*M);

        m_fastsum = new FastSummation(3, N, M, n, m, p, (char *)"one_over_sixth", &param, eps_I, eps_B);

        // make x,y within [-1/4, 1/4]
        double abs_max, abs_i;
        abs_max = -1.0;
        for (i = 0; i < 3; i++)
        {
                abs_i = fabs(surface->min[i]);
                if ( abs_i > abs_max)   abs_max = abs_i;
                abs_i = fabs(surface->max[i]);
                if ( abs_i > abs_max)   abs_max = abs_i;
        }

        for (i = 0; i < surface->numbtris; i++)
        {
                for (k = 0; k < nn; k++)
                {
                        x[3*(i*nn+k)] = surface->m_Faces[i]->node[k].Point[0] / (4*abs_max);
                        x[3*(i*nn+k)+1] = surface->m_Faces[i]->node[k].Point[1] / (4*abs_max);
                        x[3*(i*nn+k)+2] = surface->m_Faces[i]->node[k].Point[2] / (4*abs_max);
                }
        }
        for ( i = 0; i < molecule->numOfAtoms; i++)
        {
                y[3*i] = molecule->m_Atoms[i]->position[0] / (4*abs_max);
                y[3*i+1] = molecule->m_Atoms[i]->position[1] / (4*abs_max);
                y[3*i+2] = molecule->m_Atoms[i]->position[2] / (4*abs_max);
        }

        // summation //
        for (i = 0; i < surface->numbtris; i++)
        {
                for (k = 0; k < nn; k++)
                {
                        coeff[i*nn+k] = dotProduct(surface->m_Faces[i]->node[k].Point, surface->m_Faces[i]->node[k].Normal);
                        coeff[i*nn+k] *= surface->m_Faces[i]->weight[k] / pow(4*abs_max, 6);

                        coeffx[i*nn+k] = surface->m_Faces[i]->node[k].Normal[0];
                        coeffx[i*nn+k] *= surface->m_Faces[i]->weight[k] / pow(4*abs_max, 6);

                        coeffy[i*nn+k] = surface->m_Faces[i]->node[k].Normal[1];
                        coeffy[i*nn+k] *= surface->m_Faces[i]->weight[k] / pow(4*abs_max, 6);

                        coeffz[i*nn+k] = surface->m_Faces[i]->node[k].Normal[2];
                        coeffz[i*nn+k] *= surface->m_Faces[i]->weight[k] / pow(4*abs_max, 6);
                }
        }

        m_fastsum->threadedFastSum(x, y, coeff, coeffx, coeffy, coeffz, sum, sumx, sumy, sumz);

        for (i = 0; i < molecule->numOfAtoms; i++)
        {
                A6 = -12.0*(sum[i] - molecule->m_Atoms[i]->position[0]*sumx[i] - 
				     molecule->m_Atoms[i]->position[1]*sumy[i] - 
				     molecule->m_Atoms[i]->position[2]*sumz[i]);
		Rk_xk_2ndInt[3*i] =  molecule->m_Atoms[i]->position[0]*A6;
		Rk_xk_2ndInt[3*i+1] =  molecule->m_Atoms[i]->position[1]*A6;
		Rk_xk_2ndInt[3*i+2] =  molecule->m_Atoms[i]->position[2]*A6;
        }
//	for (i = 0; i < molecule->numOfAtoms; i++)
//		printf("%f %f %f\n", Rk_xk_2ndInt[3*i], Rk_xk_2ndInt[3*i+1], Rk_xk_2ndInt[3*i+2]);
        delete m_fastsum;
}


void PolForce::derivRi_xk_1stInt(GroupOfGBAtoms *molecule, int i, int k, const double w, const double pi, double *Ri_xk_1stInt)
{
	double oneGaussian_node = 0.0;
	double oneGaussian_weight = 2.0;
	double fourGaussian_node[4] = {-0.861136311594053, -0.339981043584856, 0.339981043584856, 0.861136311594053};
	double fourGaussian_weight[4] = {0.347854845137454, 0.652145154862546, 0.652145154862546, 0.347854845137454};
	double r_node[4], r_weight[4];
	double theta_node[4], theta_weight[4];
	double phi_node[4], phi_weight[4];
	double sumx, sumy, sumz;
	double f[3];

	int m,n,l;
	r_node[0] = 0.5*oneGaussian_node + molecule->m_Atoms[k]->radius + 0.5*w;
	r_weight[0] = 0.5*w*oneGaussian_weight;
	for (m = 0; m < 4; m++)
	{
//		r_node[m] = 0.5*Gaussian_node[m] + molecule->m_Atoms[k]->radius + 0.5*w;
//		r_weight[m] = 0.5*w*Gaussian_weight[m];
		theta_node[m] = pi*(fourGaussian_node[m]+1);
		theta_weight[m] = pi*fourGaussian_weight[m];
		phi_node[m] = 0.5*theta_node[m];
		phi_weight[m] = 0.5*theta_weight[m];
	}
	sumx = 0.0;	sumy = 0.0;	sumz = 0.0;
	for (m = 0; m < 4; m++)
	{
		double sin_phi = sin(phi_node[m]);
		for (n = 0; n < 4; n++)
		{
			for (l = 0; l < 1; l++)
			{
				evaluateIntegrand(molecule, i, k, r_node[l], theta_node[n], phi_node[m], w, f);
				double new_w = r_weight[l]*theta_weight[n]*phi_weight[m];
				sumx += f[0] * r_node[l]*r_node[l]*sin_phi * new_w;
				sumy += f[1] * r_node[l]*r_node[l]*sin_phi * new_w;
				sumz += f[2] * r_node[l]*r_node[l]*sin_phi * new_w;
			}
		}
	}
	Ri_xk_1stInt[0] = sumx;
	Ri_xk_1stInt[1] = sumy;
	Ri_xk_1stInt[2] = sumz;
}


void PolForce::evaluateIntegrand(GroupOfGBAtoms *molecule, int i, int k, double r, double theta, double phi, const double w, double *f)
{
        double xi[3], xk[3], x[3];
	double rhom, rhol, rhon;
	int m, n, l;
	double dens, sum1, sum2, sum3;
        double drhok;
        double dk, di, di_square;
        int N = molecule->numOfAtoms;
        int p;

        for (p = 0; p < 3; p++)
        {
                xi[p] = molecule->m_Atoms[i]->position[p];
                xk[p] = molecule->m_Atoms[k]->position[p];
        }
        x[0] = xk[0] + r*cos(theta)*sin(phi);
        x[1] = xk[1] + r*sin(theta)*sin(phi);
        x[2] = xk[2] + r*cos(phi);

	sum1 = 0.0;
        for (m = 0; m < N; m++)
        {
		if (m == k)	continue;
                rhom = evaluateAtomicDensity(molecule, x, m, w);
                if (rhom  < 0.000001)
                        continue;
		sum2 = 0.0;
                for (n = m+1; n < N; n++)
                {
                        rhon = evaluateAtomicDensity(molecule, x, n, w);
                        if (rhon  < 0.000001)
                                continue;
			sum3 = 0.0;
                        for (l = n+1; l < N; l++)
                        {
                                rhol = evaluateAtomicDensity(molecule, x, l, w);
                                if (rhol  < 0.000001)
                                        continue;
				sum3 += rhol;
                        }
			sum2 += rhon*(1.0-sum3);
                }
		sum1 += rhom*(1.0-sum2);
        }
	dens = 1.0 - sum1;

        dk = sqrt((x[0] - xk[0])*(x[0] - xk[0]) + (x[1] - xk[1])*(x[1] - xk[1]) + (x[2] - xk[2])*(x[2] - xk[2]));
        di_square = (x[0] - xi[0])*(x[0] - xi[0]) + (x[1] - xi[1])*(x[1] - xi[1]) + (x[2] - xi[2])*(x[2] - xi[2]);
        di = sqrt(di_square);
        drhok = molecule->m_Atoms[k]->rho->coeffOfr[1] + dk*(2*molecule->m_Atoms[k]->rho->coeffOfr[2] + dk*3*molecule->m_Atoms[k]->rho->coeffOfr[3]);
        for (p = 0; p < 3; p++)
                f[p] = -1.0*drhok*dens/(di_square*di_square*dk);
        f[0] *= xk[0];
        f[1] *= xk[1];
        f[2] *= xk[2];
}


/*
void PolForce::evaluateIntegrand(GroupOfGBAtoms *molecule, int i, int k, double r, double theta, double phi, const double w, double *f)
{
	double xi[3], xk[3], x[3];
	double dens;
	vector<int> temp_k_neighbor;
	vector<double> temp_rhom;
	int N = molecule->numOfAtoms;
	int p;

	double drhok;
	double dk, di, di_square;

	for (p = 0; p < 3; p++)
	{
		xi[p] = molecule->m_Atoms[i]->position[p];
		xk[p] = molecule->m_Atoms[k]->position[p];
	}
	x[0] = xk[0] + r*cos(theta)*sin(phi);
	x[1] = xk[1] + r*sin(theta)*sin(phi);
	x[2] = xk[2] + r*cos(phi);

	for (p = 0; p < N; p++)
	{
		if (molecule->neighbor[k*N+p] == false)
			continue;
		double rhop = evaluateAtomicDensity(molecule, x, p , w);
                if (rhop  < 0.000001)
                        continue;
		int m = p;
		temp_k_neighbor.push_back(m);
		temp_rhom.push_back(rhop);
	}

	int M = temp_k_neighbor.size();

	int *k_neighbor = new int[M];
	double *rhom = new double[M];
	for (p = 0; p < M; p++)
	{
		k_neighbor[p] = temp_k_neighbor[p];
		rhom[p] = temp_rhom[p];
	}
	temp_k_neighbor.clear();
	temp_rhom.clear();
	
	dens = 0.0;
	for (p = 0; p < M; p++)
	{
		dens += Density(molecule, p, k_neighbor, rhom, M, N, 0);
	}
	dens = 1.0 - dens;
	// compute d_rho_k
	dk = sqrt((x[0] - xk[0])*(x[0] - xk[0]) + (x[1] - xk[1])*(x[1] - xk[1]) + (x[2] - xk[2])*(x[2] - xk[2]));
	di_square = (x[0] - xi[0])*(x[0] - xi[0]) + (x[1] - xi[1])*(x[1] - xi[1]) + (x[2] - xi[2])*(x[2] - xi[2]);
	di = sqrt(di_square);
	drhok = molecule->m_Atoms[k]->rho->coeffOfr[1] + dk*(2*molecule->m_Atoms[k]->rho->coeffOfr[2] + dk*3*molecule->m_Atoms[k]->rho->coeffOfr[3]);
	for (p = 0; p < 3; p++)
		f[p] = drhok*dens/(di_square*di_square*dk);
	f[0] *= xk[0];
	f[1] *= xk[1];
	f[2] *= xk[2];
}
*/

double PolForce::Density(GroupOfGBAtoms *molecule, int P, int *k_neighbor, double *rhom, int k_neighbor_size, int N, int depth)
{
	if (k_neighbor == NULL)	
		return rhom[P];
	if (depth > 2)	return 0.0;

	int p = k_neighbor[P];
	vector<int> temp_p_neighbor;
	double x = 0.0;
	int t, q;
	for (t = 0; t < k_neighbor_size; t++)
	{
		q = k_neighbor[t];
		if (q < p+1)	continue;
		if (molecule->neighbor[p*N+q] == false)	continue;
		temp_p_neighbor.push_back(q);
	}

	int M  = temp_p_neighbor.size();
	int *p_neighbor = new int[M];
	for (t = 0; t < M; t++)
		p_neighbor[t] = temp_p_neighbor[t];
	temp_p_neighbor.clear();

	for (t = 0; t < M; t++)
	{
		q = p_neighbor[t];
		x += Density(molecule, t, p_neighbor, rhom, M, N, depth+1);
	}
	return rhom[P]*(1-x);
}

/*
	for (p = 0; p < N; p++)
	{
		if (molecule->neighbors[i*N+p] == false)
			continue;
		rhom = evaluateDensity(molecule, x, p, w);
		if (rhom  < 0.000001)
			continue;
		m = p;
		for (q = m+1; q < N; q++)
		{
			if (molecule->neighbors[i*N+q] == false)
                        	continue;
                	rhon = evaluateDensity(molecule, x, q, w);
                	if (rhon  < 0.000001)
                        	continue;
                	n = q;
                	for (s = n+1; s < N; s++)
			{
				if (molecule->neighbors[i*N+s] == false)
					continue;
				rhol = evaluateDensity(molecule, x, s, w);
				if (rhol  < 0.000001)
					continue;
				l = s;
				
			}
		}
	}
}
*/


double PolForce::evaluateAtomicDensity(GroupOfGBAtoms *molecule, double *x, int j, const double w)
{
	double xj[3];
	double rj;
	double d, r;

	xj[0] = molecule->m_Atoms[j]->position[0];
	xj[1] = molecule->m_Atoms[j]->position[1];
	xj[2] = molecule->m_Atoms[j]->position[2];
	rj = molecule->m_Atoms[j]->radius;

	d = sqrt((x[0] - xj[0])*(x[0] - xj[0]) + (x[1] - xj[1])*(x[1] - xj[1]) + (x[2] - xj[2])*(x[2] - xj[2]));

	if ( (rj+w-d) < 0.000001)
		return 0.0;
	if ((d-rj) < 0.000001)
		return molecule->m_Atoms[j]->rho->para_c;
	r = d - rj;
	return molecule->m_Atoms[j]->rho->coeffOfr[0] + r*(molecule->m_Atoms[j]->rho->coeffOfr[1] + r*(molecule->m_Atoms[j]->rho->coeffOfr[2] + r*molecule->m_Atoms[j]->rho->coeffOfr[3]));

}

double PolForce::derivGikOverDistance(GroupOfGBAtoms *molecule, int i, int k)
{
        double qi, qk;
        double Ri, Rk;
        double rik_square;
	double x, y, z;
	double deriv;
/*
        const double e_in = 1.0;
        const double e_out = 80.0;
        double tau = 1.0/e_in - 1.0/e_out;
*/
	qi = molecule->m_Atoms[i]->charge;
        Ri = molecule->m_Atoms[i]->Born_radius;
        qk = molecule->m_Atoms[k]->charge;
        Rk = molecule->m_Atoms[k]->Born_radius;
	rik_square = (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[k]->position[0]) *
                     (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[k]->position[0]) +
		     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[k]->position[1]) *
		     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[k]->position[1]) +
		     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[k]->position[2]) *
		     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[k]->position[2]);
	x = rik_square/(4*Ri*Rk);
	y = exp(-x);
	z = 4*x+y;
	deriv = qi*qk*sqrt(x)*(2-0.5*y)/(Ri*Rk*sqrt(z*z*z));
//	deriv = tau*qi*qk*sqrt(rik_square) * (1-Ri*Rk*exp(-x)) / sqrt((rik_square+Ri*Rk*exp(-x))*(rik_square+Ri*Rk*exp(-x))*(rik_square+Ri*Rk*exp(-x)));
	return deriv;
}


void PolForce::deriv_rik_xk(GroupOfGBAtoms *molecule, int i, int k, double *rik_xk)
{
	double xi[3], xk[3];
	double r_ik;
	int j;
	for (j = 0; j < 3; j++)
	{
		xi[j] = molecule->m_Atoms[i]->position[j];
		xk[j] = molecule->m_Atoms[k]->position[j];
		rik_xk[j] = xk[j] - xi[j];
	}
	r_ik = sqrt((xi[0]-xk[0])*(xi[0]-xk[0])+(xi[1]-xk[1])*(xi[1]-xk[1])+(xi[2]-xk[2])*(xi[2]-xk[2]));
	for (j = 0; j < 3; j++)
		rik_xk[j] /= r_ik;
}


double PolForce::derivGijOverRi(GroupOfGBAtoms *molecule, int i,  int j)
{
        double qi, qj;
        double Ri, Rj;
        double rij_square;
        double sum, x, y, z;

	qi = molecule->m_Atoms[i]->charge;
        Ri = molecule->m_Atoms[i]->Born_radius;
	qj = molecule->m_Atoms[j]->charge;
        Rj = molecule->m_Atoms[j]->Born_radius;
        rij_square = (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[j]->position[0]) *
                     (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[j]->position[0]) +
                     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[j]->position[1]) *
                     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[j]->position[1]) +
                     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[j]->position[2]) *
                     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[j]->position[2]);
        x = rij_square/(4*Ri*Rj);
        y = exp(-x);
        z = (4*x+y)*Ri*Rj;
//	printf("Ri %f Rj %f z %f\n", Ri, Rj, z);
	return -0.5*qi*qj*Rj*(1+x)*y/sqrt(z*z*z);
}

double PolForce::derivGiiOverRi(GroupOfGBAtoms *molecule, int i)
{
        double qi;
        double Ri;

        qi = molecule->m_Atoms[i]->charge;
        Ri = molecule->m_Atoms[i]->Born_radius;
        return -qi*qi/(Ri*Ri);
}


double PolForce::dotProduct(double *x1, double *x2)
{
	return x1[0]*x2[0]+x1[1]*x2[1]+x1[2]*x2[2];
}


/*
void PolForce::derivGpolOverR(double *Gpol_R, GroupOfGBAtoms *molecule)
{
	int i, j;
	double qi, qj;
	double Ri, Rj;
	double rij_square;
	double sum, x, y, z;

        const double e_in = 1.0;
        const double e_out = 80.0;
        double tau = 1.0/e_in - 1.0/e_out;

	for (i = 0; i < molecule->numOfAtoms; i++)
	{
		qi = molecule->m_Atoms[i]->charge;
		Ri = molecule->m_Atoms[i]->Born_radius;
		sum = 0.0;
		for (j = i+1; j < molecule->numOfAtoms; j++)
		{
	                qj = molecule->m_Atoms[j]->charge;
	                Rj = molecule->m_Atoms[j]->Born_radius;
			rij_square = (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[j]->position[0]) * 
				     (molecule->m_Atoms[i]->position[0] - molecule->m_Atoms[j]->position[0]) + 
				     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[j]->position[1]) * 
				     (molecule->m_Atoms[i]->position[1] - molecule->m_Atoms[j]->position[1]) + 
				     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[j]->position[2]) * 
				     (molecule->m_Atoms[i]->position[2] - molecule->m_Atoms[j]->position[2]);
			x = rij_square/(4*Ri*Rj);
			y = exp(-x);
			z = 4*x+y;
			sum += qj*sqrt(Rj)*(1+x)*y/sqrt(z*z*z);
//			sum += 0.5*qj*Rj*(1+x)*exp(-x)/sqrt((rij_square+Ri*Rj*exp(-x))*(rij_square+Ri*Rj*exp(-x))*(rij_square+Ri*Rj*exp(-x)));
		}
		sum += qi/sqrt(Ri);
		Gpol_R[i] = tau*0.5*qi*sum/sqrt(Ri*Ri*Ri);
	}
}
*/
