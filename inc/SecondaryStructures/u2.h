#ifndef __U2_H__
#define __U2_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/util.h>
#include <SecondaryStructures/robust_cc.h>
#include <SecondaryStructures/intersect.h>
#include <SecondaryStructures/hfn_util.h>
#include <SecondaryStructures/op.h>
#include <SecondaryStructures/degen.h>

pair< vector< vector<SecondaryStructures::Cell_handle> >, vector<SecondaryStructures::Facet> >
compute_u2(SecondaryStructures::Triangulation& triang, char* prefix);

#endif
