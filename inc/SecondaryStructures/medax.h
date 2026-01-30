#ifndef __MEDAX_H__
#define __MEDAX_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/util.h>
#include <SecondaryStructures/robust_cc.h>
#include <SecondaryStructures/op.h>

void compute_medial_axis(SecondaryStructures::Triangulation& triang, const double theta, const double medial_ratio, int& biggest_medax_comp_id);

#endif
