#ifndef __TCOCONE_H__
#define __TCOCONE_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/util.h>
#include <SecondaryStructures/robust_cc.h>

void compute_poles(SecondaryStructures::Triangulation& triang);
void mark_flat_vertices(SecondaryStructures::Triangulation& triang, double ratio, double cocone_phi, double flat_phi);
void tcocone(const double DEFAULT_ANGLE, const double DEFAULT_SHARP, const double DEFAULT_FLAT, const double DEFAULT_RATIO, SecondaryStructures::Triangulation& triang);

#endif
