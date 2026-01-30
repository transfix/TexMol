#ifndef ROBUST_CC_H
#define ROBUST_CC_H

#include <SecondaryStructures/datastruct_ss.h>

//using namespace SecondaryStructures;

SecondaryStructures::Point nondg_voronoi_point(
	const SecondaryStructures::Point& a,
	const SecondaryStructures::Point& b,
	const SecondaryStructures::Point& c,
	const SecondaryStructures::Point& d,
	bool& is_correct_computation);

SecondaryStructures::Point dg_voronoi_point(
	const SecondaryStructures::Point& a,
	const SecondaryStructures::Point& b,
	const SecondaryStructures::Point& c,
	const SecondaryStructures::Point& d,
	bool& is_correct_computation);

SecondaryStructures::Point nondg_cc_tr_3(const SecondaryStructures::Point& a,
					const SecondaryStructures::Point& b,
					const SecondaryStructures::Point& c,
					bool& is_correct_computation);

SecondaryStructures::Point cc_tr_3(const SecondaryStructures::Point& a,
			  const SecondaryStructures::Point& b,
			  const SecondaryStructures::Point& c);

double sq_cr_tr_3(const SecondaryStructures::Point& a,
				  const SecondaryStructures::Point& b,
				  const SecondaryStructures::Point& c);

SecondaryStructures::Point circumcenter(const SecondaryStructures::Facet& f);

double circumradius(const SecondaryStructures::Facet& f);

#endif
