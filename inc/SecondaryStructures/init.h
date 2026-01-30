#ifndef __INIT_H__
#define __INIT_H__

#include <SecondaryStructures/datastruct_ss.h>

void initialize(SecondaryStructures::Triangulation& triang);
void compute_voronoi_vertex_and_cell_radius(SecondaryStructures::Triangulation& triang);

#endif
