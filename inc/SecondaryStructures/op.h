#ifndef __OP_H__
#define __OP_H__

#include <SecondaryStructures/hfn_util.h>
#include <SecondaryStructures/skel.h>
#include <Geometry/Geometry.h>
#include <SecondaryStructures/datastruct_ss.h>

void draw_Ray(const SecondaryStructures::Ray_3 & myRay_3,
			  const double& r,
			  const double& g,
			  const double& b,
			  const double& a,
			  ofstream& fout);

void draw_segment(const SecondaryStructures::Segment& segment,
				  const double& r,
				  const double& g,
				  const double& b,
				  const double& a,
				  ofstream& fout);

void draw_poly(const vector<SecondaryStructures::Point>& poly,
			   const double& r,
			   const double& g,
			   const double& b,
			   const double& a,
			   ofstream& fout);

void draw_VF(const SecondaryStructures::Triangulation& triang,
			 const SecondaryStructures::Edge& dual_e,
			 const double& r,
			 const double& g,
			 const double& b,
			 const double& a,
			 ofstream& fout);

void draw_tetra(const SecondaryStructures::Cell_handle& cell,
				const double& r,
				const double& g,
				const double& b,
				const double& a,
				ofstream& fout);

void write_wt(const SecondaryStructures::Triangulation& triang,
			  const char* file_prefix);
void write_helix_wrl(Geometry* geom, const char* filename);

void write_sheet_wrl(Geometry* geom, const char* filename);

void write_iobdy(const SecondaryStructures::Triangulation& triang,
				 const char* file_prefix);

void write_axis(const SecondaryStructures::Triangulation& triang,
				const int& biggest_medax_comp_id,
				const char* file_prefix);

void vectors_to_tri_geometry(const std::vector<float>& vertices,
							 const std::vector<unsigned int>& indices,
							 const std::vector<float>& colors,
							 Geometry* geom);

void vectors_to_line_geometry(const std::vector<float>& vertices,
							  const std::vector<unsigned int>& indices,
							  float r, float g, float b, float a,
							  Geometry* geom);

void vectors_to_point_geometry(const std::vector<float>& vertices,
							   const std::vector<float>& colors,
							   Geometry* geom);
#endif
