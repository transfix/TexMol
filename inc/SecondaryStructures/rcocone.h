#ifndef __RCOCONE_H__
#define __RCOCONE_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/util.h>
#include <SecondaryStructures/robust_cc.h>

void robust_cocone(const double bb_ratio,
				   const double theta_ff,
				   const double theta_if,
				   SecondaryStructures::Triangulation& triang,
				   const char* outfile_prefix);


#endif
