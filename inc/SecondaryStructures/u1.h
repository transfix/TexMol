#ifndef __U1_H__
#define __U1_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/util.h>
#include <SecondaryStructures/robust_cc.h>
#include <SecondaryStructures/intersect.h>
#include <SecondaryStructures/hfn_util.h>
#include <SecondaryStructures/op.h>
#include <SecondaryStructures/degen.h>

void compute_u1(SecondaryStructures::Triangulation& triang, char* op_file_prefix);

#endif
