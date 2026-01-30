#ifndef __HFN_UTIL_H__
#define __HFN_UTIL_H__

#include <SecondaryStructures/datastruct_ss.h>
#include <SecondaryStructures/util.h>
#include <SecondaryStructures/intersect.h>


bool is_maxima(const SecondaryStructures::Cell_handle& c);
bool is_outflow(const SecondaryStructures::Facet& f);
bool is_transversal_flow(const SecondaryStructures::Facet& f);
bool find_acceptor(const SecondaryStructures::Cell_handle& c, const int& id, int& uid, int& vid, int& wid);
bool is_acceptor_for_any_VE(const SecondaryStructures::Triangulation& triang, const SecondaryStructures::Edge& e);
bool is_i2_saddle(const SecondaryStructures::Facet& f);
bool is_i1_saddle(const SecondaryStructures::Edge& e, const SecondaryStructures::Triangulation& triang);
void grow_maxima(SecondaryStructures::Triangulation& triang, SecondaryStructures::Cell_handle c_max);
void find_flow_direction(SecondaryStructures::Triangulation& triang);

#endif
