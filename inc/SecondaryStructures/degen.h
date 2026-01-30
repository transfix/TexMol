#ifndef __DEGEN_H__
#define __DEGEN_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/op.h>

bool is_degenerate_VF(const SecondaryStructures::Triangulation& triang,
					  const SecondaryStructures::Cell_handle& c,
					  const int& fid,
					  const int& uid,
					  const int& vid,
					  const SecondaryStructures::Point& d,
					  const char* prefix);
#endif
